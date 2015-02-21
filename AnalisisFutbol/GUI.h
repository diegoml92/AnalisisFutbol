#include "config_file.h"

/* INTERFAZ GR�FICA DE USUARIO */
class GUI {

private:

	static Mat statsInterface;				// Interfaz para las estad�sticas

	static vector<Point> rButtonsSts;		// Radio buttons para las estad�sticas
	static vector<int> active_rButtonsSts;	// S�lo habr� un radio button activo por grupo,
											// y ser� representado mediante su �ndice.
	static vector<Rect> checkBoxesSts;		// Buttons para las estad�sticas
	static vector<bool> activeCheckBoxesSts;// Cada bot�n activo estar� a true

	/* FUNCI�N AUXILIAR ACTIVADA CUANDO SE USA UNA TRACKBAR */
	static void on_trackbar(int, void*);

	/* FUNCI�N QUE OCULTA ELEMENTOS EN LA INTERFAZ */
	static void hidePlayers();

	/* FUNCI�N QUE MUESTRA ELEMENTOS EN LA INTERFAZ */
	static void unhidePlayers(int rButton);

	/* FUNCI�N QUE DEVUELVE EL EQUIPO SELECCIONADO */
	static int getActiveTeam();

	/* FUNCI�N QUE DEVUELVE EL JUGADOR SELECCIONADO */
	static int getActivePlayer(int team);

public:

	/* INICIALIZA LA INTERFAZ GR�FICA PARA LAS ESTAD�STICAS */
	static void initStatsGUI();

	/* MUESTRA LA INTERFAZ GR�FICA PARA LAS ESTAD�STICAS */
	static void showStatsGUI();

	/* MUESTRA LAS ESTAD�STICAS SELECCIONADAS */
	static void showStatsWindow(int individualMode, int comparativeMode);

	/* CAMBIA EL ESTADO DEL CHECKBOX */
	static void switchCheckBox(int i);

	/* ACTIVA EL RADIO BUTTON CORRESPONDIENTE */
	static void activateRadioButtonSts(int rb, int group);

	/* DETERMINA SI EST� ACTIVADO EL CHECKBOX 0 */
	static bool isActiveIndividualMode();

	/* DETERMINA SI EST� ACTIVADO EL CHECKBOX 1 */
	static bool isActiveComparativeMode();
};