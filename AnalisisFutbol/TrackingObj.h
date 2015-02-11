#include "config_file.h"

/* CLASE ENCARGADA DEL SEGUIMIENTO DE LOS ELEMENTOS DEL CAMPO */
class TrackingObj {

public:

        /* TRACKING DE LOS JUGADORES */
        static void trackPlayers(vector<Rect> detectedPlayers);

	/* DETECCIÓN DE JUGADORES SOBRE EL CAMPO */
	static void objectDetection(Mat filtro, Mat &partido, int nVideo, Mat paint);

};
