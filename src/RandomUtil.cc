

#include "RandomUtil.h"
#include <exception>

#include <iostream>
using namespace std;


double RandomUtil::genInterval(double mean, double shape, std::string distribution)
{
    if (distribution == "pareto")
    {
        // mu = (a * b) / (a - 1)
        // What is b (scale), given mu and shape a
        // b = ((a - 1) * mu) / a

        double scale = ((shape - 1) * mean) / shape;

        double result =  pareto_shifted(shape, scale, 0);

        return result;
    }
    else
    if (distribution == "poisson")
    {
        double result = exponential(mean);

        return result;
    }
    else
    if (distribution == "normal")
    {
        double result = normal(mean, 0);

        return result;
    }

    return 0;
}
