#pragma once

namespace math
{

double clamp(double val, double min, double max);

// Factor should be between 0.0 and 1.0
// Factor=0.0 => return a
double lerp(double a, double b, double factor);

}
