
#include "knobs.hpp"

namespace game {

	enum class Action {
		none,
		turn_left,
		turn_right,
		use_powerup
	};

	class Player {


	public:

		virtual Action get_action() = 0;

	};

	class LocalPlayer : Player {

		knobs::KnobManager& controller_;
	public:

		LocalPlayer(knobs::KnobManager& controller) :controller_{ controller } {}

		Action get_action() override;


	};

	class RemotePlayer : Player {

		//TODO implement
	public:
		Action get_action() override;

	};

	class AutonomousPlayer : Player {

		//TODO implement
	public:
		Action get_action() override;

	};




}