#include "config_file.h"

/* INTERFAZ GRÁFICA DE USUARIO */
class GUI {

private:

	static Mat interfaz;					// Interfaz de usuario

	static vector<Point> rButtons;			// Radio buttons
	static vector<int> active_rButtons;		// Sólo habrá un radio button activo por grupo,
											// y será representado mediante su índice.
	static vector<Rect> buttons;			// Buttons
	static vector<bool> activeButtons;		// Cada botón activo estará a true

	static int hover;						// Botón sobre el que se encuentra el ratón

	/* FUNCIÓN AUXILIAR ACTIVADA CUANDO SE USA UNA TRACKBAR */
	static void on_trackbar(int, void*);

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
};