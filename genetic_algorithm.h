#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H
#include "decision_module.h"
#include "level.h"
#include "genome.h"
#include "config.h"
#include <utility>
#include <vector>

#define GENERATION_SIZE 110
#define TOP_COUNT 10
#define MUTATION_PROBABLITY 0.1f

#define MAX_MOVES 200

	class GeneticAlgorithm : public DecisionModule {
			Level * _level;

			std::vector<Genome *> _individuals;

			// Theese are the top individuals  that survives in a generation.
			std::vector<Genome *> _topIndividuals;
	  

			// The best individual so far
			Genome * _bestIndividual;

			bool _hasReachedGoal;
	
			void run_generation();

			void run_slow(Genome * ind);

			float fitness_function(Game * game);

			Config _config;


		public:
			GeneticAlgorithm(Config config);
			~GeneticAlgorithm();
			void run();
    };
#endif
