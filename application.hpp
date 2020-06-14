#pragma once

/*
	Computer architectures semestral assignment - Zelinka Josef & Michal Vojtech

	This file contains the high level state machine of the application.
	One object of type Application is constructed in main and constrols the application flow. 
	Graph of internal state machine can be found in docs/application-states.pdf.
*/

#include "state_machine.hpp"
#include "snake-options.hpp"
#include "game.hpp"
#include "help.hpp"
#include "map.hpp"
#include <memory>

struct Configuration {
	unsigned autonomous_players = 0, local_players = 0;
	unsigned map_index = 0;
	unsigned fps = 30;
};

class Application {

public:
	enum class State {
		init, //immediatelly goes to welcome_screen (we only use it for the transition function)
		welcome_screen, //Displays big hello, blinks some LEDs, plays some music. Just fancy invitation
		main_menu, //Allows printing of help, leaving the application and starting new game
		help, //scrollable menu with program help
		map_selection, //before starting new game, selects one of predefined maps specified in the maps folder
		player_selection, //selects the number of players
		ingame, //Game is currently running. Knob input is redirected to snakes, display is redrawn
		pause, //Pause menu with some information about the game and the option to leave
		display_score, //Player standings - results of the match
		ended //End state, when the application exits
	};

private:
	StateMachine<State> state_machine_;
	std::unique_ptr<game::Game> game_;
	Configuration settings_;
	help::Help help_;
	int help_line_;

	void welcome_screen_loop();
	void main_menu_loop();
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

	void display_pause_info() const;

public:
	Application();

	bool running() const { return state_machine_.current() != State::ended; }
	void process();


};

const char* to_string(Application::State s);

