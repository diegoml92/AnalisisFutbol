#include "config_file.h"
#include "Team.h"

/*
   CONTIENE LAS INSTANCIAS GLOBALES DE LOS JUGADORES
   PARA EL CORRECTO MANEJO DE LAS ESTADISITCAS EN EL
   RESTO DE CLASES
*/
class GlobalStats {
public:

	// Variables globales
	static Team teams[N_TEAMS];
	static vector<Point> detectedPlayers;
	static vector<int> playersToDelete;

	static vector<Scalar> colors;

	/*
	* Vector que almacenará los elementos detectados
	* en cada iteración por cada una de las cámaras
	*/	
	static vector<Rect> locations [];

	/* VACÍA LOS VECTORES DE POSICIONES */
	static void clearLocations();

	/* AÑADE LAS ESTADÍSTICAS ACTUALES */
	static void addStats();

	/* AÑADE LAS ESTADÍSTICAS ACTUALES */
	static bool alreadyDetected(Point p);

	/* CALCULA EL CENTRO DE UN RECTÁNGULO */
	static Point getCenter(Rect r);
};

