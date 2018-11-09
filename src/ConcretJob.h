/*
 * ConcretJob.h
 *
 *  Created on: 19 oct. 2018
 *      Author: 3521467
 */

#ifndef CONCRETJOB_H_
#define CONCRETJOB_H_

#include "Job.h"
#include "Vec3D.h"
#include "Rayon.h"
#include "Scene.h"
#include "Pool.h"
#include "ConcretJob.h"
#include "Queue.h"
#include "Barrier.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <limits>
#include <random>

class ConcretJob: public Job {
	const pr::Vec3D& screenPoint;
	pr::Color& pixel;
	pr::Scene& scene;
	std::vector<pr::Vec3D>& lights;
	Barrier* barrier;
public:
	ConcretJob(const pr::Vec3D& screen, pr::Color& res, pr::Scene& s,
			std::vector<pr::Vec3D>& l, Barrier* b) :
			screenPoint(screen), pixel(res), scene(s), lights(l), barrier(b) {
	}
	void run() {
		/*std::cout << "début sur args=" << arg << std::endl;
		 std::this_thread::sleep_for(std::chrono::milliseconds(10));
		 *result = arg%256;
		 std::cout << "fini sur args=" << arg << " res vaut " << *result << std::endl;
		 barrier->done();*/
		// le rayon a inspecter
		pr::Rayon ray(scene.getCameraPos(), screenPoint);

		auto minz = std::numeric_limits<float>::max();
		int targetSphere = -1;
		int index = 0;
		for (const auto & obj : scene) {
			// rend la distance de l'objet a la camera
			auto zinter = obj.instersects(ray);
			// si intersection plus proche  ?
			if (zinter < minz) {
				minz = zinter;
				targetSphere = index;
			}
			index++;
		}

		if (targetSphere != -1) {

			const pr::Sphere & obj = *(scene.begin() + targetSphere);
			// pixel prend la couleur de l'objet
			pr::Color finalcolor = obj.getColor();

			// calcul du rayon et de sa normale a la sphere
			// on prend le vecteur de la camera vers le point de l'ecran (dest - origine)
			// on le normalise a la longueur 1, on multiplie par la distance à l'intersection
			pr::Vec3D rayInter = (ray.dest - ray.ori).normalize() * minz;
			// le point d'intersection
			pr::Vec3D intersection = rayInter + scene.getCameraPos();
			// la normale a la sphere au point d'intersection donne l'angle pour la lumiere
			pr::Vec3D normal = obj.getNormale(intersection);
			// le niveau d'eclairage total contribue par les lumieres 0 sombre 1 total lumiere
			double dt = 0;
			// modifier par l'eclairage la couleur
			for (const auto & light : lights) {
				// le vecteur de la lumiere au point d'intersection
				pr::Vec3D tolight = (light - intersection);
				// si on est du bon cote de la sphere, i.e. le rayon n'intersecte pas avant de l'autre cote
				if (obj.instersects(pr::Rayon(light, intersection))
						>= tolight.length() - 0.05) { //  epsilon 0.05 for double issues
					dt += tolight.normalize() & normal; // l'angle (scalaire) donne la puissance de la lumiere reflechie
				}
			}
			// eclairage total
			finalcolor = finalcolor * dt + finalcolor * 0.2; // *0.2 = lumiere speculaire ambiante

			// mettre a jour la couleur du pixel dans l'image finale.
			pixel = finalcolor;
		}

		barrier->done();
	}

	virtual ~ConcretJob() {
	}
	;
};

#endif /* CONCRETJOB_H_ */
