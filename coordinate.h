#ifndef COORDINATE_H_
#define COORDINATE_H_
	#include <cmath>
	#define PI 3.14159265

	struct Coordinate {
		int x;
		int y;
		Coordinate() : x(0), y(0) {};
		Coordinate(int _x,int _y) : x(_x), y(_y) {};

		Coordinate &operator+=(const Coordinate &c);
		Coordinate operator+(const Coordinate &c) const;
		Coordinate operator-(const Coordinate &c) const;
		Coordinate &operator-=(const Coordinate &c);
		Coordinate operator*(const Coordinate &c) const;
		Coordinate operator*(const float &f) const;
		bool operator==(const Coordinate &c) const;
		bool operator!=(const Coordinate &c) const;
		float angle() const;
		int state() const;
		void rotate(float angle);
		float norm();
	};
#endif
