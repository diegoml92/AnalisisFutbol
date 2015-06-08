#include "config_file.h"

/* CLASE ENCARGADA DE LA GESTIÓN DE LA SECUENCIA DE VÍDEO */
class VideoManager {

private:

	static VideoCapture video[];	// Almacenará la captura de vídeo

	static int actualFrame;			// Determina el número de frames recorridos

public:

	/* INICILIZA LA CAPTURA DE VÍDEO */
	static bool init();

	/* OBTIENE EL SIGUIENTE FRAME DE VÍDEO */
	static bool nextFrame(Mat frame[]);
    
    /* UNE LAS SECUENCIAS DE VêDEO EN UNA SOLA IMAGEN */
    static Mat joinSequences(Mat frame[]);

	/* DEVUELVE EL NÚMERO DE FRAME ACTUAL */
	static int getActualFrame();

};

