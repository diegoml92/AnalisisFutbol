#include "config_file.h"

/* INTERFAZ GRÁFICA DE USUARIO */
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

	/* FUNCIÓN AUXILIAR ACTIVADA CUANDO SE USA UNA TRACKBAR */
	static void on_trackbar(int, void*);

public:


	// Valores que marca en rango de filtro del césped
	static int MIN_B, MAX_B;
	static int MIN_G, MAX_G;
	static int MIN_R, MAX_R;

	// Valores para el rango de tamaño de los jugadores
	static int MIN_WIDTH, MIN_HEIGH;
	static int MAX_WIDTH, MAX_HEIGH;

	/* CREA TRACKBARS PARA LA REGULACIÓN DEL FILTRO DEL CAMPO */
	static void fieldThresTrackbars();

	/* CREA TRACKBARS PARA EL TAMAÑO DE LOS JUGADORES */
	static void playerSizeTrackbars();

	/* INICIA LA INTERFAZ GRÁFICA */
	static void initGUI();

	/* MUESTRA LA INTERFAZ GRÁFICA */
	static void showGUI();

	/* MARCA AL BUTTON i COMO HOVER */
	static void setHoverButton(int i);
};

