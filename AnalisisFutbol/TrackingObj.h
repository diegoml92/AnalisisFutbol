#include "config_file.h"
#include "PlayerClassifier.h"

/* CLASE ENCARGADA DEL SEGUIMIENTO DE LOS ELEMENTOS DEL CAMPO */
class TrackingObj {

public:

	/* REALIZA EL SEGUIMIENTO DE LOS ELEMENTOS DEL PARTIDO */
	static void trackObject(Mat filtro, Mat &partido);

};