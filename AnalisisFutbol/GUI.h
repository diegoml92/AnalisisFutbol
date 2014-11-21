#include "config_file.h"

/* INTERFAZ GR�FICA DE USUARIO */
class GUI {

private:

	static Mat interfaz;					// Interfaz de usuario
	static Mat statsInterface;				// Interfaz para las estad�sticas

	static vector<Point> rButtons;			// Radio buttons
	static vector<int> active_rButtons;		// S�lo habr� un radio button activo por grupo,
											// y ser� representado mediante su �ndice.
	static vector<Rect> buttons;			// Buttons
	static vector<bool> activeButtons;		// Cada bot�n activo estar� a true

	static vector<Point> rButtonsSts;		// Radio buttons para las estad�sticas
	static vector<int> active_rButtonsSts;	// S�lo habr� un radio button activo por grupo,
											// y ser� representado mediante su �ndice.
	static vector<Rect> checkBoxesSts;		// Buttons para las estad�sticas
	static vector<bool> activeCheckBoxesSts;// Cada bot�n activo estar� a true

	static int hover;						// Bot�n sobre el que se encuentra el rat�n

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

	// Valores que marcan el rango de filtro del c�sped
	static int MIN_B, MAX_B;
	static int MIN_G, MAX_G;
	static int MIN_R, MAX_R;

	// Valores para el rango de tama�o de los jugadores
	static int MIN_WIDTH, MIN_HEIGH;
	static int MAX_WIDTH, MAX_HEIGH;

	// Tama�o del bal�n
	static int MAX_BALL_SIZE, MIN_BALL_SIZE;

	/* INICIALIZA LA INTERFAZ GR�FICA */
	static void initGUI();

	/* MUESTRA LA INTERFAZ GR�FICA */
	static void showGUI();

	/* CREA TRACKBARS PARA LA REGULACI�N DEL FILTRO DEL CAMPO */
	static void fieldThresTrackbars();

	/* CREA TRACKBARS PARA EL TAMA�O DE LOS JUGADORES */
	static void playerSizeTrackbars();

	/* CREA TRACKBARS PARA EL TAMA�O DEL BAL�N */
	static void ballSizeTrackbars();

	/* MARCA AL BUTTON i COMO HOVER */
	static void setHoverButton(int i);

	/* DESMARCA EL HOVER DEL BUTTON SI LO HAY*/
	static void removeHover();

	/* CAMBIA EL ESTADO DEL BUTTON */
	static void switchButton(int i);

	/* ACTIVA EL RADIO BUTTON CORRESPONDIENTE */
	static void activateRadioButton(int rb, int group);

	/* DETERMINA SI EST� ACTIVADO EL BUTTON 0 */
	static bool isActiveFieldThres();

	/* DETERMINA SI EST� ACTIVADO EL BUTTON 1 */
	static bool isActivePlayerSize();

	/* DETERMINA SI EST� ACTIVADO EL BUTTON 2 */
	static bool isActiveBallSize();

	/* DETERMINA QU� OPCI�N EST� ACTIVA EN EL RADIO BUTTON 0 */
	static int playerBox();

	/* DETERMINA QU� OPCI�N EST� ACTIVA EN EL RADIO BUTTON 1 */
	static int ballBox();

	/* DETERMINA QU� OPCI�N EST� ACTIVA EN EL RADIO BUTTON 2 */
	static int ballTail();




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