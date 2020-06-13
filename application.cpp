
#include "application.hpp"
#include "state_machine.hpp"
#include "snake-options.hpp"
#include "text.hpp"
#include "display.hpp"
#include "menu.hpp"
#include "audio.hpp"
#include "knobs.hpp"
#include "led-line.hpp"
#include "map.hpp"

#include <sstream>
#include <utility>
#include <algorithm>
#include <iomanip>

namespace {
	void show_menu(menu::menus_t menu) {
		menu::display(menu);
		display_lcd();
	}

}

Application::Application()
	: state_machine_{ State::init }, help_{ "menus/help.txt" } {
	using namespace helpers;

	state_machine_.add_state(State::init >> [&] { state_machine_.perform_transition(State::welcome_screen); });
	state_machine_.add_state(State::welcome_screen >> std::bind(&Application::welcome_screen_loop, this));
	state_machine_.add_state(State::main_menu >> std::bind(&Application::main_menu_loop, this));
	state_machine_.add_state(State::help >> std::bind(&Application::help_loop, this));
	state_machine_.add_state(State::settings >> std::bind(&Application::settings_loop, this));
	state_machine_.add_state(State::map_selection >> std::bind(&Application::map_selection_loop, this));
	state_machine_.add_state(State::ingame >> std::bind(&Application::ingame_loop, this));
	state_machine_.add_state(State::pause >> std::bind(&Application::pause_loop, this));
	state_machine_.add_state(State::ended >> [] { /*Nothing to do in the loop*/});

	state_machine_.add_transition(State::init >> [] { welcome_screen(); display_lcd(); } >> State::welcome_screen);
	state_machine_.add_transition(State::welcome_screen >> std::bind(show_menu, menu::MAIN_MENU) >> State::main_menu);
	state_machine_.add_transition(State::main_menu >> [] {/*TODO do I need to do something?*/} >> State::settings);
	state_machine_.add_transition(State::main_menu >> [&] {help_line_ = 0; redraw_help(); } >> State::help);
	state_machine_.add_transition(State::main_menu >> [&] { settings_.map_index = 0; show_map(); } >> State::map_selection);
	state_machine_.add_transition(State::main_menu >> [] { closing_screen(); display_lcd(); } >> State::ended);

	state_machine_.add_transition(State::settings >> std::bind(show_menu, menu::MAIN_MENU) >> State::main_menu);
	state_machine_.add_transition(State::help >> std::bind(show_menu, menu::MAIN_MENU) >> State::main_menu);

	state_machine_.add_transition(State::map_selection >> std::bind(&Application::start_game, this) >> State::ingame);

	state_machine_.add_transition(State::ingame >> [&] {game_->pause(); show_menu(menu::PAUSED_MENU); } >> State::pause);
	state_machine_.add_transition(State::pause >> [&] {game_->resume(); } >> State::ingame);
	state_machine_.add_transition(State::pause >> [&] {std::bind(show_menu, menu::MAIN_MENU)(); game_.reset(); } >> State::main_menu);
}

void Application::process() {
	//Sample all input knobs
	std::for_each(knobs::knobs.begin(), knobs::knobs.end(), std::mem_fn(&knobs::KnobManager::sample));

	state_machine_.loop();
}

void Application::welcome_screen_loop() {
	using namespace std::chrono_literals;
	static std::chrono::steady_clock::time_point last_audio;
	static std::chrono::steady_clock::time_point last_blink;
	static int leds = 0;

	if (std::chrono::steady_clock::now() - last_audio > welcome_screen_beep_frequency) { //Beep periodically
		pwm::audio.playing(!pwm::audio.playing());
		last_audio = std::chrono::steady_clock::now();
	}

	if (std::chrono::steady_clock::now() - last_blink > welcome_screen_led_frequency) {
		std::uint32_t const mask = (1 << leds) | (1 << (LED_line_length - 1 - leds));
		leds = (leds + 1) % (LED_line_length / 2);
		led::line.write(mask);

		last_blink = std::chrono::steady_clock::now();
	}

	if (std::any_of(knobs::knobs.begin(), knobs::knobs.end(), std::mem_fn(&knobs::KnobManager::pressed))) {
		pwm::audio.turn_off();
		state_machine_.perform_transition(State::main_menu);
	}
}

