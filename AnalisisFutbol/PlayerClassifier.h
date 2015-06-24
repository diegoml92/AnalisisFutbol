#include "config_file.h"
#include "Player.h"

/* CLASE ENCARGADA DE LA GESTI�N DE LOS JUGADORES */
class PlayerClassifier {
private:

	/* CALCULA EL HISTOGRAMA DEL JUGADOR */
	static vector<Mat> calculateHistogram(Mat frame, Mat filter, vector<Rect> rects);

	/* CALCULA EL HISTOGRAMA DEL JUGADOR */
	static vector<Mat> calculateHistogram(Point pos, int nCam);

public:

	/* A�ADE UN JUGADOR */
	static void addPlayer(Mat partido, Mat filtro, Point player);

	/* DETERMINA SI CUMPLE EL TAMA�O PROPIO DE UN JUGADOR */
	static bool isPlayerSize(Rect player);

	/* DETERMINA CU�L DE LOS ELEMENTOS ASIGNADOS A UN JUGADOR ES EL CORRECTO */
	static Point findBestMatch(Player* player);

	/* COMPARA LOS HISTOGRAMAS DE LOS JUGADORES */
	static double compareHistogram(vector<Mat> playerHist, vector<Mat> newHist);

	/* DESCARTA FALSOS POSITIVOS (L�NEAS, ETC) EN FUNCI�N DE EL N�MERO DE P�XELES BLANCOS */
	static bool PlayerClassifier::canBePlayer(Mat roi, float val = 0.35);

};

