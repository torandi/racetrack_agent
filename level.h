#ifndef LEVEL_H_
#define LEVEL_H_

#include <string>
#include "coordinate.h"
#include <GL/glfw.h>

	#define WALL_COLOR {0x66,0x66,0x66}
	#define ROAD_COLOR {0xFF,0xFF,0xFF}
	#define START_COLOR {0xFF,0,0}
	#define FINISH_COLOR {0,0,0xFF}
	#define START_POINT_COLOR {0xFF,0xFF,0}

	#define WIDTH 800
	#define HEIGHT 600
	
	enum level_data_t {
		LEVEL_DATA_WALL=0,
		LEVEL_DATA_ROAD=1,
		LEVEL_DATA_START=2,
		LEVEL_DATA_FINISH=3
	};

    struct BFSItem {
        int steps;
        Coordinate coordinate;
        level_data_t fromData;
        
        BFSItem(int s, Coordinate c, level_data_t d) {
            steps = s;
            coordinate = c;
            fromData = d;
        }
    };

	class Level {
			level_data_t * _map_data;
			char * _color_data;


			Coordinate _start;
			GLuint _texture;
        
			int **bfsMatrix;


			level_data_t map_color(char color[3]);

			int load(std::string file);

			static bool compare_colors(const char c1[3],const char c2[3]);

		public:
			//The size of the level in squares
			int width, height;
			int square_size;
			Level(std::string file,int square_size_);
			~Level();

			level_data_t get(const Coordinate &c) const;
         int goalDistance(const Coordinate &c) const;

			Coordinate startPosition() const;

			void draw();
        
            friend class Graphics;
	};

#endif