void Application::main_menu_loop() {

	switch (knobs::green.movement()) {
	case knobs::Rotation::none: break;
	case knobs::Rotation::clockwise:
		menu::move_selected(DOWN, menu::MAIN_MENU);
		display_lcd();
		break;
	case knobs::Rotation::counterclockwise:
		menu::move_selected(UP, menu::MAIN_MENU);
		display_lcd();
		break;
	default: assert(false);
	}

	if (knobs::green.pressed()) {
		switch (menu::get_selected(menu::MAIN_MENU)) { //TODO make this API a bit more sensible
		case menu::NEW_GAME_OPT:
			return state_machine_.perform_transition(State::map_selection);
		case menu::SETTINGS_OPT:
			return state_machine_.perform_transition(State::settings);
		case menu::HELP_OPT:
			return state_machine_.perform_transition(State::help);
		case menu::QUIT_OPT:
			return state_machine_.perform_transition(State::ended);
		default: assert(false);
		}
	}

}

void Application::settings_loop() {
	//TODO implement
	state_machine_.perform_transition(State::main_menu);
}

void Application::help_loop() {

	if (knobs::Rotation rot = knobs::green.movement(); rot != knobs::Rotation::none) {
		help_line_ += rot == knobs::Rotation::clockwise ? -1 : 1;
		help_line_ = std::clamp<int>(help_line_, 0, help_.size() - 1);
		redraw_help();
	}

	if (std::any_of(knobs::knobs.begin(), knobs::knobs.end(), std::mem_fn(&knobs::KnobManager::pressed)))
		state_machine_.perform_transition(State::main_menu);

}

void Application::show_map() const {
	flood_fill_lcd(game::colors::bg);
	unsigned const map_count = game::Map::maps().size();
	assert(settings_.map_index >= 0);
	assert(settings_.map_index < map_count);
	game::Map const& map = game::Map::maps()[settings_.map_index];
	map.draw();

	//Draw starting positions in a different color
	for (auto const [col, row] : map.starting_positions())
		fill_square_lcd(col, row, game::colors::starting_position);

	std::ostringstream map_index;
	map_index << (settings_.map_index + 1) << '/' << map_count << ": map "
		<< map.size().x << 'x' << map.size().y << ", " << map.starting_positions().size() << " players.";
	write_line_to_display(MAX_LINE_NUMBER - 2, map_index.str().c_str(), game::colors::wall, game::colors::bg);

	display_lcd();
}

void Application::map_selection_loop() {

	if (knobs::blue.pressed()) {
		if (settings_.map_index != game::Map::maps().size() - 1)
			++settings_.map_index;
		show_map();
	}

	if (knobs::red.pressed()) {
		if (settings_.map_index)
			--settings_.map_index;
		show_map();
	}

	if (knobs::green.pressed())
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
		menu::move_selected(DOWN, menu::PAUSED_MENU);
		display_lcd();
		break;
	case knobs::Rotation::counterclockwise:
		menu::move_selected(UP, menu::PAUSED_MENU);
		display_lcd();
		break;
	default: assert(false);
	}

	if (knobs::green.pressed()) {
		switch (menu::get_selected(menu::PAUSED_MENU)) { //TODO make this API a bit more sensible
		case menu::PLAY_OPT:
			return state_machine_.perform_transition(State::ingame);
		case menu::RETURN_OPT:
			return state_machine_.perform_transition(State::main_menu);
		default:
			assert(false);
		}
	}

}

void Application::start_game() {
	assert(!game_);
	game_ = std::make_unique<game::Game>(game::Map::maps()[settings_.map_index]);

	//TODO add real players
	game_->add_player(game::Player::Type::autonomous);
	game_->add_player(game::Player::Type::autonomous);
	game_->start();
}

void Application::redraw_help() const {
	help_.display_help(help_line_);

	//Display scrolling information
	std::ostringstream line_number;
	line_number << help_line_ << '/' << help_.size();
	write_line_to_display(MAX_LINE_NUMBER - 1, line_number.str().c_str(), BLACK, WHITE);

	display_lcd();
}

const char* to_string(Application::State s) {
	switch (s) {
	case Application::State::init: return "init";
	case Application::State::welcome_screen: return "welcome_screen";
	case Application::State::main_menu: return "main_menu";
	case Application::State::settings: return "settings";
	case Application::State::help: return "help";
	case Application::State::map_selection: return "map_selection";
	case Application::State::ingame: return "ingame";
	case Application::State::pause: return "pause";
	case Application::State::ended: return "ended";
	default: assert(false);
	}
}
