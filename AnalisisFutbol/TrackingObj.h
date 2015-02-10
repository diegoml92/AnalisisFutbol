#include "config_file.h"

/* CLASE ENCARGADA DEL SEGUIMIENTO DE LOS ELEMENTOS DEL CAMPO */
class TrackingObj {

public:

	/* DETECCIÓN DE JUGADORES SOBRE EL CAMPO */
	static void objectDetection(Mat filtro, Mat &partido, int nVideo, Mat paint);

};