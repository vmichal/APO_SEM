#pragma once
#include <stdio.h>
#include <map>
#include <functional>
#include <assert.h>
#include <array>

namespace helpers {

	template<typename ENUM>
	struct StateFun {
		ENUM from_;
		std::function<void()> fun_;
	};

	template<typename ENUM>
	struct StateFunState {
		ENUM from_;
		std::function<void()> fun_;
		ENUM to_;
	};


	template<typename ENUM>
	helpers::StateFun<ENUM> operator>>(ENUM from, std::function<void()> transition) {
		return { from, std::move(transition) };
	}

	template<typename ENUM>
	helpers::StateFunState<ENUM> operator>>(helpers::StateFun<ENUM> lhs, ENUM to) {
		return { lhs.from_, std::move(lhs.fun_), to };
	}

}

template<typename E>
class StateMachine {

public:

	struct State {
		E id_;
		std::function<void()> loop_;
		std::map<E, std::function<void()>> transitions_;
	};

private:
	std::map<E, State> states_;
	E current_;

public:

	StateMachine(E starting_state) : current_{ starting_state } {}

	E current() const { return current_; }
	void loop() const { states_.at(current_).loop_(); }

	void add_state(helpers::StateFun<E> data) {
		assert(states_.count(data.from_) == 0);

		states_[data.from_].id_ = data.from_;
		states_[data.from_].loop_ = data.fun_;
	}

	void add_transition(helpers::StateFunState<E> data) {
		assert(states_.count(data.from_));
		assert(states_.count(data.to_));

		states_[data.from_].transitions_[data.to_] = data.fun_;
	}

	void perform_transition(E to) {
		assert(states_[current_].transitions_.count(to));

		const char* to_string(E);

		printf("STATE: Transition from %s to %s.\n", to_string(current_), to_string(to));

		states_[current_].transitions_[to]();
		current_ = to;
	}

};



