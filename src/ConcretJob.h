/*
 * ConcretJob.h
 *
 *  Created on: 19 oct. 2018
 *      Author: 3521467
 */

#ifndef CONCRETJOB_H_
#define CONCRETJOB_H_

#include "Job.h"

class ConcretJob:public Job {
	int* result;
	int arg;
public:
	ConcretJob(){
		result = nullptr;

	}

	void run(){

	}
	virtual ~ConcretJob();
};

#endif /* CONCRETJOB_H_ */
