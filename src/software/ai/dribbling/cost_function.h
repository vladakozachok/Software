#pragma once

#include <functional>

#include "proto/message_translation/tbots_protobuf.h"
#include "proto/parameters.pb.h"
#include "software/ai/passing/pass.h"
#include "software/math/math_functions.h"
#include "software/util/make_enum/make_enum.h"
#include "software/world/field.h"
#include "software/world/team.h"
#include "software/world/world.h"

/**
 * CostFunction class for evaluating the attractiveness of positions on the soccer field
 * for actions such as passing, dribbling, and shooting in a simulated environment.
 */
class CostFunction {
public:
    /**
     * Constructs a new CostFunction object with passing configuration.
     * 
     * @param config The passing configuration parameters used for scoring calculations.
     */
    explicit CostFunction(const TbotsProto::PassingConfig& config);

    /**
     * Calculates the overall score for a position on the field based on various factors
     * such as distance to goal, enemy proximity, dribble distance, etc.
     * 
     * @param position The target position to evaluate.
     * @param ballPosition The current position of the ball.
     * @param world The current state of the world, including field, teams, etc.
     * @return A score between 0 and 1, where higher scores indicate more attractive positions.
     */
    double calculatePositionScore(const Point& position, const Point& ballPosition, const World& world);

private:
    TbotsProto::PassingConfig passingConfig; // Configuration parameters for scoring

    double computeDistanceToGoalScore(const Point& position, const Field& field);
    double distancePointToLineSegment(const Point& p, const Point& lineStart, const Point& lineEnd);
    double computeEnemyPathProximityScore(const Point& start, const Point& end, const Team& enemyTeam);
    double computeDribbleDistanceScore(const Point& ballPosition, const Point& targetPosition);
    double computeTeammateProximityScore(const Point& targetPosition, const Team& friendlyTeam);
    double computePassOrShotOpportunityScore(const Point& position, const Field& field, const Team& enemyTeam);
};

#endif // COST_FUNCTION_H


