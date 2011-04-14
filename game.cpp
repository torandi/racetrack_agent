#include "game.h"
#include "coordinate.h"
#include <cmath>
#include <vector>
#include <cstdio>
#include <cstdlib>

//Public non-scoped function, should probably be moved to sensors.cpp or something
void modify_sensor(sensor_value_t &sensor,int add) {
	sensor=(sensor_value_t)( (int)sensor+add);
}

Game::Game() : _lvl_data(LEVEL_DATA_ROAD), debug(false) { }

Game::Game(Level * level) : _level(level),debug(false) {
	_level = level;
	position=level->startPosition();
	_lvl_data=level->get(position);
	_positions.push_back(position);
	last_movement_result=MOVEMENT_OK;
}

int radians_to_degrees(double rad) {
	return (int) (rad * (180/PI));
}

bool Game::perform_move(char move) {
	Coordinate rel(0,0);
	switch(move) {
		case 0: 
			rel.x=0;
			rel.y=0;
			break;
		case 1:
			rel.x=-1;
			rel.y=1;
			break;
		case 2:
			rel.x=0;
			rel.y=1;
			break;
		case 3:
			rel.x=1;
			rel.y=1;
			break;
		case 4:
			rel.x=-1;
			rel.y=0;
			break;
		case 5:
			rel.x=1;
			rel.y=0;
			break;
		case 6:
			rel.x=-1;
			rel.y=-1;
			break;
		case 7:
			rel.x=0;
			rel.y=-1;
			break;
		case 8:
			rel.x=1;
			rel.y=-1;
			break;
	}
	//Rotate to be relative
	float angle=momentum.angle();
	angle=round(4*angle/PI)*(PI/4); //(angle/(PI/4)), deal only in 45 degrees multiples

	if(debug)
		std::printf("Momentum angle: %i, modifed angle: %i. Current rel: (%i,%i)",radians_to_degrees(momentum.angle()),radians_to_degrees(angle),rel.x,rel.y);

	rel.rotate(angle);
	if(debug) std::printf("res: (%i,%i)\n",rel.x,rel.y);
	
	momentum+=rel;

	bool result=true;
	/** 
	 * Make sure we don't excide the maximum speed
	 */
	if(abs(momentum.x)>MAX_SPEED || abs(momentum.y)>MAX_SPEED) {
		//Revert
		momentum-=rel;
		result=false;	
	}
	
	position+=momentum;
	_positions.push_back(position);
	return result;
}

int Game::num_moves() {
	return _positions.size();
}

movement_result_t Game::move(char move) {
	level_data_t last_type=_lvl_data;
	bool move_result=perform_move(move);
	_lvl_data=_level->get(position);
	if(move_result) {
		last_movement_result=evaluate_lvl_data(last_type,_lvl_data);
	} else {
		last_movement_result=MOVEMENT_CRASH;
	}
	return last_movement_result;
}

movement_result_t Game::evaluate_lvl_data(level_data_t prev,level_data_t cur) {
	switch(cur) {
		case LEVEL_DATA_ROAD:
			return MOVEMENT_OK;
		case LEVEL_DATA_WALL:
			return MOVEMENT_CRASH;
		case LEVEL_DATA_START:
			if(prev==LEVEL_DATA_FINISH) {
				return MOVEMENT_GOAL;
			} else {
				return MOVEMENT_OK;
			}
		case LEVEL_DATA_FINISH: 
			if(prev==LEVEL_DATA_START) {
				return MOVEMENT_CRASH;
			} else {
				return MOVEMENT_OK;
			}
	}
}
/**
 * 0: left
 * 1: forward
 * 2: right
 */
void Game::get_sensors(sensor_value_t (&values)[3],int th_near, int th_normal) {
	float angle=momentum.angle();
	angle=round(4*angle/PI)*(PI/4); //(angle/(PI/4)), deal only in 45 degrees multiples
	values[0]=sensor_value(angle-PI/2,th_near,th_normal);
	values[1]=sensor_value(angle,th_near,th_normal);
	values[2]=sensor_value(angle+PI/2,th_near,th_normal);
}

sensor_value_t Game::sensor_value(float dir,int th_near,int th_normal) {
	Coordinate near,normal;
	near=Coordinate((int)round(th_near*cos(dir)),(int)round(th_near*sin(dir)));
	normal=Coordinate((int)round(th_normal*cos(dir)),(int)round(th_normal*sin(dir)));
	if(debug) {
		printf("near: (%i,%i): %i, normal: (%i,%i): %i\n",near.x,near.y,_level->get(position+near),normal.x,normal.y,_level->get(position+normal));
	}
	if(evaluate_lvl_data(_lvl_data,_level->get(position+near))==MOVEMENT_CRASH) {
		return SENSOR_NEAR;
	} else if(evaluate_lvl_data(_lvl_data,_level->get(position+normal))==MOVEMENT_CRASH) {
		return SENSOR_NORMAL;
	} else {
		return SENSOR_FAR;
	}
}
