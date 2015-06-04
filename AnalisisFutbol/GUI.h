#include "config_file.h"

/* INTERFAZ GRÁFICA DE USUARIO */
class GUI {

private:

	static Mat statsInterface;				// Interfaz para las estadísticas

public:

	// Jugador marcado actualmente
	static unsigned nPlayer;

	/* MUESTRA LAS ESTADÍSTICAS SELECCIONADAS */
	static void showStatsWindow();

};