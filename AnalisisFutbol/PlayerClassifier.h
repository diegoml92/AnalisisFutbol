#include "config_file.h"

/* CLASE ENCARGADA DE LA GESTIÓN DE LOS JUGADORES */
class PlayerClassifier {
private:

	/* CALCULA EL HISTOGRAMA DEL JUGADOR */
	static vector<Mat> PlayerClassifier::calculateHistogram(Mat frame, Mat filter, vector<Rect> rects, Point pos);

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

