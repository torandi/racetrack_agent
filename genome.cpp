#include "genome.h"
#include "game.h"
#include <cstdlib>
#include <list>
#include <utility>
#include <cstring>

Genome::Genome() {
	decision_map=new char[GENOME_VECTOR_SIZE];
	path=std::vector<Coordinate>();
}

Genome::Genome(const Genome &genome) {
	decision_map=new char[GENOME_VECTOR_SIZE];
   memcpy(decision_map, genome.decision_map, GENOME_VECTOR_SIZE);
	points=genome.points;
	path=genome.path;
}

Genome::~Genome() {
	delete[] decision_map;
}

int Genome::get(const Coordinate &movement, const sensor_value_t sensors[3]) const {
	return decision_map[state(movement,sensors)]%9;
}

int Genome::state(const Coordinate &movement, const sensor_value_t sensors[3]) const {
	return movement.state()+sensor_state(sensors)*(2*MAX_SPEED+1)*(MAX_SPEED+1);	
}

int Genome::sensor_state(const sensor_value_t sensors[3]) const {
	return sensors[0]+sensors[1]*3+sensors[2]*9;
}

void Genome::randomize() {
	for(int i=0;i<GENOME_VECTOR_SIZE;++i) {
		decision_map[i]=rand()%9;
	}
}

Genome &Genome::operator=(const Genome &genome) {
   //decision_map=new char[GENOME_VECTOR_SIZE];
	memcpy(decision_map, genome.decision_map, GENOME_VECTOR_SIZE);
	points=genome.points;
	path=genome.path;
	return *this;
}

bool Genome::operator<(const Genome &genome) const {
    return points < genome.points;
}

int Genome::crossover(Genome *child, Genome *parent1, Genome *parent2, float mutation) {
    std::list<std::pair<Genome*, float> > parents;
    
    // Because lower is better, they get each other's points.
    parents.push_back(std::make_pair(parent1, parent2->points));
    parents.push_back(std::make_pair(parent2, parent1->points));
    
    return crossover(*child, parents, mutation);
}

int Genome::crossover(Genome &child,const std::list< std::pair<Genome*,float> > &parents,float mutation) {
	std::list<std::pair<Genome*,int> > sources;
	float sum=mutation;
	std::list< std::pair<Genome*,float> >::const_iterator it;
	std::list< std::pair<Genome*,int> >::const_iterator it2;

	//First pass
	for(it=parents.begin();it!=parents.end();++it) {
		sum+=it->second;
	}

	int mutation_th=1000*mutation;
	int total=1000-mutation_th;

	//Second pass, build sources list
	int c=mutation_th;
	for(it=parents.begin();it!=parents.end();++it) {
		std::pair<Genome*,int> gp;
		gp.first=it->first;
		c+=round(total*(it->second/sum));
		gp.second=c; //Use this genome if a rand()%1000 < c
		sources.push_back(gp);
	}

	if(c!=1000) {
		//Add that to the last one
		sources.back().second+=(1000-c);
	}

	int num_mutations=0;

	for(int i=0;i<GENOME_VECTOR_SIZE;++i) {
		int n=rand()%1000;
		if(n<mutation_th) {
			child.decision_map[i]=rand()%9;
			++num_mutations;
		} else {
			//Find source to use
			for(it2=sources.begin();it2!=sources.end();++it2) {
				if(n<it2->second) {
					child.decision_map[i]=it2->first->decision_map[i];
					break;
				}
			}
		}
	}

	return num_mutations;
}
