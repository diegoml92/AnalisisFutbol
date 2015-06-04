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
	* Vector que almacenará los elementos detectados
	* en cada iteración por cada una de las cámaras
	*/	
	static vector<Rect> locations [];

	/* VACÍA LOS VECTORES DE POSICIONES */
	static void clearLocations();

	/* AÑADE LAS ESTADÍSTICAS ACTUALES */
	static bool alreadyDetected(Point p);

	/* CALCULA EL CENTRO DE UN RECTÁNGULO */
	static Point getCenter(Rect r);

	/* CALCULA EL RECTÁNGULO A PARTIR DEL CENTRO */
	static Rect getPlayerRect(Point pos);

	/* INDICA SE ESTÁN TODOS LOS JUGADORES DETECTADOS */
	static bool allPlayersDetected();

	static int totalPlayers(); 
};

