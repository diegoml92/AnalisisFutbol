#include "config_file.h"

/* CLASE ENCARGADA DE LA GESTI�N DE LA SECUENCIA DE V�DEO */
class VideoManager {

private:

	static VideoCapture[] video;	// Almacenar� la captura de v�deo

public:

	/* INICILIZA LA CAPTURA DE V�DEO */
	static bool init();

	/* OBTIENE EL SIGUIENTE FRAME DE V�DEO */
	static bool nextFrame(Mat[] partido);

};

