/*
 * Queue.h
 *
 *  Created on: 19 oct. 2018
 *      Author: 3521467
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include <iostream>
#include <mutex>
#include <condition_variable>


template <typename T>
class Queue {
	T** tab;
	const size_t allocsize;
	size_t begin;
	size_t sz;
	mutable std::mutex m;
	std::condition_variable cv;
	bool isBlockable;

	bool full() const{
		return sz == allocsize;
	}

	bool empty() const{
		return sz == 0;
	}

public:
	Queue(size_t maxsize):allocsize(maxsize), begin(0), sz(0), isBlockable(true){
		tab = new T*[maxsize];
		memset(tab, 0, maxsize*sizeof(T*));
	}

	Queue(const Queue& q):allocsize(q.allocsize){
		tab = q.tab;
		begin = q.begin;
		sz = q.sz;
		isBlockable = q.isBlockable;
	}

	size_t size() const {
		std::lock_guard<std::mutex> lg(m);
		return sz;
	}

	T* pop(){
		std::unique_lock<std::mutex> lg(m);
		while(empty()){
			if(!isBlockable)
				return nullptr;
			cv.wait(lg);
		}


		if(full())
			cv.notify_all();
		auto ret = tab[begin];
		tab[begin] = nullptr;
		sz--;
		begin = (begin+1)%allocsize;
		return ret;
	}

	bool push(T* elt){
		std::unique_lock<std::mutex> lg(m);
		while(full()){
			if(!isBlockable)
				return false;
			cv.wait(lg);
		}
		if(empty())
			cv.notify_all();
		tab[(begin + sz) % allocsize] = elt;
		sz++;
		return true;
	}

	void setBlockingPop(bool isBlocking){
		isBlockable = isBlocking;
		if(!isBlockable)
			cv.notify_all();
	}

	virtual ~Queue(){
		for(size_t i = 0; i < sz; i++){
			auto ind = (begin+i) % allocsize;
			delete  tab[ind];
		}

		delete [] tab;
	}


};

#endif /* QUEUE_H_ */
