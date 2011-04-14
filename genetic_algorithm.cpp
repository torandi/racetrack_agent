#include "genetic_algorithm.h"
#include "level.h"
#include "game.h"
#include "genome.h"
#include "graphics.h"
#include "config.h"

#include <cstdio>
#include <utility>
#include <algorithm>
#include <limits.h>

bool genomePointerComp(Genome *g1, Genome *g2) {
    return g1->points < g2->points;
}

GeneticAlgorithm::GeneticAlgorithm(Config config) {
	_level=new Level(config["track"],config.get_int("square_size"));
	_config=config;

	srand(time(0));
    
    // Because each survival mates wich each other survival..

    _individuals = std::vector<Genome *>(GENERATION_SIZE);
    _topIndividuals = std::vector<Genome *>(TOP_COUNT);

    
    printf("ic:%lu\n", _individuals.size());
    
    for(int i = 0; i < GENERATION_SIZE; ++i) {
        _individuals[i] = new Genome();
    }
    
}

GeneticAlgorithm::~GeneticAlgorithm() {
	//delete _level;
	//delete[] _individuals;
}

void GeneticAlgorithm::run() {
	_level->draw();
	_hasReachedGoal=false;

	_bestIndividual=new Genome();
	_bestIndividual->points=INT_MAX;
	//init
	for(int i=0;i<GENERATION_SIZE;++i) {
		_individuals[i]->randomize();
	}

    for(int i = 0; i < _config.get_int("generations"); ++i) {
        run_generation();
    }
	 //Run the best generation
	 printf("The best generation, points: %i\n",_bestIndividual->points);
	 run_slow(_bestIndividual);

}

void GeneticAlgorithm::run_generation() {
   printf("New generation!\n");
	for(int i=0;i<GENERATION_SIZE;++i) {
		Genome * ind=_individuals[i];
		Game * game=new Game(_level);
		sensor_value_t sensors[3];
		movement_result_t last_result=MOVEMENT_OK;

		int r_moves=MAX_MOVES;
		while(last_result==MOVEMENT_OK && r_moves>0) {
			game->get_sensors(sensors,_config.get_int("sensor_near"),_config.get_int("sensor_normal"));
			last_result = game->move(ind->get(game->momentum,sensors));
			--r_moves;
		}

		_individuals[i]->path=game->_positions;
		
		//If we reach goal show it explicitly:
		if(last_result==MOVEMENT_GOAL) {
			_hasReachedGoal=true;
		}
		
        // Evaluate the genome 
        _individuals[i]->points = fitness_function(game);
        
		delete game;
	}
    
    // Sort all the individuals.
    std::sort(_individuals.begin(), _individuals.end(), genomePointerComp);

    // Copy the ones that survive.
    printf("Top individuals (points):");
    for(int i = 0; i < TOP_COUNT; ++i) {
        //*_topIndividuals[i] = *_individuals[i];
        printf(" %i", _individuals[i]->points);
			
		  if(*_individuals[i] < *_bestIndividual) {
				*_bestIndividual=*_individuals[i];	
				printf("New best individual: %i points\n",_bestIndividual->points);
		  }

		  //Draw the top individuals
		  Graphics::drawPath(_individuals[i]->path,_level);

    }
    printf("\n");

    // The fun part..
    int childCount = 10; //Individual 0-9 is the top individuals
    for(int p1 = 0; p1 < TOP_COUNT; ++p1) {
        for(int p2 = 0; p2 < TOP_COUNT; ++p2) {
            Genome::crossover(_individuals[childCount++], _individuals[p1], _individuals[p2], MUTATION_PROBABLITY);
        }
    }
}

float GeneticAlgorithm::fitness_function(Game * game) {
  Coordinate c = game->_positions.size() > 1 ? game->_positions[game->_positions.size() - 2] : game->_level->startPosition();
  int goalDistance=_level->goalDistance(c); 
  int steps=game->_positions.size();
	
  //float weight=_hasReachedGoal ? 1.5 : 2.0;
  float weight=_config.get_float("goal_distance_weight");

  return steps+weight*goalDistance+(game->last_movement_result!=MOVEMENT_GOAL?_config.get_float("goal_bonus"):0); 
  //Goal bonus is applied in inverse, that is, it is added if it has not reached the goal
}

/**
 * Runs a game slow and explicit
 */
void GeneticAlgorithm::run_slow(Genome * ind) {
		Game * game=new Game(_level);
		game->debug=false;
		sensor_value_t sensors[3];
		movement_result_t last_result=MOVEMENT_OK;

		while(last_result==MOVEMENT_OK) {
			game->get_sensors(sensors,_config.get_int("sensor_near"),_config.get_int("sensor_normal"));
			printf("Sensor value: %i, %i, %i\n",sensors[0],sensors[1],sensors[2]);
			printf("Momentum: (%i,%i)\n",game->momentum.x,game->momentum.y);
			int m=ind->get(game->momentum,sensors);
			printf("State id: %i, Decision: %i\n",ind->state(game->momentum,sensors),m);
			last_result = game->move(m);
			printf("---------\n");
			Graphics::drawGame(game);
			usleep(500000);
		}
		printf("\nMoves: %i\n",game->_positions.size());
		delete game;
}

