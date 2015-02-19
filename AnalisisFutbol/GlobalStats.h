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
	static vector<Point> detectedPlayers;
	static vector<int> playersToDelete;

	/*
	* Vector que almacenar� los elementos detectados
	* en cada iteraci�n por cada una de las c�maras
	*/	
	static vector<Point2f> locations [];

	/* VAC�A LOS VECTORES DE POSICIONES */
	static void clearLocations();

	/* A�ADE LAS ESTAD�STICAS ACTUALES */
	static void addStats();

	/* A�ADE LAS ESTAD�STICAS ACTUALES */
	static bool alreadyDetected(Point p);

};

