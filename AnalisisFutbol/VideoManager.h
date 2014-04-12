#include "config_file.h"

/* CLASE ENCARGADA DE LA GESTIÓN DE LA SECUENCIA DE VÍDEO */
class VideoManager {

public:

	// Almacenará la captura de vídeo
	static cv::VideoCapture video;

	/*	INICILIZA LA CAPTURA DE VÍDEO:
	*	Devuelve true si ha iniciado correctamente
	*/
	static bool init();

	/*	OBTIENE EL SIGUIENTE FRAME DE VÍDEO:
	*	Se le pasa el Mat en el que se guardará este frame
	*/
	static bool nextFrame(cv::Mat* partido);
};

