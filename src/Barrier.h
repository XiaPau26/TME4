/*
 * Barrier.h
 *
 *  Created on: 21 oct. 2018
 *      Author: paulinexia
 */

#ifndef BARRIER_H_
#define BARRIER_H_

#include <mutex>

class Barrier {
	mutable std::mutex m;
	int cpt;
	int n;
	std::condition_variable cv;
public:
	Barrier(int nb):cpt(0),n(nb){}

	void done(){
		cpt++;
		if(cpt == n){
			cv.notify_all();
			cpt = 0;
		}
	}

	void waitFor(){
		std::unique_lock<std::mutex> ul(m);
		cv.wait(ul);
	}

	virtual ~Barrier();
};

#endif /* BARRIER_H_ */
