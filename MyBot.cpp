#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "Game_Api.h"
#include <vector>
#include <iostream>
using json = nlohmann::json;
using Player = Game_Api::Player;
using Monster = Game_Api::Monster;
using DeathEffects = Game_Api::DeathEffects;

#define RESPONSE_SECS 1
#define RESPONSE_NSECS 0

#include <iostream>
using namespace std;

int main() {
	//declare some variables that last the entire game
	vector<node_id_t> desired_path;
	bool on_route = false;
	int current_steps_taken_in_path = 0;

	Game_Api * api;
	int my_player_num = 0;
	while(1){
		char* buf = NULL;
		size_t size = 0;
		getline(&buf, &size, stdin);
		json data = json::parse(buf);
		if(data["type"] == "map"){
			my_player_num = data["player_id"];
			api = new Game_Api(my_player_num, data["map"]);
		} else {
			api->update(data["game_data"]);

//This is where they insert their data///////////////////////////////////


		 	Player me = api->get_self();
			Player opponent = api->get_opponent();
			node_id_t destination_decision = me._location;
			string stance = "Rock";
			//you dont want to perform the operation below every turn. Only if you have reached your final destination
			if (!on_route) {

				//TODO change next line
			if (1) {
				//Want to kill the nearest monster of type "Paper 6"
				vector<Monster> monsters = api->nearest_monsters(me._location, 1);
				//if any monsters of that type were found
				if (monsters.size() > 0)  {
					//look for Speed
					// int s;
					// for(int i = 0; i < monsters.size(); i++){
					// 	if(monsters[i]._death_effects._speed != 0){
					// 		//monsters.erase(0);
					// 		s = i;
					// 		break;
					// 		//Monster target = monsters[i];
					// 	}
					// }

					//Monster target = monsters[0];
					Monster target = monsters[0];
					if(target._stance == "Rock") stance = "Paper";
					else if(target._stance == "Paper") stance = "Scissors";
					else if(target._stance == "Scissors") stance = "Rock";

					//vector<vector<node_id_t>> shortest_paths = api->shortest_paths(me._location, target._location);
					vector<vector<node_id_t>> shortest_paths = api->shortest_paths(me._location, 0);

					//if(monter)

					// TODO see if we can do if the monster at 0's counter will be done
					// by the time we would get there
					if(api->has_monster(0)){

						Monster zeromonster = api->get_monster(0);
						if(!zeromonster._dead){
							int zeromonsterCounter = zeromonster._respawn_counter;
							vector<vector<node_id_t>> pathToZero = api->shortest_paths(me._location, 0);
							if(pathToZero[0].size() >= zeromonsterCounter){
								shortest_paths = api->shortest_paths(me._location, 0);
							}
						}

					}
					// if(me._health < 100 && me._location != 0 && api->has_monster(0)){
					// 	//std::cout << "health is less than 100 :()" << std::endl;
					// 	shortest_paths = api->shortest_paths(me._location, 0);
					//
					// }
//TODO fix monster not dying in time
					else{
						//std::cout << "health is greater than 100" << std::endl;

						int total_reward = 0;
						for(int i = 0; i < 4; i++){
							if(monsters[i]._death_effects._speed + monsters[i]._death_effects._health + monsters[i]._death_effects._rock + monsters[i]._death_effects._paper + monsters[i]._death_effects._scissors > total_reward){
								total_reward = monsters[i]._death_effects._speed + monsters[i]._death_effects._health + monsters[i]._death_effects._rock + monsters[i]._death_effects._paper + monsters[i]._death_effects._scissors;
								target = monsters[i];
							}
						}
						shortest_paths = api->shortest_paths(me._location, target._location);
					}



					//Want to take the least dangerous path, so sum monster attacks on each path
					vector<int> cumulative_monster_attacks_in_path;
					for (size_t i = 0; i < shortest_paths.size(); i++) {
						vector<node_id_t> path = shortest_paths[i];
						cumulative_monster_attacks_in_path.push_back(0);
						for (node_id_t node : path) {
							if (api->has_monster(node)){
								Monster mon = api->get_monster(node);
								cumulative_monster_attacks_in_path[i] += mon._attack;
							}
						}
					}
					//find the path with least cumulative monster attack
					int min_cumulative_attack = 9999;
					int desired_path_idx;
					for (size_t i = 0; i < cumulative_monster_attacks_in_path.size(); i++){
						if (cumulative_monster_attacks_in_path[i] < min_cumulative_attack) {
							min_cumulative_attack = cumulative_monster_attacks_in_path[i];
							desired_path_idx = i;
						}
					}
				    desired_path = shortest_paths[desired_path_idx];
					on_route = true;
				     }
			     }
		     }

				 // if(me._location == 0)
				 // vector<vector<node_id_t>> shortest_paths = api->shortest_paths(me._location, 3);
				 // else if(me._location == 3)
				 // vector<vector<node_id_t>> shortest_paths = api->shortest_paths(me._location, 0);
				 // else if(me._location == )
				 // vector<vector<node_id_t>> shortest_paths = api->shortest_paths(me._location, 0);

				 //on_route = true;



			 if (on_route) {

				 //if(me._health < 100){
					 if(me._location == desired_path.back()) {
						  on_route = false;
						 desired_path.clear();
						 current_steps_taken_in_path = 0;
					 } else if(me._location == desired_path[current_steps_taken_in_path]) {
						 current_steps_taken_in_path++;
						 destination_decision = desired_path[current_steps_taken_in_path];
					 } else {
			     	     destination_decision = desired_path[current_steps_taken_in_path];
				     }

				 //}
				 // if(me._location == 0) destination_decision = 1;
				 // else if(me._location == 1) destination_decision = 3;
				 // else if(me._location == 3) destination_decision = 2;
				 // else if(me._location == 2) destination_decision = 4;
				 // else if(me._location == 4) destination_decision = 5;
				 // else if(me._location == 5) destination_decision = 6;
				 // else if(me._location == 6) destination_decision = 0;



			 }
			 if(me._location == opponent._location){
				 if(opponent._stance == "Rock") stance = "Paper";
				 else if(opponent._stance == "Paper") stance = "Scissors";
				 else if(opponent._stance == "Scissors") stance = "Rock";
			 }


///////////////////////////////////////////////////////////////////////////////


			  api->submit_decision(destination_decision, stance);
		 	 fflush(stdout);
			 free(buf);
		}
	}
}
