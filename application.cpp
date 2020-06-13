
#include "application.hpp"
#include "state_machine.hpp"
#include "text.hpp"
#include "display.hpp"
#include "menu.hpp"
#include "audio.hpp"
#include "knobs.hpp"
#include "led-line.hpp"

#include <utility>

namespace {
	void show_main_menu() {
		display_menu(0);
		display_lcd();
	}

}

Application::Application()
	: state_machine_{ State::init } {
	using namespace helpers;

	state_machine_.add_state(State::init >> [&] { state_machine_.perform_transition(State::welcome_screen); });
	state_machine_.add_state(State::welcome_screen >> std::bind(&Application::welcome_screen_loop, this));
	state_machine_.add_state(State::main_menu >> std::bind(&Application::main_menu_loop, this));
	state_machine_.add_state(State::settings >> std::bind(&Application::settings_loop, this));
	state_machine_.add_state(State::start_game >> std::bind(&Application::start_game_loop, this));
	state_machine_.add_state(State::ingame >> std::bind(&Application::ingame_loop, this));
	state_machine_.add_state(State::pause >> std::bind(&Application::pause_loop, this));
	state_machine_.add_state(State::ended >> [] { /*Nothing to do in the loop*/});

	state_machine_.add_transition(State::init >> [] { welcome_screen(); display_lcd(); } >> State::welcome_screen);
	state_machine_.add_transition(State::welcome_screen >> show_main_menu >> State::main_menu);
	state_machine_.add_transition(State::main_menu >> [] {} >> State::settings);
	state_machine_.add_transition(State::main_menu >> [] {} >> State::start_game);
	state_machine_.add_transition(State::main_menu >> [] {} >> State::ended);

	state_machine_.add_transition(State::settings >> show_main_menu >> State::main_menu);

	state_machine_.add_transition(State::start_game >> std::bind(&Application::start_game, this) >> State::ingame);
	state_machine_.add_transition(State::start_game >> show_main_menu >> State::main_menu);

	state_machine_.add_transition(State::ingame >> [&] {game_->pause(); display_menu(1); display_lcd(); } >> State::pause);
	state_machine_.add_transition(State::pause >> [&] {game_->resume(); } >> State::ingame);
	state_machine_.add_transition(State::pause >> show_main_menu >> State::main_menu);
}

void Application::process() {
	//Sample all input knobs
	std::for_each(knobs::knobs.begin(), knobs::knobs.end(), std::mem_fn(&knobs::KnobManager::sample));

	state_machine_.loop();
}

void Application::welcome_screen_loop() {
	using namespace std::chrono_literals;
	static std::chrono::steady_clock::time_point last_music = std::chrono::steady_clock::now();
	static std::chrono::steady_clock::time_point last_led = std::chrono::steady_clock::now();
	static int leds = 0;

	if (std::chrono::steady_clock::now() - last_music > 200ms) { //Beep periodically
		pwm::audio.playing(!pwm::audio.playing());
		last_music = std::chrono::steady_clock::now();
	}

	if (std::chrono::steady_clock::now() - last_led > 50ms) {
		std::uint32_t const mask = 1 << leds | 1 << (LED_line_length - 1 - leds);
		leds = (leds + 1) % (LED_line_length / 2);
		led::line.write(mask);

		last_led = std::chrono::steady_clock::now();
	}

	for (auto& knob : knobs::knobs)
		if (knob.pressed()) {
			pwm::audio.turn_off();
			state_machine_.perform_transition(State::main_menu);
			break;
		}
}

void Application::main_menu_loop() {

	switch (knobs::green.movement()) {
	case knobs::Rotation::none: break;
	case knobs::Rotation::clockwise:
		move_selected(DOWN, 0);
		display_lcd();
		break;
	case knobs::Rotation::counterclockwise:
		move_selected(UP, 0);
		display_lcd();
		break;
	default: assert(false);
	}

	if (knobs::green.pressed()) {
		switch (get_selected(0)) { //TODO make this API a bit more sensible
		case 0:
			return state_machine_.perform_transition(State::start_game);
		case 1:
			return state_machine_.perform_transition(State::settings);
		case 2:
			return state_machine_.perform_transition(State::ended);
		}
	}

}

void Application::settings_loop() {
	//TODO implement
	state_machine_.perform_transition(State::main_menu);
}

void Application::start_game_loop() {
	//TODO implement
	state_machine_.perform_transition(State::ingame);
}

void Application::ingame_loop() {
	assert(game_);

	if (game_->frame_elapsed()) {
		game_->update();
		game_->draw();
	}

	if (knobs::green.pressed())
		state_machine_.perform_transition(State::pause);


}

void Application::pause_loop() {

	switch (knobs::green.movement()) {
	case knobs::Rotation::none: break;
	case knobs::Rotation::clockwise:
		move_selected(DOWN, 1);
		display_lcd();
		break;
	case knobs::Rotation::counterclockwise:
		move_selected(UP, 1);
		display_lcd();
		break;
	default: assert(false);
	}

	if (knobs::green.pressed()) {
		switch (get_selected(1)) { //TODO make this API a bit more sensible
		case 0:
			return state_machine_.perform_transition(State::ingame);
		case 1:
			return state_machine_.perform_transition(State::main_menu);
		}
	}

}

void Application::start_game() {
	assert(!game_);
	game_ = std::make_unique<game::Game>(LCD_WIDTH / settings_.square_size, LCD_HEIGHT / settings_.square_size);

	//TODO add real players
	game_->add_player(game::Player::Type::local);
	game_->add_player(game::Player::Type::local);
	game_->start();
}

const char* to_string(Application::State s) {
	switch (s) {
	case Application::State::init: return "init";
	case Application::State::welcome_screen: return "welcome_screen";
	case Application::State::main_menu: return "main_menu";
	case Application::State::settings: return "settings";
	case Application::State::start_game: return "start_game";
	case Application::State::ingame: return "ingame";
	case Application::State::pause: return "pause";
	case Application::State::ended: return "ended";
	default: assert(false);
	}
}