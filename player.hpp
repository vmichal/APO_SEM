#pragma once

#include "knobs.hpp"
#include "map.hpp"
#include "led-rgb.hpp"
#include <memory>

namespace game {

	class Game;
	class Snake;


	class Player {

	public:

		enum class Action {
			none,
			turn_left,
			turn_right,
			use_powerup
		};

		enum class Type {
			local,
			remote,
			autonomous
		};

		int const id_;
		Type const type_;
		bool dead_;

	protected:
		std::unique_ptr<Snake> snake_;
		Game& my_game_;

	public:

		Player(int id, Type type, Game& game)
			: id_{ id }, type_{ type }, my_game_{ game } {}
		virtual ~Player() {};

		virtual Action get_action() = 0;

		Snake* snake() const { return snake_.get(); }
		int id() const { return id_; }
		int score() const;
		void reset_snake();
		bool dead() const { return dead_; }

		void die();

	};

	class LocalPlayer : public Player {

		knobs::KnobManager& controller_;
	public:
		led::RGB_LED& powerup_led_;

		LocalPlayer(int id, Game& game, knobs::KnobManager& controller, led::RGB_LED& led)
			: Player{ id,Type::local, game }, controller_{ controller }, powerup_led_{ led } {}
		~LocalPlayer() override {}

		Action get_action() override;


	};

	class RemotePlayer : public Player {
		//For lack of multiple testing boards, we have postponed this functionality till
		//some indefinite time in future. Maybe a semestral assignment from Computer networks
		//TODO implement
	public:

		RemotePlayer(int id, Game& game) : Player{ id, Type::remote, game } {}
		~RemotePlayer() override {}

		Action get_action() override { throw 42; /*Not implemented*/ }

	};

	class AutonomousPlayer :public  Player {

		struct SquareData {
			int distance;
			Direction desired_dir;
			bool reachable;
		};

		static std::vector<std::vector<SquareData>> to_food_;
		static std::vector<std::vector<SquareData>> to_powerup_;
		static bool powerup_lives_;

		static void bfs(Map const& map, coord const start, std::vector<std::vector<SquareData>>& target);
		Direction find_desired_direction(SquareData food, SquareData powerup) const;
	public:
		AutonomousPlayer(int id, Game& game) : Player{ id, Type::autonomous, game } {}
		~AutonomousPlayer()override {}

		Action get_action() override;


		static void learn_map(Game const& game);
		static void consider_actions(Game const& game);
	};




}