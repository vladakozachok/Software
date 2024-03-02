#include <vector>
#include "software/geom/point.h"


class PointSampler {
public:
    /**
     * Samples points in a circle around a given center.
     * 
     * @param center The center of the circle (beginning dribble position).
     * @return A vector of sampled points.
     */
    static std::vector<Point> samplePointsInCircle(const Point& center);
};