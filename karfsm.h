#pragma once

#ifdef KARFSM_EXPORTS
#define KARFSM_API _declspec(dllexport)
#else
#define KARFSM_API _declspec(dllimport)
#endif

enum class options { running = 0, ok = 2, fail = 3 };
enum class actions { walk = 0, stop = 1, kick = 2, punch = 3 };

struct FSM_NODE
{
	actions current_action = actions::stop;
	options current_state = options::ok;
	actions first_choice = actions::stop;
	actions second_choice = actions::stop;
	
	int ranking1 = 0; // first choice
	int ranking2 = 0;

	int attack_delay = 100;
};

class KARFSM_API ACTIONCLASS
{
	private:
		
		int object_lifes = 0;
		int enemy_lifes = 0;
		float distance_to_enemy = 0;

		ACTIONCLASS(int _my_lifes, int _enemy_lifes, float _distance);
		
	public:
		FSM_NODE Node;

		static void CreateFSM(int _my_lifes, int _enemy_lifes, float _distance, ACTIONCLASS** new_object);

		void SetInfo(int _my_lifes, int _enemy_lifes, float _distance);
		
		actions WhatToDo(bool in_fight = false);

		void Release()
		{
			delete this;
		}
		
};