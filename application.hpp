#pragma once

#include "state_machine.hpp"
#include "snake-options.hpp"
#include "game.hpp"

#include <memory>

struct Configuration {
	int square_size = SIDE;
};

class Application {

public:
	enum class State {
		init,
		welcome_screen,
		main_menu,
		settings,
		start_game,
		ingame,
		pause,
		ended
	};

private:
	StateMachine<State> state_machine_;
	std::unique_ptr<game::Game> game_;
	Configuration settings_;

	void welcome_screen_loop();
	void main_menu_loop();
	void settings_loop();
	void start_game_loop();
	void ingame_loop();
	void pause_loop();

	void start_game();

public:
	Application();

	bool running() const { return state_machine_.current() != State::ended; }
	void process();


};

const char* to_string(Application::State s);

