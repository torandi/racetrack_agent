#include "level.h"
#include "coordinate.h"
#include "game.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include <queue>
#include <limits.h>

const char wall_color[]=WALL_COLOR;
const char road_color[]=ROAD_COLOR;
const char start_color[]=START_COLOR;
const char finish_color[]=FINISH_COLOR;
const char start_point_color[]=START_POINT_COLOR;

int Level::load(std::string fileName) {
	

	FILE *file = fopen(fileName.c_str(), "rb");
	
	if(file == 0)
		return 0;
	
	
	fread(_color_data, 1, WIDTH * HEIGHT * 3, file);
	
	fclose(file);

	//Parse data:
	for(int y=0;y<height;++y) {
		for(int x=0;x<width;++x) {
			level_data_t t;
			int pos=(y*square_size*WIDTH+x*square_size)*3;
			char colors[3];
			colors[0]=_color_data[pos];
			colors[1]=_color_data[pos+1];
			colors[2]=_color_data[pos+2];

			if(compare_colors(colors,wall_color)) {
				t=LEVEL_DATA_WALL;
			} else if(compare_colors(colors,road_color)) {
				t=LEVEL_DATA_ROAD;
			} else if(compare_colors(colors,start_color)) {
				t=LEVEL_DATA_START;
			} else if(compare_colors(colors,finish_color)) {
				t=LEVEL_DATA_FINISH;
			} else if(compare_colors(colors,start_point_color)) {
				t=LEVEL_DATA_START;
				_start=Coordinate(x,y);
				printf("Found start: (%i,%i)\n",x,y);
			} else {
				printf("Unknown color: %i,%i,%i @ (%i,%i)\n",colors[0],colors[1],colors[2],x,y); 
				t=LEVEL_DATA_ROAD; //Better road than wall!
			}
				
			_map_data[x+y*(WIDTH/square_size)]=t;
		}
	}

	//Second pass to find the start point
	if(_start.x==0 && _start.y==0) {
		for(int y=0;y<HEIGHT;++y) {
			for(int x=0;x<WIDTH;++x) {
				int pos=(y*WIDTH+x)*3;
				char colors[3];
				colors[0]=_color_data[pos];
				colors[1]=_color_data[pos+1];
				colors[2]=_color_data[pos+2];
				if(compare_colors(colors,start_point_color)) {
					_start=Coordinate(x/square_size,y/square_size);
					printf("Found start point on second iteration (%i,%i)\n",_start.x,_start.y);
					if(get(_start)!=LEVEL_DATA_START) {
						//Ehm, should be on a start field
						for(int y2=0;y2<3;++y2) {
							for(int x2=-1;x2<2;++x2) {
								Coordinate a=Coordinate(x2,y2);
								//We do this weird thing with x,y since we prefere to find a start point
								//to the direct left or right
								if(a.y>1) a.y=-1; 
								if(get(_start+a)==LEVEL_DATA_START) {
									_start+=a;
									goto second_pass_done;
								}
							}
						}
						printf("Warning! Start point is not on a start field.\n");
					} else {
						goto second_pass_done;
					}
				}
			}
		}
	}
second_pass_done:
    // allocate a texture name
    glGenTextures(1, &_texture);
    
    // select our current texture
    glBindTexture(GL_TEXTURE_2D, _texture);
    
    // select modulate to mix texture with color for shading
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_DECAL);
    
    // when texture area is small, bilinear filter the closest mipmap
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    // when texture area is large, bilinear filter the first mipmap
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // texture should tile
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    // build our texture mipmaps
    gluBuild2DMipmaps(GL_TEXTURE_2D, 4, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, _color_data);
    
    int width = WIDTH / square_size;
    int height = HEIGHT / square_size;
    
    char visitedMatrix[width][height];

    // Reset the bfs matrix.
    bfsMatrix = new int*[width];
    for(int x = 0; x < width; ++x) {
        bfsMatrix[x] = new int[height];
        for(int y = 0; y < height; ++y)
            bfsMatrix[x][y] = INT_MAX/2;
    }

    // Generate the bfs matrix.
    for(int x = 0; x < width; ++x) {
        for(int y = 0; y < height; ++y) {
            std::queue<BFSItem> bfsQueue;

            memset(visitedMatrix, 0, sizeof(visitedMatrix));
            
            bfsQueue.push(BFSItem(0, Coordinate(x, y), get(Coordinate(x, y))));

            // Mark as visited;
            visitedMatrix[x][y] = 1;
            
            int it = 0;
            
            while(!bfsQueue.empty()) {
                it++;
                BFSItem item = bfsQueue.front();
                bfsQueue.pop();

                level_data_t lastData = item.fromData;
                level_data_t currentData = get(item.coordinate);

                movement_result_t m = Game::evaluate_lvl_data(lastData, currentData);

                if(m == MOVEMENT_OK) {
                    // Add children.
                    for(int xc = item.coordinate.x - 1; xc <= item.coordinate.x + 1; ++xc) {
                        for(int yc = item.coordinate.y - 1; yc <= item.coordinate.y + 1; ++yc) {
                            if(xc >= 0 && xc < width && yc >= 0 && yc < height) {
                                movement_result_t nm = Game::evaluate_lvl_data(currentData, get(Coordinate(xc, yc)));

                                // Don't bother visiting child if it results in crash. Only if it is unvisited or leads to a goal.
                                if(nm != MOVEMENT_CRASH && (!visitedMatrix[xc][yc] || nm == MOVEMENT_GOAL)) {
                                    bfsQueue.push(BFSItem(item.steps + 1, Coordinate(xc, yc), currentData));
                                
                                    // Mark as visited;
                                    visitedMatrix[xc][yc] = 1;
                                }
                            }
                        }
                    }
                } else if(m == MOVEMENT_GOAL) {
                    bfsMatrix[x][y] = item.steps;
                    break;
                }
            }
        }
    }
    
    // Debug print bfs matrix.
    /*for(int y = 0; y < height; ++y) {
        for(int x = 0; x < width; ++x) {
            printf("%2.i ", bfsMatrix[x][y]);
        }
        printf("\n");

    }*/

	return 1;
}

