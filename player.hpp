
#include "knobs.hpp"

namespace game {

	class Game; class Snake;

	enum class Action {
		none,
		turn_left,
		turn_right,
		use_powerup
	};

	class Player {

	public:

		Game const& my_game_;
		Snake const& my_snake_;

		Player(Game const& game, Snake const& snake)
			:my_game_{ game }, my_snake_{ snake } {}

		virtual Action get_action() = 0;

	};

	class LocalPlayer : Player {

		knobs::KnobManager& controller_;
	public:

		LocalPlayer(Game const& game, Snake const& snake, knobs::KnobManager& controller)
			: Player{ game, snake }, controller_{ controller } {}

		Action get_action() override;


	};

	class RemotePlayer : Player {

		//TODO implement
	public:

		RemotePlayer(Game const& game, Snake const& snake)
			: Player{ game, snake } {}

		Action get_action() override;

	};

	class AutonomousPlayer : Player {

		//TODO implement
	public:
		AutonomousPlayer(Game const& game, Snake const& snake)
			: Player{ game, snake } {}

		Action get_action() override;

	};




}