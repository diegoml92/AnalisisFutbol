#include "config_file.h"

/* INTERFAZ GR�FICA DE USUARIO */
class GUI {

private:

	static Mat statsInterface;	// Interfaz para las estad�sticas

public:

	static int nPlayer;	// Jugador visualizado actualmente

	/* MUESTRA LAS ESTAD�STICAS DEL JUGADOR */
	static void showStatsWindow();

};