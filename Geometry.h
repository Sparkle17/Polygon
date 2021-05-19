#pragma once
#include <string>
#include <vector>

using namespace std;

// values less than precision are considered to be zero
const double precision = 1e-12;

struct Point
{
	double x;
	double y;
	double z;

	Point() :
		Point(0, 0, 0)
	{}

	Point(double _x, double _y, double _z) :
		x(_x),
		y(_y),
		z(_z)
	{}

	// returns distance from this point to point p
	double distance(const Point& p) const
	{
		return sqrt((p.x - x) * (p.x - x) + (p.y - y) * (p.y - y) + (p.z - z) * (p.z - z));
	}
};

struct Vector : public Point
{
	Vector(const Point& p) :
		Point(p.x, p.y, p.z)
	{}

	Vector(const Point& p1, const Point& p2) :
		Point(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z)
	{}

	// returns |r^2|
	double module2() const
	{
		return x * x + y * y + z * z;
	}

	// scalar product of this vector and vector v
	double scalarProduct(const Vector& v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	Vector scale(double d) const
	{
		return Vector({ x * d, y * d, z * d });
	}
};

struct Projection : public Point
{
	size_t number;
	double parameter;

	Projection(double _x, double _y, double _z, size_t _number, double _parameter):
		Point(_x, _y, _z),
		number(_number),
		parameter(_parameter)
	{}

	string to_string() const;
};

class Polygon
{
private:
	vector<Point> m_points;

public:
	Polygon(vector<Point>&& data);

	vector<Projection> calculateProjection(const Point& pt) const;
};