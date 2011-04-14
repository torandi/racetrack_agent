#ifndef GAME_H_
#define GAME_H_
	#include "level.h"
	#include "coordinate.h"
	#include <vector>

	#define MAX_SPEED 5

	enum movement_result_t {
		MOVEMENT_OK,
		MOVEMENT_GOAL,
		MOVEMENT_CRASH
	};

	enum sensor_value_t {
		SENSOR_NEAR=0,
		SENSOR_NORMAL=1,
		SENSOR_FAR=2
	};

	/**
	 * Adds [add] to sensor. 
	 */
	void modify_sensor(sensor_value_t &sensor,int add);

	class Game {
			std::vector<Coordinate> _positions;
			level_data_t _lvl_data; //The current level data (wall/road etc)
			Level * _level;

			/* Returns false if the move was allowed, true otherwise */
			bool perform_move(char move);
			sensor_value_t sensor_value(float dir,int th_near,int th_normal);
		public:
			Coordinate momentum;
			Coordinate position;
			movement_result_t last_movement_result;
			bool debug;
        
			static movement_result_t evaluate_lvl_data(level_data_t prev,level_data_t cur);

			Game();
			Game(Level * level);
			/**
			 * Move is an integer between 0 and 8 representing these movements:
			 * The movements are relative to the currect momentum. If momentum is 0 up is up on the screen
			 * 1 2 3
			 * 4 0 5
			 * 6 7 8
			 */
			movement_result_t move(char move);

			void get_sensors(sensor_value_t (&values)[3],int th_near,int th_normal);	

			int num_moves();

			friend class Graphics;
            friend class GeneticAlgorithm;
	};
#endif
