#include "config_file.h"

/* CLASE ENCARGADA DE LA GESTI�N DE LA SECUENCIA DE V�DEO */
class VideoManager {

public:

	// Almacenar� la captura de v�deo
	static cv::VideoCapture video;

	/*	INICILIZA LA CAPTURA DE V�DEO:
	*	Devuelve true si ha iniciado correctamente
	*/
	static bool init();

	/*	OBTIENE EL SIGUIENTE FRAME DE V�DEO:
	*	Se le pasa el Mat en el que se guardar� este frame
	*/
	static bool nextFrame(cv::Mat* partido);
};

