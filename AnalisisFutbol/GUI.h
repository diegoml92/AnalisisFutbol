#include "config_file.h"

/* INTERFAZ GRÁFICA DE USUARIO */
class GUI {

private:

	static Mat interfaz;					// Interfaz de usuario
	static Mat statsInterface;				// Interfaz para las estadísticas

	static vector<Point> rButtons;			// Radio buttons
	static vector<int> active_rButtons;		// Sólo habrá un radio button activo por grupo,
											// y será representado mediante su índice.
	static vector<Rect> buttons;			// Buttons
	static vector<bool> activeButtons;		// Cada botón activo estará a true

	static vector<Point> rButtonsSts;		// Radio buttons para las estadísticas
	static vector<int> active_rButtonsSts;	// Sólo habrá un radio button activo por grupo,
											// y será representado mediante su índice.
	static vector<Rect> checkBoxesSts;		// Buttons para las estadísticas
	static vector<bool> activeCheckBoxesSts;// Cada botón activo estará a true

	static int hover;						// Botón sobre el que se encuentra el ratón

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

	// Valores que marcan el rango de filtro del césped
	static int MIN_B, MAX_B;
	static int MIN_G, MAX_G;
	static int MIN_R, MAX_R;

	// Valores para el rango de tamaño de los jugadores
	static int MIN_WIDTH, MIN_HEIGH;
	static int MAX_WIDTH, MAX_HEIGH;

	// Tamaño del balón
	static int MAX_BALL_SIZE, MIN_BALL_SIZE;

	/* INICIALIZA LA INTERFAZ GRÁFICA */
	static void initGUI();

	/* MUESTRA LA INTERFAZ GRÁFICA */
	static void showGUI();

	/* CREA TRACKBARS PARA LA REGULACIÓN DEL FILTRO DEL CAMPO */
	static void fieldThresTrackbars();

	/* CREA TRACKBARS PARA EL TAMAÑO DE LOS JUGADORES */
	static void playerSizeTrackbars();

	/* CREA TRACKBARS PARA EL TAMAÑO DEL BALÓN */
	static void ballSizeTrackbars();

	/* MARCA AL BUTTON i COMO HOVER */
	static void setHoverButton(int i);

	/* DESMARCA EL HOVER DEL BUTTON SI LO HAY*/
	static void removeHover();

	/* CAMBIA EL ESTADO DEL BUTTON */
	static void switchButton(int i);

	/* ACTIVA EL RADIO BUTTON CORRESPONDIENTE */
	static void activateRadioButton(int rb, int group);

	/* DETERMINA SI ESTÁ ACTIVADO EL BUTTON 0 */
	static bool isActiveFieldThres();

	/* DETERMINA SI ESTÁ ACTIVADO EL BUTTON 1 */
	static bool isActivePlayerSize();

	/* DETERMINA SI ESTÁ ACTIVADO EL BUTTON 2 */
	static bool isActiveBallSize();

	/* DETERMINA QUÉ OPCIÓN ESTÁ ACTIVA EN EL RADIO BUTTON 0 */
	static int playerBox();

	/* DETERMINA QUÉ OPCIÓN ESTÁ ACTIVA EN EL RADIO BUTTON 1 */
	static int ballBox();

	/* DETERMINA QUÉ OPCIÓN ESTÁ ACTIVA EN EL RADIO BUTTON 2 */
	static int ballTail();




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