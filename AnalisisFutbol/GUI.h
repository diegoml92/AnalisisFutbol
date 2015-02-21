#include "config_file.h"

/* INTERFAZ GRÁFICA DE USUARIO */
class GUI {

private:

	static Mat statsInterface;				// Interfaz para las estadísticas

	static vector<Point> rButtonsSts;		// Radio buttons para las estadísticas
	static vector<int> active_rButtonsSts;	// Sólo habrá un radio button activo por grupo,
											// y será representado mediante su índice.
	static vector<Rect> checkBoxesSts;		// Buttons para las estadísticas
	static vector<bool> activeCheckBoxesSts;// Cada botón activo estará a true

	/* FUNCIÓN AUXILIAR ACTIVADA CUANDO SE USA UNA TRACKBAR */
	static void on_trackbar(int, void*);

	/* FUNCIÓN QUE OCULTA ELEMENTOS EN LA INTERFAZ */
	static void hidePlayers();

	/* FUNCIÓN QUE MUESTRA ELEMENTOS EN LA INTERFAZ */
	static void unhidePlayers(int rButton);

	/* FUNCIÓN QUE DEVUELVE EL EQUIPO SELECCIONADO */
	static int getActiveTeam();

	/* FUNCIÓN QUE DEVUELVE EL JUGADOR SELECCIONADO */
	static int getActivePlayer(int team);

public:

	/* INICIALIZA LA INTERFAZ GRÁFICA PARA LAS ESTADÍSTICAS */
	static void initStatsGUI();

	/* MUESTRA LA INTERFAZ GRÁFICA PARA LAS ESTADÍSTICAS */
	static void showStatsGUI();

	/* MUESTRA LAS ESTADÍSTICAS SELECCIONADAS */
	static void showStatsWindow(int individualMode, int comparativeMode);

	/* CAMBIA EL ESTADO DEL CHECKBOX */
	static void switchCheckBox(int i);

	/* ACTIVA EL RADIO BUTTON CORRESPONDIENTE */
	static void activateRadioButtonSts(int rb, int group);

	/* DETERMINA SI ESTÁ ACTIVADO EL CHECKBOX 0 */
	static bool isActiveIndividualMode();

	/* DETERMINA SI ESTÁ ACTIVADO EL CHECKBOX 1 */
	static bool isActiveComparativeMode();
};