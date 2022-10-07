#pragma once

#include <cmath>
#include <ostream>

constexpr float PI = 3.14159f;
constexpr float DEG_TO_RAD = PI/180.0f;

struct Vector2f {
	float x;
	float y;

	Vector2f() :x{0.0f},y{0.0f} {}
	Vector2f(float xx, float yy) :x{xx}, y{yy} {}

	Vector2f operator+(const Vector2f& rhs) const {return Vector2f(x+rhs.x,y+rhs.y);}
	Vector2f operator-(const Vector2f& rhs) const {return Vector2f(x-rhs.x,y-rhs.y);}
	Vector2f operator*(float scalar) const        {return Vector2f(x*scalar,y*scalar);}
	Vector2f& operator+=(const Vector2f& rhs)     {x+=rhs.x; y+=rhs.y; return *this;}
	Vector2f& operator-=(const Vector2f& rhs)     {x-=rhs.x; y-=rhs.y; return *this;}
	Vector2f& operator*=(float scalar)		      {x*=scalar; y*=scalar; return *this;}
	
	Vector2f normalize() const {float r = 1.0f/mag(); return Vector2f(x*r,y*r);}
	float mag() const {return sqrt(x*x+y*y);}
	float dot(const Vector2f& rhs) const {return x*rhs.x + y*rhs.y;}
	friend std::ostream& operator<<(std::ostream& str, const Vector2f& vec)
	{
		str << vec.x << " " << vec.y;
		return str;
	}
	
	bool operator==(const Vector2f& rhs) {return (this->x==rhs.x && this->y==rhs.y);}
	bool operator!=(const Vector2f& rhs) {return !(*this==rhs);}
};
