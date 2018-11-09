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

using namespace std;
using namespace pr;

// NB : en francais pour le cours, preferez coder en english toujours.
// pas d'accents pour eviter les soucis d'encodage

int main() {
	/*int N = 5;
	 pr::Pool p(6);
	 p.start(2);

	 Barrier* b = new Barrier(N);

	 std::vector<int> res;
	 res.reserve(N);

	 for(int i = 0; i < 5; i++){
	 ConcretJob* j = new ConcretJob(i, &res[i], std::ref(b));
	 p.submit(j);
	 }

	 b->waitFor();
	 p.stop();

	 delete b;*/

	// on pose une graine basee sur la date
	default_random_engine re(
			std::chrono::system_clock::now().time_since_epoch().count());
	// definir la Scene : resolution de l'image
	Scene scene(1000, 1000);
	// Nombre de spheres (rend le probleme plus dur)
	const int NBSPHERES = 250;

	std::chrono::steady_clock::time_point start =
			std::chrono::steady_clock::now();

	// on remplit la scene de spheres colorees de taille position et couleur aleatoire
	uniform_int_distribution<int> distrib(0, 200);
	uniform_real_distribution<double> distribd(-200, 200);
	for (int i = 0; i < NBSPHERES; i++) {
		// position autour de l'angle de la camera
		// rayon entre 3 et 33, couleur aleatoire
		// distrib(re) rend un entier aleatoire extrait de re
		scene.add(
				Sphere(
						{ 50 + distribd(re), 50 + distribd(re), 120
								+ distribd(re) },
						double(distrib(re) % 30) + 3.0, Color::random()));
	}
	// quelques spheres de plus pour ajouter du gout a la scene
	scene.add(Sphere( { 50, 50, 40 }, 15.0, Color::red));
	scene.add(Sphere( { 100, 20, 50 }, 55.0, Color::blue));

	// les points de l'ecran
	const Scene::screen_t & screen = scene.getScreenPoints();
	// lumieres 
	vector<Vec3D> lights;
	lights.reserve(3);
	lights.emplace_back(Vec3D(50, 50, -50));
	lights.emplace_back(Vec3D(50, 50, 120));
	lights.emplace_back(Vec3D(200, 0, 120));

	// Les couleurs des pixels
	int taille = scene.getWidth() * scene.getHeight();
	Color * pixels = new Color[taille];

	pr::Pool p(taille);
	Barrier* b = new Barrier(taille);

	p.start(8);
	// pour chaque pixel, calculer sa couleur
	for (int x = 0; x < scene.getWidth(); x++) {
		for (int y = 0; y < scene.getHeight(); y++) {

			// le point de l'ecran par lequel passe ce rayon
			auto & screenPoint = screen[y][x];
			// le point de l'image (pixel) dont on vient de calculer la couleur
			Color & pixel = pixels[y * scene.getHeight() + x];

			ConcretJob* j = new ConcretJob(screenPoint, pixel, std::ref(scene), std::ref(lights),
					b);
			p.submit(j);
		}
	}

	b->waitFor();
	p.endOfproject(8);
	std::chrono::steady_clock::time_point end =
			std::chrono::steady_clock::now();
	std::cout << "Total time "
			<< std::chrono::duration_cast<std::chrono::milliseconds>(
					end - start).count() << "ms.\n";

	// ppm est un format ultra basique
	ofstream img("toto.ppm");
	// P3 signifie : les pixels un par un en ascii
	img << "P3" << endl; // ascii format, colors
	// largeur hauteur valeur max d'une couleur (=255 un char)
	img << scene.getWidth() << "\n" << scene.getHeight() << "\n" << "255"
			<< endl;
	// tous les pixels au format RGB
	for (int y = 0; y < scene.getHeight(); y++) {
		for (int x = 0; x < scene.getWidth(); x++) {
			Color & pixel = pixels[x * scene.getHeight() + y];
			img << pixel << '\n';
		}
	}

	// oui ca fait un gros fichier :D
	img.close();

	delete b;

	return 0;
}
