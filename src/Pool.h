/*
 * Pool.h
 *
 *  Created on: 19 oct. 2018
 *      Author: 3521467
 */

#ifndef POOL_H_
#define POOL_H_

#include "Job.h"
#include "Queue.h"
#include <thread>
#include <vector>

class Pool {
	Queue<Job> qj;
	std::vector<std::thread> vt;
public:
	Pool(int taille){
		qj = new Queue<Job>(taille);
	}

	void workerThread(){
		while(true){
			Job* j = qj.pop();

		}
	}

	void start(int NBTHREAD){
		for(int i = 0; i < NBTHREAD; i++){
			vt = std::thread()
		}
	}

	virtual ~Pool();
};

#endif /* POOL_H_ */
