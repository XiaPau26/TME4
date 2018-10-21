/*
 * ConcretJob.h
 *
 *  Created on: 19 oct. 2018
 *      Author: 3521467
 */

#ifndef CONCRETJOB_H_
#define CONCRETJOB_H_

#include "Job.h"
#include <iostream>
#include <thread>
#include <chrono>



class ConcretJob:public Job {
	int* result;
	int arg;
public:
	ConcretJob(int a):result(nullptr), arg(a){}

	void run(Barrier* b){
		std::cout << "début sur args=" << arg << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		*result = arg%256;
		std::cout << "fini sur args=" << arg << " res vaut " << *result << std::endl;
		b->done();
	}

	virtual ~ConcretJob();
};

#endif /* CONCRETJOB_H_ */
