/*
 * Job.h
 *
 *  Created on: 19 oct. 2018
 *      Author: 3521467
 */

#ifndef JOB_H_
#define JOB_H_

#include "Barrier.h"

class Job {
public:
	virtual void run() = 0;
	virtual ~Job(){};
};

#endif /* JOB_H_ */
