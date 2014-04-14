#include "config_file.h"

/* INTERFAZ GRÁFICA DE USUARIO */
class GUI {

private:

	/* FUNCIÓN AUXILIAR ACTIVADA CUANDO SE USA UNA TRACKBAR */
	static void on_trackbar(int, void*);

public:

	// Valores que marca en rango de filtro del césped
	static int MIN_B, MAX_B;
	static int MIN_G, MAX_G;
	static int MIN_R, MAX_R;

	// Valores para el rango de tamaño de los jugadores
	static int MIN_WIDTH, MIN_HEIGH;
	static int MAX_WIDTH, MAX_HEIGH;

	/* CREA TRACKBARS PARA LA REGULACIÓN DEL FILTRO DEL CAMPO */
	static void fieldThresTrackbars();

	/* CREA TRACKBARS PARA EL TAMAÑO DE LOS JUGADORES */
	static void playerSizeTrackbars();

};

