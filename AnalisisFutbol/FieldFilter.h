#include "config_file.h"

/* FILTRADO DEL TERRENO DE JUEGO */
class FieldFilter {

public:

	/* FILTRAMOS EL CAMPO PARA ENCONTRAR A LOS JUGADORES */
	static Mat discardField(Mat partido, Mat bg);
};

