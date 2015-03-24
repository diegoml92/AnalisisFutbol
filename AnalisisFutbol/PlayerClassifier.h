#include "config_file.h"

/* CLASE ENCARGADA DE LA CLASIFICACI�N DE LOS JUGADORES EN DIFERENTES EQUIPOS */
class PlayerClassifier {

private:

	/*	
	*	CLASIFICA LOS ELEMENTOS DETECTADOS:
	*	Compara el histograma de los jugadores y los clasifica por equipos
	*/
	static void comparePlayer(Mat partido, Mat umbral, vector<Rect> rect, Point pos);

	/* ENCUENTRA EL EQUIPO EN EL QUE EST� UN JUGADOR */
	static void findAndDraw(Rect rect, Mat partido);

public:

	/* A�ADE UN JUGADOR */
	static void addPlayer(Mat partido, Mat filtro, Point player);

	/* DETERMINA SI CUMPLE EL TAMA�O PROPIO DE UN JUGADOR */
	static bool isPlayerSize(Rect player);

	/* DESCARTA FALSOS POSITIVOS (L�NEAS, ETC) EN FUNCI�N DE EL N�MERO DE P�XELES BLANCOS */
	static bool PlayerClassifier::canBePlayer(Mat roi);

};

