#include "Geometry.h"
#include <sstream>

string Projection::to_string() const
{
	stringstream ss;
	ss  << "segment number: " << number << ", parameter: " << parameter 
		<< ", coordinates x=" << x << ", y=" << y << ", z=" << z;
	return ss.str();
}

Polygon::Polygon(vector<Point>&& data):
	m_points(data) 
{
}

// Calculates projection as minimal distance from point to each of segments in polygon
// To do this we solve simple vector equation to get perpendicular to segment line, 
// then we can compute parameter which shows us is destination point lies on segment or not
// If it is not, then we just get the minimal distance as the distance to one end of the segment
vector<Projection> Polygon::calculateProjection(const Point& pt) const
{
	vector<Projection> result;
	double currentMinDistance = numeric_limits<double>::max();
	auto addResult = [this, &result, &currentMinDistance](Point p, size_t index, double parameter, double distance, bool isSecondPoint = false)
	{
		//we add second point of segment only if its last and polygon is not closed
		if (isSecondPoint && (index != m_points.size() - 1 || m_points[0].distance(m_points[m_points.size() - 1]) < precision))
			return;

		if (abs(currentMinDistance - distance) < precision) {
			// we consider that distance as equal to current minimum
			result.push_back({ p.x, p.y, p.z, index, parameter });
		}
		else if (distance < currentMinDistance) {
			currentMinDistance = distance;
			result.clear();
			result.push_back({ p.x, p.y, p.z, index, parameter });
		}
	};

	for (size_t i = 0; i < m_points.size() - 1; i++) {
		auto point0 = m_points[i];
		auto point1 = m_points[i + 1];
		if (point0.distance(point1) < precision)
			throw runtime_error("segment length cannot be zero");

		// line vector equation r = r0 + t * a (line through points p0 and p1)
		Vector r0(point0);
		Vector a(point0, point1);

		// r1 - radius-vector of perpendicular base from pt to line
		Vector n(pt, point0);
		Vector r1(a.scale(n.scalarProduct(a) / a.module2()), r0);

		// calculating parameter using åthe largest coordinate of vector a
		Point temp(abs(a.x), abs(a.y), abs(a.z));
		double parameter = (temp.x >= temp.y && temp.x >= temp.z) ? (r1.x - r0.x) / a.x : ((temp.y >= temp.z) ? (r1.y - r0.y) / a.y : (r1.z - r0.z) / a.z);

		// if projection lies between point0 and point1 then min distance is length of perpendicular, else min distance is distance to either of points: point0 or point1
		double distance;
		if (parameter >= 0 && parameter <= 1) {
			addResult(r1, i, parameter, r1.distance(pt));
		}
		else {
			double dist0 = point0.distance(pt);
			double dist1 = point1.distance(pt);
			if (abs(dist0 - dist1) < precision) {
				addResult(point0, i, 0, dist0);
				addResult(point1, i, 1, dist1, true);
			}
			else {
				distance = min(dist0, dist1);
				r1 = dist0 < dist1 ? point0 : point1;
				parameter = dist0 < dist1 ? 0 : 1;
				addResult(r1, i, parameter, distance, dist1 < dist0);
			}
		}
	}
	return result;
}