bool Level::compare_colors(const char c1[3],const char c2[3]) {
	return (	c1[0]==c2[0] &&
				c1[1]==c2[1] &&
				c1[2]==c2[2] );
}

Level::Level(std::string file,int square_size_) {
	square_size=square_size_;

	width=WIDTH/square_size;
	height=HEIGHT/square_size;
	_map_data=(level_data_t*) malloc( height * width * sizeof(level_data_t));
	_color_data = (char *)malloc(HEIGHT * WIDTH * 3);
	load(file);
}

Level::~Level() {
	free(_map_data);
	free(_color_data);
}

Coordinate Level::startPosition() const {
	return _start;
}

level_data_t Level::get(const Coordinate &c) const {
	int pos=c.x+c.y*width;
	if(pos>0 && pos<width*height) {
		return _map_data[pos];
	} else {
		return LEVEL_DATA_WALL;
	}
}

/**
 * Oh god!
 */
void Level::draw() {
	for(int y=0;y<height;++y) {
		for(int x=0;x<width;++x) {
			if(Coordinate(x,y)!=_start) {
				switch(get(Coordinate(x,y))) {
					case LEVEL_DATA_WALL:
						printf("X");
						break;
					case LEVEL_DATA_ROAD:
						printf(".");
						break;
					case LEVEL_DATA_START:
						printf("s");
						break;
					case LEVEL_DATA_FINISH:
						printf("f");
						break;
				}
			} else {
				printf("*");
			}
		}
		printf("\n");
	}
}

int Level::goalDistance(const Coordinate &c) const {
	 if(c.x<width && c.y<height && c.x >= 0 && c.y >= 0) {
		 return bfsMatrix[c.x][c.y];
	 } else {
		return INT_MAX/2;
	 }
}
