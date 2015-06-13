#include "config_file.h"

/* FILTRADO DEL TERRENO DE JUEGO */
class FieldFilter {
private:

	// Almacenar� los backgrounds de cada secuencia
	static Mat bg[N_VIDEOS];

	// M�scara utilizada en el filtro
	static Mat mask[N_VIDEOS];

	/* SE INICIALIZA LA M�SCARA PARA EL FILTRO */
	static void initFilterMask(int nCam);

public:

	/* INICIA LOS ELEMENTOS NECESARIOS PARA EL FILTRADO */
	static void initFilter();

	/* FILTRAMOS EL CAMPO PARA ENCONTRAR A LOS JUGADORES */
	static void discardField();

};

