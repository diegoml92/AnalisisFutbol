#include "config_file.h"
#include "Team.h"
#include "Ball.h"

/*
   CONTIENE LAS INSTANCIAS GLOBALES DE LOS JUGADORES
   PARA EL CORRECTO MANEJO DE LAS ESTADISITCAS EN EL
   RESTO DE CLASES
*/
class GlobalStats {
public:

	// Variables globales
	static Team teams[N_TEAMS];
	static Ball ball;

	/*
	* Vector que almacenará los elementos detectados
	* en cada iteración por cada una de las cámaras
	*/	
	static vector<Point2f> locations [];

	/* VACÍA LOS VECTORES DE POSICIONES */
	static void clearLocations();

};

