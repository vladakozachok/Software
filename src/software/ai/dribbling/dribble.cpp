#include "software/ai/dribbling/dribble.h"

Dribble::Dribble(Point start_point, Point end_point)
    : start_point(start_point)
      receiver_point(receiver_point)
      {}

Point Dribble::startPoint() const
{
    return start_point;
}

Point Dribble::endPoint() const
{
    return end_point;
}

std::ostream& operator<<(std::ostream& output_stream, const Dribble& dribble)
{
    output_stream << "End Point: " << dribble.end_point

    return output_stream;
}

bool Dribble::operator==(const Dribble& other) const
{
    return this->start_point == other.start_point &&
           this->end_point == other.end_point 
}
