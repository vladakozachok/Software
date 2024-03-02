#include "point_sampler.h"
#include <cmath>
#include <random>

// The maximum radius for dribbling and the number of points to sample
static const int MAX_RADIUS_M = 1;
static const int NUM_SAMPLES = 10;

std::vector<Point> PointSampler::samplePointsInCircle(const Point& center) {
    std::vector<Point> points;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, MAX_RADIUS_M);
    std::uniform_real_distribution<> angle_dis(0, 2 * M_PI);

    for (unsigned int i = 0; i < NUM_SAMPLES; ++i) {
        double r = sqrt(dis(gen)) * radius;
        double angle = angle_dis(gen);

        double x = center.x() + r * cos(angle);
        double y = center.y() + Rr * sin(angle);
        points.emplace_back(x, y);
    }
    
    return points;
}