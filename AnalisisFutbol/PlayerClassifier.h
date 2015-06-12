#include "config_file.h"

/* CLASE ENCARGADA DE LA CLASIFICACIÓN DE LOS JUGADORES EN DIFERENTES EQUIPOS */
class PlayerClassifier {
private:

	/*	
	*	CLASIFICA LOS ELEMENTOS DETECTADOS:
	*	Compara el histograma de los jugadores y los clasifica por equipos
	*/
	static vector<Mat> PlayerClassifier::comparePlayer(Mat frame, Mat filter, vector<Rect> rects, Point pos);

public:

	/* AÑADE UN JUGADOR */
	static void addPlayer(Mat partido, Mat filtro, Point player);

	/* DETERMINA SI CUMPLE EL TAMAÑO PROPIO DE UN JUGADOR */
	static bool isPlayerSize(Rect player);

	/* DESCARTA FALSOS POSITIVOS (LÍNEAS, ETC) EN FUNCIÓN DE EL NÚMERO DE PÍXELES BLANCOS */
	static bool PlayerClassifier::canBePlayer(Mat roi);

	/* DESCARTA FALSOS POSITIVOS (LÍNEAS, ETC) EN FUNCIÓN DE EL NÚMERO DE PÍXELES BLANCOS */
	static bool PlayerClassifier::canBePlayer2(Mat roi);

};

