#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "game.h"
#include "coordinate.h"
#include <vector>

class Graphics {
public:
    static void init(int width, int height);
    static void drawLoop();

    // Draw a game.
    static void drawGame(Game * game);
    static void drawPath(const std::vector<Coordinate> &path,Level * level);

private:
    static void draw();
    static void drawPath();

    static int _height;
    static int _width;

	 static Level * _level;
	 static std::vector<Coordinate> _path;

    // Do we have a new path to draw?
    static bool _newPath;
};

#endif
