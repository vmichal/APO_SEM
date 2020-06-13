#pragma once

#include "state_machine.hpp"
#include "snake-options.hpp"
#include "game.hpp"
#include "help.hpp"
#include "map.hpp"
#include <memory>

struct Configuration {
	unsigned autonomous_players = 0, local_players = 0;
	unsigned map_index = 0;
};

class Application {

public:
	enum class State {
		init,
		welcome_screen,
		main_menu,
		help,
		settings,
		map_selection,
		player_selection,
		ingame,
		pause,
		display_score,
		ended
	};

private:
	StateMachine<State> state_machine_;
	std::unique_ptr<game::Game> game_;
	Configuration settings_;
	help::Help help_;
	int help_line_;

	void welcome_screen_loop();
	void main_menu_loop();
	void settings_loop();
	void help_loop();
	void map_selection_loop();
	void player_selection_loop();
	void ingame_loop();
	void pause_loop();
	void display_score_loop();

	void start_game();
	void redraw_help() const;
	void show_map() const;
	void show_players() const;

public:
	Application();

	bool running() const { return state_machine_.current() != State::ended; }
	void process();


};

const char* to_string(Application::State s);

