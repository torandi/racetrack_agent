#include "graphics.h"
#include <cstdlib>
#include <vector>

int Graphics::_width = 0;
int Graphics::_height = 0;
bool Graphics::_newPath = false;
Level * Graphics::_level;
std::vector<Coordinate> Graphics::_path;

void Graphics::init(int width, int height) {
    _height = height;
    _width = width;

    if (glfwInit() != GL_TRUE) {
        glfwTerminate();
        exit(1);
    }

	// 800 x 600, 16 bit color, no depth, alpha or stencil buffers, windowed
	if(glfwOpenWindow(width, height, 8, 8, 8, 8, 24, 0, GLFW_WINDOW) != GL_TRUE) {
        glfwTerminate();
        exit(1);
    }

	glfwSetWindowTitle("Racetrack");
    
	glViewport(0, 0, width, height);
	glClearColor(0, 0, 0, 0);
    
    glPointSize(3.0f);
    
	// set the projection matrix to a normal frustum with a max depth of 50
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    
	glShadeModel(GL_FLAT);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
	gluOrtho2D(0, width, 0, height);
    
	glMatrixMode(GL_MODELVIEW);
	
	//loadTexture("oval.raw");
}

void Graphics::drawPath() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    // Draw track.
    glBindTexture(GL_TEXTURE_2D, _level->_texture);
    glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glNormal3f(0.0, 0.0, 1.0);
    glTexCoord2d(0, 1); glVertex3f(0.0, 0.0, 0.0);
    glTexCoord2d(0, 0); glVertex3f(0.0, _height, 0.0);
    glTexCoord2d(1, 0); glVertex3f(_width, _height, 0.0);
    glTexCoord2d(1, 1); glVertex3f(_width, 0.0, 0.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopAttrib();
    
    // Draw grid.
    glColor4f(0.0, 0.0, 0.0, 0.1);
    glBegin(GL_LINES);
    for(int x = 0; x < _width; x += _level->square_size) {
        glVertex2f(x, 0);
        glVertex2f(x, _height);
    }
    for(int y = 0; y < _height; y += _level->square_size) {
        glVertex2f(0, y);
        glVertex2f(_width, y);
    }
    glEnd();

    glTranslatef(0.0f, _height, 0.0f);
    glScalef(10.0f, -10.0f, 1.0f);

    glColor3f(1.0, 0.0, 1.0);
    glBegin(GL_LINE_STRIP);
    //glVertex2f(_game.position.x, _game.position.y);
    for(int i = 0; i < _path.size(); ++i)
        glVertex2f(_path[i].x, _path[i].y);
    glEnd();

    glBegin(GL_POINTS);
    //glVertex2f(_game.position.x, _game.position.y);
    for(int i = 0; i < _path.size(); ++i)
        glVertex2f(_path[i].x, _path[i].y);
   
		glColor3f(0.0, 0.0, 0.0);
		glVertex2f(_level->startPosition().x,_level->startPosition().y);
		glColor3f(0.0, 1.0, 0.0);
		glVertex2f(_path.back().x,_path.back().y);
	glEnd();

}

void  Graphics::drawPath(const std::vector<Coordinate> &path, Level * level) {
	_path=path;
	_level=level;
	_newPath=true;
}

void Graphics::drawGame(Game * game) {
	_path=game->_positions;
	_level=game->_level;
	_newPath=true;
}

void Graphics::drawLoop() {
    // the time of the previous frame
	double old_time = glfwGetTime();
	// this just loops as long as the program runs
	while(1) {
		// calculate time elapsed, and the amount by which stuff rotates
		double current_time = glfwGetTime();
		/*delta_rotate = (current_time - old_time) * rotations_per_tick * 360;
		old_time = current_time;
		// escape to quit, arrow keys to rotate view
		if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS)
			break;
		if (glfwGetKey(GLFW_KEY_LEFT) == GLFW_PRESS)
			rotate_y += delta_rotate;
		if (glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS)
			rotate_y -= delta_rotate;
		// z axis always rotates
		rotate_z += delta_rotate;*/
		
		// clear the buffer
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// draw the figure
        
		//draw();
        
        if(_newPath) {
            drawPath();
            glfwSwapBuffers();
            drawPath();
            _newPath= false;
        }
        
		// swap back and front buffers
		glfwSwapBuffers();
      glfwSleep(1 / 60.0f);
	}
}
/*
void Graphics::draw() {
	glLoadIdentity();

    // Draw track.
    glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);
    //glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glNormal3f(0.0, 0.0, 1.0);
    glTexCoord2d(1, 1); glVertex3f(0.0, 0.0, 0.0);
    glTexCoord2d(1, 0); glVertex3f(0.0, _height, 0.0);
    glTexCoord2d(0, 0); glVertex3f(_width, _height, 0.0);
    glTexCoord2d(0, 1); glVertex3f(_width, 0.0, 0.0);
    glEnd();
    //glDisable(GL_TEXTURE_2D);
    glPopAttrib();
    
    // Draw grid.
    glColor4f(0.0, 0.0, 0.0, 0.1);
    glBegin(GL_LINES);
    for(int x = 0; x < _width; x += SQUARE_SIZE) {
        glVertex2f(x, 0);
        glVertex2f(x, _height);
    }
    for(int y = 0; y < _height; y += SQUARE_SIZE) {
        glVertex2f(0, y);
        glVertex2f(_width, y);
    }
    glEnd();
    
    // TODO: Draw path.
    
    
    // TODO: Draw car.
    
}*/
