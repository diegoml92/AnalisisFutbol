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
	* Vector que almacenar� los elementos detectados
	* en cada iteraci�n por cada una de las c�maras
	*/	
	static vector<Rect> locations [];

	/* VAC�A LOS VECTORES DE POSICIONES */
	static void clearLocations();

	/* A�ADE LAS ESTAD�STICAS ACTUALES */
	static void addStats();

	/* A�ADE LAS ESTAD�STICAS ACTUALES */
	static bool alreadyDetected(Point p);

	/* CALCULA EL CENTRO DE UN RECT�NGULO */
	static Point getCenter(Rect r);
};

