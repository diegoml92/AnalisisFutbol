#include "config_file.h"

/* INTERFAZ GRÁFICA DE USUARIO */
class GUI {

private:

	static Mat statsInterface;	// Interfaz para las estadísticas

public:

	static int nPlayer;	// Jugador visualizado actualmente

	/* MUESTRA LAS ESTADÍSTICAS DEL JUGADOR */
	static void showStatsWindow();

};