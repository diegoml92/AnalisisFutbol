#include "config_file.h"

/* CLASE ENCARGADA DE LA GESTI�N DE LA SECUENCIA DE V�DEO */
class VideoManager {

private:

	static VideoCapture video[];	// Almacenar� la captura de v�deo

	static int actualFrame;			// Determina el n�mero de frames recorridos

public:

	/* INICILIZA LA CAPTURA DE V�DEO */
	static bool init();

	/* OBTIENE EL SIGUIENTE FRAME DE V�DEO */
	static bool nextFrame(Mat frame[]);
    
    /* UNE LAS SECUENCIAS DE V�DEO EN UNA SOLA IMAGEN */
    static Mat joinSequences(Mat frame[]);

	/* DEVUELVE EL N�MERO DE FRAME ACTUAL */
	static int getActualFrame();

};

