#include "config_file.h"

/* INTERFAZ GR�FICA DE USUARIO */
class GUI {

private:

	// Interfaz de usuario
	static Mat interfaz;

	// Elementos de la interfaz

	/* RADIO BUTTON */
	//static Point rb1a, rb1b, rb1c, rb2a, rb2b, rb2c, rb3a, rb3b, rb3c;
	/* BUTTON */
	//static Rect button1, button2, button3;

	static vector<Rect> buttons;

	/* FUNCI�N AUXILIAR ACTIVADA CUANDO SE USA UNA TRACKBAR */
	static void on_trackbar(int, void*);

public:


	// Valores que marca en rango de filtro del c�sped
	static int MIN_B, MAX_B;
	static int MIN_G, MAX_G;
	static int MIN_R, MAX_R;

	// Valores para el rango de tama�o de los jugadores
	static int MIN_WIDTH, MIN_HEIGH;
	static int MAX_WIDTH, MAX_HEIGH;

	/* CREA TRACKBARS PARA LA REGULACI�N DEL FILTRO DEL CAMPO */
	static void fieldThresTrackbars();

	/* CREA TRACKBARS PARA EL TAMA�O DE LOS JUGADORES */
	static void playerSizeTrackbars();

	/* INICIA LA INTERFAZ GR�FICA */
	static void initGUI();

	/* MUESTRA LA INTERFAZ GR�FICA */
	static void showGUI();

	/* MARCA AL BUTTON i COMO HOVER */
	static void setHoverButton(int i);
};

