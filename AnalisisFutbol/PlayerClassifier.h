#include "config_file.h"

/* CLASE ENCARGADA DE LA CLASIFICACIÓN DE LOS JUGADORES EN DIFERENTES EQUIPOS */
class PlayerClassifier {

private:

	/*	
	*	CLASIFICA LOS ELEMENTOS DETECTADOS:
	*	Compara el histograma de los jugadores y los clasifica por equipos
	*/
	static void comparePlayer(Mat partido, Mat umbral, vector<Rect> rect, Point pos);

	/* ENCUENTRA EL EQUIPO EN EL QUE ESTÁ UN JUGADOR */
	static void findAndDraw(Rect rect, Mat partido);

public:

	/* AÑADE UN JUGADOR */
	static void addPlayer(Mat partido, Mat filtro, Point player);

	/* DETERMINA SI CUMPLE EL TAMAÑO PROPIO DE UN JUGADOR */
	static bool isPlayerSize(Rect player);

	/* DESCARTA FALSOS POSITIVOS (LÍNEAS, ETC) EN FUNCIÓN DE EL NÚMERO DE PÍXELES BLANCOS */
	static bool PlayerClassifier::canBePlayer(Mat roi);

};

