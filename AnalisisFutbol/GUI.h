#include "config_file.h"

/* INTERFAZ GR�FICA DE USUARIO */
class GUI {

private:

	static Mat statsInterface;				// Interfaz para las estad�sticas

public:

	// Jugador marcado actualmente
	static unsigned nPlayer;

	/* MUESTRA LAS ESTAD�STICAS SELECCIONADAS */
	static void showStatsWindow();

};