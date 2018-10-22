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

namespace pr{

	void workerThread(Barrier* b, Queue<Job> qj){
		while(true){
			Job* j = qj.pop();
			j->run(b);
		}
	}

	class Pool {
		Queue<Job> qj;
		std::vector<std::thread> vt;
	public:
		Pool(size_t taille):qj(taille){}

		void submit(Job* job){
			// Peut être bloquant
			qj.push(job);
		}

		// Arrêt de tous les threads lancés par la fonction start puis attente de l'arrêt total avec join
		void stop(){
			qj.setBlockingPop(false);
			int taille = vt.size();
			for(int i = 0; i < taille; i++)
				vt[i].join();
		}


		void start(int NBTHREAD, Barrier* b){
			// Mettre en boucle sur la queue (donc traitement des jobs dans la queue
			for(int i = 0; i < NBTHREAD; i++){
				vt.push_back(std::thread(pr::workerThread, b, qj));
			}
		}

		virtual ~Pool();
	};


}

#endif /* POOL_H_ */
