#include "config_file.h"

/* FILTRADO DEL TERRENO DE JUEGO */
class FieldFilter {
private:

	// M�scara utilizada en el filtro
	static Mat mask[N_VIDEOS];

public:

	/* FILTRAMOS EL CAMPO PARA ENCONTRAR A LOS JUGADORES */
	static Mat discardField(Mat partido, Mat bg, int nCam);

	/* SE INICIALIZA LA M�SCARA PARA EL FILTRO */
	static void initFilterMask(int nCam);
};

