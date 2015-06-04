#include "config_file.h"
#include "Player.h"

/*
   CONTIENE LAS INSTANCIAS GLOBALES DE LOS JUGADORES
   PARA EL CORRECTO MANEJO DE LAS ESTADISITCAS EN EL
   RESTO DE CLASES
*/
class GlobalStats {
public:

	// Variables globales
	static vector<Player> playerV;
	static vector<Player*> playersToDelete;
	static vector<Scalar> colors;

	/*
	* Vector que almacenar� los elementos detectados
	* en cada iteraci�n por cada una de las c�maras
	*/	
	static vector<Rect> locations [];

	/* VAC�A LOS VECTORES DE POSICIONES */
	static void clearLocations();

	/* A�ADE LAS ESTAD�STICAS ACTUALES */
	static bool alreadyDetected(Point p);

	/* CALCULA EL CENTRO DE UN RECT�NGULO */
	static Point getCenter(Rect r);

	/* CALCULA EL RECT�NGULO A PARTIR DEL CENTRO */
	static Rect getPlayerRect(Point pos);

	/* INDICA SE EST�N TODOS LOS JUGADORES DETECTADOS */
	static bool allPlayersDetected();

	static int totalPlayers(); 
};

