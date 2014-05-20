#include "config_file.h"

/* CLASE ENCARGADA DE LA GESTIÓN DE LA SECUENCIA DE VÍDEO */
class VideoManager {

private:

	static VideoCapture[] video;	// Almacenará la captura de vídeo

public:

	/* INICILIZA LA CAPTURA DE VÍDEO */
	static bool init();

	/* OBTIENE EL SIGUIENTE FRAME DE VÍDEO */
	static bool nextFrame(Mat[] partido);

};

