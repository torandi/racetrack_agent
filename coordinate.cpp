#include "coordinate.h"
#include "game.h"

#include <cstdlib>
#include <cstdio>
#include <exception>

Coordinate &Coordinate::operator+=(const Coordinate &c) {
	x+=c.x;
	y+=c.y;
	return *this;
}	

Coordinate Coordinate::operator+(const Coordinate &c) const{
	return Coordinate(x+c.x,y+c.y);
}

Coordinate Coordinate::operator-(const Coordinate &c) const{
	return Coordinate(x-c.x,y-c.y);
}

Coordinate &Coordinate::operator-=(const Coordinate &c) {
	x-=c.x;
	y-=c.y;
	return *this;
}

Coordinate Coordinate::operator*(const Coordinate &c) const {
	return Coordinate(x*c.x,y*c.y);	
}

Coordinate Coordinate::operator*(const float &f) const {
	return Coordinate(round(x*f),round(y*f));	
}

bool Coordinate::operator==(const Coordinate &c) const {
	return c.x==x && c.y==y;
}

bool Coordinate::operator!=(const Coordinate &c) const {
	return !(*this==c);
}

float Coordinate::angle() const {
	return atan2((float)y,(float)x);
}

void Coordinate::rotate(float a) {
	float n=norm();
	a+=angle();
	x=round(cos(a)*n);
	y=round(sin(a)*n);
}

float Coordinate::norm() {
	return sqrt(x*x+y*y);
}

int Coordinate::state() const {
	int _x=x;
	int _y=y;

	if(abs(x)>MAX_SPEED || abs(y)>MAX_SPEED )  {
		std::fprintf(stderr,"Called state on Coordinate(%i,%i) with abs(x) or abs(y) > MAX_SPEED\n",x,y);
		throw std::exception();
	}

	if(_y<0) {
		_y=-y;
		_x=-x;
	}
	return _x+_y*(2*MAX_SPEED+1);
}
