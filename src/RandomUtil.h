

#ifndef RANDOM_UTIL_H
#define RANDOM_UTIL_H

#include <omnetpp.h>
#include <string>

class RandomUtil
{




protected:

public:

	cOutVector deviationVector;

	static double genInterval(double mean, double shape, std::string distribution);

};

#endif
