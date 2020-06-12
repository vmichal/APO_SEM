#pragma once

#include "knobs.hpp"
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

		void die();

	};

	class LocalPlayer : public Player {

		knobs::KnobManager& controller_;
	public:

		LocalPlayer(int id, Game& game, knobs::KnobManager& controller)
			: Player{ id,Type::local, game }, controller_{ controller } {}
		~LocalPlayer() override {}

		Action get_action() override;


	};

	class RemotePlayer : public Player {

		//TODO implement
	public:

		RemotePlayer(int id, Game& game) : Player{ id, Type::remote, game } {}
		~RemotePlayer() override {}

		Action get_action() override;

	};

	class AutonomousPlayer :public  Player {

		//TODO implement
	public:
		AutonomousPlayer(int id, Game& game) : Player{ id, Type::autonomous, game } {}
		~AutonomousPlayer()override {}

		Action get_action() override;

	};




}