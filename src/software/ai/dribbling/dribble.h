#pragma once

#include <array>
#include <cstdlib>
#include <iostream>

#include "software/geom/point.h"
#include "software/time/timestamp.h"

/**
 * This class represents a Dribble
 */
class Dribble
{
   public:
    Dribble() = delete;

    /**
     * Create a pass with given parameters
     *
     * @param start_point The point where the dribble starts
     * @param end_point The point where the dribble ends
     */
    Dribble(Point start_point, Point end_point);

    /**
     * Gets the value of the start point
     *
     * @return The value of the starting point
     */
    Point startPoint() const;

    /**
     * Gets the value of the end point
     *
     * @return The value of the end point
     */
    Point endPoint() const;


    /**
     * Implement the "<<" operator for printing
     *
     * @param output_stream The stream to print to
     * @param dribbl The double to print
     * @return The output stream with the string representation of the class appended
     */
    friend std::ostream& operator<<(std::ostream& output_stream, const Dribble& dribble);

    /**
     * Compares Dribbles for equality. Dribbles are considered
     * equal if all their member variables are equal.
     *
     * @param other the Dribble to compare with for equality
     *
     * @return true if the Dribbles are equal and false otherwise
     */
    bool operator==(const Dribble& other) const;

   private:
    // The location of the passer
    Point start_point;

    // The location of the receiver
    Point end_point;
};
