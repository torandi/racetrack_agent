#ifndef GENOME_H_
#define GENOME_H_
#include "coordinate.h"
#include "game.h"
#include <list>
#include <utility>
#include <vector>

#define GENOME_VECTOR_SIZE 3267

struct Genome {
	char * decision_map;

	Genome();
    Genome(const Genome &genome);
	~Genome();

	/**
	 * Gets the decision for a given movement vector and sensor values
	 */
	int get(const Coordinate &movement, const sensor_value_t sensor[3]) const;

	/**
	 * Returns an int for the given state
	 */
	int state(const Coordinate &movement, const sensor_value_t sensors[3]) const;

	/**
	 * Randomizes the decision map for this genome
	 */
	void randomize();

	/**
	 * Creates a child from the given parents. The new child is written to child
	 * parents: A list containing pairs, each one containing a pointer to the parent,
	 *		and a float specifying the probability that a decision from that parent is inherited.
	 *
	 * mutation: A float specifying the probablity that a mutation occurs
	 */
	static int crossover(Genome &child,const std::list< std::pair<Genome*,float> > &parents,float mutation);
    
    static int crossover(Genome *child, Genome *parent1, Genome *parent2, float mutation);
    
    bool operator<(const Genome &genome) const;
    Genome &operator=(const Genome &genome);
    
    int points;

	 std::vector<Coordinate> path;
private:
	int sensor_state(const sensor_value_t sensors[3]) const;
};
#endif
