#include "cost_function.h"
#include <cmath>
#include <algorithm>
#include <limits>

CostFunction::CostFunction(const TbotsProto::PassingConfig& config) : passingConfig(config) {}

double CostFunction::computeDistanceToGoalScore(const Point& position, const Field& field) {
    double maxFieldLength = field.xLength(); // Assume maxFieldLength is derived from the field dimensions
    double distanceToGoal = (field.enemyGoalCenter() - position).length();
    return 1.0 - std::min(distanceToGoal / maxFieldLength, 1.0);
}

// Helper function to calculate the minimum distance from a point to a line segment
double CostFunction::distancePointToLineSegment(const Point& p, const Point& lineStart, const Point& lineEnd) {
    const double lengthSquared = (lineEnd - lineStart).lengthSquared();
    if (lengthSquared == 0.0) return (p - lineStart).length(); // Line start == line end
    
    // Consider the line extending the segment, parameterized as lineStart + t (lineEnd - lineStart).
    // We find projection of point p onto the line. 
    // It falls where t = [(p-lineStart) . (lineEnd-lineStart)] / |lineEnd-lineStart|^2
    const double t = std::max(0.0, std::min(1.0, (p - lineStart).dot(lineEnd - lineStart) / lengthSquared));
    const Point projection = lineStart + (lineEnd - lineStart) * t; // Projection falls on the segment
    return (p - projection).length();
}

double CostFunction::computeEnemyPathProximityScore(const Point& start, const Point& end, const Team& enemyTeam) {
    double importance = passingConfig.enemy_proximity_importance(); // Adjust based on config
    double minDistanceToPath = std::numeric_limits<double>::max();
    for (const Robot& enemy : enemyTeam.getAllRobots()) {
        double distance = distancePointToLineSegment(enemy.position(), start, end);
        minDistanceToPath = std::min(minDistanceToPath, distance);
    }
    // Convert distance to a score, with a simple exponential decay based on importance factor
    return std::exp(-importance * minDistanceToPath);
}

double CostFunction::computeDribbleDistanceScore(const Point& ballPosition, const Point& targetPosition) {
    double maxDribbleDistance = passingConfig.max_dribble_distance(); 
    double dribbleDistance = (ballPosition - targetPosition).length();
    return 1.0 - std::min(dribbleDistance / maxDribbleDistance, 1.0);
}

double CostFunction::computeTeammateProximityScore(const Point& targetPosition, const Team& friendlyTeam) {
    double maxEffectiveProximity = passingConfig.max_effective_proximity(); 
    double closestTeammateDistance = std::numeric_limits<double>::max();
    for (const Robot& teammate : friendlyTeam.getAllRobots()) {
        double distance = (targetPosition - teammate.position()).length();
        closestTeammateDistance = std::min(closestTeammateDistance, distance);
    }
    return std::exp(-closestTeammateDistance / maxEffectiveProximity);
}

double CostFunction::computePassOrShotOpportunityScore(const Point& position, const Field& field, const Team& enemyTeam) {
    auto shotOpportunity = calcBestShotOnGoal(field.enemyGoal(), position, enemyTeam.getAllRobots(), TeamType::ENEMY);
    return shotOpportunity ? 1.0 : 0.0;
}

// Main function to calculate the overall score for a position on the field
double CostFunction::calculatePositionScore(const Point& position, const Point& ballPosition, const World& world) {
    double goalScore = computeDistanceToGoalScore(position, world.field());
    double enemyProximityScore = computeEnemyPathProximityScore(ballPosition, position, world.enemyTeam());
    double dribbleDistanceScore = computeDribbleDistanceScore(ballPosition, position);
    double teammateProximityScore = computeTeammateProximityScore(position, world.friendlyTeam());
    double shotOpportunityScore = computePassOrShotOpportunityScore(position, world.field(), world.enemyTeam());

    // Example weights, these should be tuned based on the game strategy and testing
    double weights[] = {0.15, 0.35, 0.1, 0.25, 0.15}; // Adjust weights as necessary

    // Calculate weighted average of scores
    double totalScore = (goalScore * weights[0]) + 
                        (enemyProximityScore * weights[1]) + 
                        (dribbleDistanceScore * weights[2]) + 
                        (teammateProximityScore * weights[3]) + 
                        (shotOpportunityScore * weights[4]);

    return totalScore;
}

