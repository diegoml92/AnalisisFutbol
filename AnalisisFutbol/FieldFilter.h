#include "config_file.h"

/* FILTRADO DEL TERRENO DE JUEGO */
class FieldFilter {
private:

	// Máscara utilizada en el filtro
	static Mat mask[N_VIDEOS];

public:

	/* FILTRAMOS EL CAMPO PARA ENCONTRAR A LOS JUGADORES */
	static Mat discardField(Mat partido, Mat bg, int nCam);

	/* SE INICIALIZA LA MÁSCARA PARA EL FILTRO */
	static void initFilterMask(int nCam);
};

