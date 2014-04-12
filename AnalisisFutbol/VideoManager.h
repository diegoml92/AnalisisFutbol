#include "config_file.h"

/* CLASE ENCARGADA DE LA GESTIÓN DE LA SECUENCIA DE VÍDEO */
class VideoManager {

private:

	// Almacenará la captura de vídeo
	static VideoCapture video;

public:

	/*	
	*	INICILIZA LA CAPTURA DE VÍDEO:
	*	Devuelve true si ha iniciado correctamente
	*/
	static bool init();

	/*	
	*	OBTIENE EL SIGUIENTE FRAME DE VÍDEO:
	*	Se le pasa el Mat en el que se guardará este frame
	*/
	static bool nextFrame(Mat* partido);
};

