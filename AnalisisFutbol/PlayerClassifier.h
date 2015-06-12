#include "config_file.h"

/* CLASE ENCARGADA DE LA GESTI�N DE LOS JUGADORES */
class PlayerClassifier {
private:

	/* CALCULA EL HISTOGRAMA DEL JUGADOR */
	static vector<Mat> PlayerClassifier::calculateHistogram(Mat frame, Mat filter, vector<Rect> rects, Point pos);

public:

	/* A�ADE UN JUGADOR */
	static void addPlayer(Mat partido, Mat filtro, Point player);

	/* DETERMINA SI CUMPLE EL TAMA�O PROPIO DE UN JUGADOR */
	static bool isPlayerSize(Rect player);

	/* DESCARTA FALSOS POSITIVOS (L�NEAS, ETC) EN FUNCI�N DE EL N�MERO DE P�XELES BLANCOS */
	static bool PlayerClassifier::canBePlayer(Mat roi);

	/* DESCARTA FALSOS POSITIVOS (L�NEAS, ETC) EN FUNCI�N DE EL N�MERO DE P�XELES BLANCOS */
	static bool PlayerClassifier::canBePlayer2(Mat roi);

};

