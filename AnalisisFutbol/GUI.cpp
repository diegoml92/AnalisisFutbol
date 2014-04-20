#include "GUI.h"

Mat GUI::interfaz;

vector<Point> GUI::rButtons;
vector<int> GUI::active_rButtons;

vector<Rect> GUI::buttons;
vector<bool> GUI::activeButtons;

int GUI::hover = -1;

Mat jugador = imread("C:/Proyecto/img/player.png");
Mat balon = imread("C:/Proyecto/img/ball.png");
Mat estela = imread("C:/Proyecto/img/ball_tail.png");

Mat campo = imread("C:/Proyecto/img/field.png");
Mat tam_j = imread("C:/Proyecto/img/player_size.png");
Mat tam_b = imread("C:/Proyecto/img/ball_size.png");

Mat button_off = imread("C:/Proyecto/img/button_off.png");
Mat button_off_hover = imread("C:/Proyecto/img/button_off_hover.png");
Mat button_on = imread("C:/Proyecto/img/button_on.png");
Mat button_on_hover = imread("C:/Proyecto/img/button_on_hover.png");



// Valores que marca en rango de filtro del césped
int GUI::MIN_B = 5;
int GUI::MAX_B = 90;
int GUI::MIN_G = 75;
int GUI::MAX_G = 135;
int GUI::MIN_R = 35;
int GUI::MAX_R = 110;

// Valores para el rango de tamaño de los jugadores
int GUI::MIN_WIDTH = 30;
int GUI::MIN_HEIGH = 65;
int GUI::MAX_WIDTH = 70;
int GUI::MAX_HEIGH = 130;

// Valores para el rango de tamaño del balón
int GUI::MIN_BALL_SIZE = 15;
int GUI::MAX_BALL_SIZE = 30;

/* FUNCIÓN AUXILIAR ACTIVADA CUANDO SE USA UNA TRACKBAR */
void GUI::on_trackbar(int, void*) {}

/* INICIALIZA LA INTERFAZ GRÁFICA */
void GUI::initGUI() {

	interfaz = Mat(Size(500,300), CV_8UC3);			// Creamos la matriz en la que crearemos la interfaz
	interfaz.setTo(Scalar(250,175,0), Mat());		// Establecemos el color de fondo

	/* SE INICIALIZAN LOS RADIO BUTTONS */
	for(int i=0; i<N_RBUTTON_GROUPS; i++) {
		for(int j=0; j<N_RBUTTONS; j++) {
			if(i==0) {
				rButtons.push_back(Point(R_BUTTON_LEFT_MARGIN, R_BUTTON_TOP_MARGIN
											+ R_BUTTON_VERT_SHIFT * j));
			} else if(i == 1) {
				rButtons.push_back(Point(R_BUTTON_LEFT_MARGIN + R_BUTTON_GROUP_SHIFT,
											R_BUTTON_TOP_MARGIN + R_BUTTON_VERT_SHIFT * j));
			} else if(i == 2) {
				rButtons.push_back(Point(R_BUTTON_LEFT_MARGIN, R_BUTTON_TOP_MARGIN
											+ R_BUTTON_VERT_GROUP_SHIFT + R_BUTTON_VERT_SHIFT * j));
			} //else (posible ampliación)
			circle(interfaz, rButtons[i*N_RBUTTONS + j], R_BUTTON_RADIUS, Scalar(255,255,255), -1);
		}
	}

	/* SE INICIALIZAN LOS BUTTONS*/
	for(int i=0; i<N_BUTTONS; i++) {
		buttons.push_back(Rect(Point(BUTTON_LEFT_MARGIN + BUTTON_SHIFT * i,
										BUTTON_TOP_MARGIN), Size(BUTTON_SIZE)));
	}

	/* AÑADIMOS LOS ELEMENTOS A LA INTERFAZ */

	Mat aux;	// Nos ayudará a insertar los elementos en la interfaz

	/* RADIO 1 (PLAYER BOX)*/

	aux = interfaz(Rect(Point(20,40), Point(60,80)));
	jugador.copyTo(aux);			// Copiamos el icono

	putText(interfaz, "None", Point(100, 45), CV_FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255));

	putText(interfaz, "Team Color", Point(100, 65), CV_FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255));
	circle(interfaz, Point(75, 60), 4, Scalar(0,0,0),-1);
	active_rButtons.push_back(1);	// Indicamos que el radio button activo es el 1

	putText(interfaz, "Black", Point(100, 85), CV_FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255));

	line(interfaz, Point(240, 40), Point(240, 85), Scalar(235,220,155), 2);

	/* RADIO 2 (BALL BOX) */

	aux = interfaz(Rect(Point(250 + 20,40), Point(250 + 60,80)));
	balon.copyTo(aux);				// Copiamos el icono

	putText(interfaz, "None", Point(100 + 250, 45), CV_FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255));

	putText(interfaz, "White", Point(100 + 250, 65), CV_FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255));
	circle(interfaz, Point(75 + 250, 60), 4, Scalar(0,0,0),-1);
	active_rButtons.push_back(1);	// Indicamos que el radio button activo es el 1

	putText(interfaz, "White (Filled)", Point(100 + 250, 85), CV_FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255));

	/* RADIO 3 (BALL TAIL)*/

	aux = interfaz(Rect(Point(20,40 + 80), Point(60,80 + 80)));
	estela.copyTo(aux);				// Copiamos el icono

	putText(interfaz, "None", Point(100, 45 + 80), CV_FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255));
	circle(interfaz, Point(75, 40 + 80), 4, Scalar(0,0,0),-1);
	active_rButtons.push_back(0);	// Indicamos que el radio button activo es el 0

	putText(interfaz, "White (N)", Point(100, 65 + 80), CV_FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255));

	putText(interfaz, "Blue (N)", Point(100, 85 + 80), CV_FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255));

	line(interfaz, Point(240, 40 + 80), Point(240, 85 + 80), Scalar(235,220,155), 2);

	/* RADIO 4 (POSIBLE AMPLACIÓN)*/

	/*aux = interfaz(Rect(Point(20 + 250,40 + 80), Point(60 + 250,80 + 80)));
	estela.copyTo(aux);

	putText(interfaz, "None", Point(100 + 250, 45 + 80), CV_FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255));
	circle(interfaz, Point(75 + 250, 40 + 80), 5, Scalar(255,255,255),-1);
	circle(interfaz, Point(75 + 250, 40 + 80), 4, Scalar(0,0,0),-1);

	putText(interfaz, "White", Point(100 + 250, 65 + 80), CV_FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255));
	circle(interfaz, Point(75 + 250, 60 + 80), 5, Scalar(255,255,255),-1);

	putText(interfaz, "Blue", Point(100 + 250, 85 + 80), CV_FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255));
	circle(interfaz, Point(75 + 250, 80 + 80), 5, Scalar(255,255,255),-1);*/

	/* BOTÓN 1 */

	aux = interfaz(Rect(Point(20,40 + 160), Point(60,80 + 160)));
	campo.copyTo(aux);					// Copiamos el icono

	aux = interfaz(buttons[0]);
	button_off.copyTo(aux);				// Copiamos el botón desactivado
	activeButtons.push_back(false);		// Marcamos el botón como desactivado 

	line(interfaz, Point(160, 200), Point(160,245), Scalar(235,220,155), 2);

	/* BOTÓN 2 */

	aux = interfaz(Rect(Point(20 + 170, 40 + 160), Point(60 + 170,80 + 160)));
	tam_j.copyTo(aux);					// Copiamos el icono

	aux = interfaz(buttons[1]);
	button_off.copyTo(aux);				// Copiamos el botón desactivado
	activeButtons.push_back(false);		// Marcamos el botón como desactivado

	line(interfaz, Point(330, 200), Point(330,245), Scalar(235,220,155), 2);

	/* BOTÓN 3 */

	aux = interfaz(Rect(Point(20 + 340, 40 + 160), Point(60 + 340,80 + 160)));
	tam_b.copyTo(aux);					// Copiamos el icono

	aux = interfaz(buttons[2]);
	button_off.copyTo(aux);				// Copiamos el botón desactivado
	activeButtons.push_back(false);		// Marcamos el botón como desactivado

}

/* MUESTRA LA INTERFAZ GRÁFICA */
void GUI::showGUI() {
	imshow(GUI_W, interfaz);
}

/* CREA TRACKBARS PARA LA REGULACIÓN DEL FILTRO DEL CAMPO */
void GUI::fieldThresTrackbars() {
	// Creamos las trackbars
	createTrackbar("MIN_B", FIELD_FILTER_W, &MIN_B, MAX_RGB, on_trackbar);
	createTrackbar("MAX_B", FIELD_FILTER_W, &MAX_B, MAX_RGB, on_trackbar);
	createTrackbar("MIN_G", FIELD_FILTER_W, &MIN_G, MAX_RGB, on_trackbar);
	createTrackbar("MAX_G", FIELD_FILTER_W, &MAX_G, MAX_RGB, on_trackbar);
	createTrackbar("MIN_R", FIELD_FILTER_W, &MIN_R, MAX_RGB, on_trackbar);
	createTrackbar("MAX_R", FIELD_FILTER_W, &MAX_R, MAX_RGB, on_trackbar);
}

/* CREA TRACKBARS PARA EL TAMAÑO DE LOS JUGADORES */
void GUI::playerSizeTrackbars() {
	// Creamos las trackbars
	createTrackbar("Ancho MIN", PLAYER_SIZE_W, &MIN_WIDTH, 100, on_trackbar);
	createTrackbar("Ancho MAX", PLAYER_SIZE_W, &MAX_WIDTH, 100, on_trackbar);
	createTrackbar("Alto MIN", PLAYER_SIZE_W, &MIN_HEIGH, 150, on_trackbar);
	createTrackbar("Alto MAX", PLAYER_SIZE_W, &MAX_HEIGH, 150, on_trackbar);
}

/* CREA TRACKBARS PARA EL TAMAÑO DEL BALÓN */
void GUI::ballSizeTrackbars() {
	// Creamos las trackbars
	createTrackbar("Tamaño MAX", BALL_SIZE_W, &MAX_BALL_SIZE, 20, on_trackbar);
	createTrackbar("Tamaño MIN", BALL_SIZE_W, &MIN_BALL_SIZE, 20, on_trackbar);
} 

/* MARCA AL BUTTON i COMO HOVER */
void GUI::setHoverButton(int i) {
	if(i != hover && i >= 0) {				// Si es negativo, no corresponde a ningún button
		Mat aux = interfaz(buttons[i]);
		if(activeButtons[i]) {				// Si el button i está activo...
			button_on_hover.copyTo(aux);	// ...cargamos button_on_hover
		} else {							// Si no...
			button_off_hover.copyTo(aux);	// ... cargamos button_off_hover
		}
	}
	hover = i;								// Indicamos qué button está a hover
}

/* DESMARCA EL HOVER DEL BUTTON SI LO HAY*/
void GUI::removeHover() {
	if(hover >= 0) {
		Mat aux = interfaz(buttons[hover]);
		if(activeButtons[hover]) {			// Si el button a hover está activo...
			button_on.copyTo(aux);			// ...cargamos button
		} else {							// Si no...
			button_off.copyTo(aux);			// ... cargamos button_off
		}
		setHoverButton(-1);
	}
}

/* CAMBIA EL ESTADO DEL BUTTON */
void GUI::switchButton(int i) {
	Mat aux = interfaz(buttons[i]);
	if(activeButtons[i]) {					// Si el button i está activo...
		button_off.copyTo(aux);				// ...cargamos button_off
	} else {								// Si no...
		button_on.copyTo(aux);				// ...cargamos button_on
	}
	activeButtons[i] = !activeButtons[i];	// Indicamos el cambio de estado
}

/* ACTIVA EL RADIO BUTTON CORRESPONDIENTE */
void GUI::activateRadioButton(int rb, int group) {
	int i = active_rButtons[group];
	if(i!=rb) { // Si rb no es el radio button activo, lo activamos y desactivamos el radio button i
		circle(interfaz, rButtons[group*N_RBUTTONS + i], R_BUTTON_RADIUS, Scalar(255,255,255), -1);
		active_rButtons[group] = rb;
		circle(interfaz, rButtons[group*N_RBUTTONS + rb], R_BUTTON_ACTIVE_RADIUS, Scalar(0,0,0), -1);
	}
}

/* DETERMINA SI ESTÁ ACTIVADO EL BUTTON 0 */
bool GUI::isActiveFieldThres() {
	return activeButtons[0];
}

/* DETERMINA SI ESTÁ ACTIVADO EL BUTTON 1 */
bool GUI::isActivePlayerSize() {
	return activeButtons[1];
}

/* DETERMINA SI ESTÁ ACTIVADO EL BUTTON 2 */
bool GUI::isActiveBallSize() {
	return activeButtons[2];
}

/* DETERMINA QUÉ OPCIÓN ESTÁ ACTIVA EN EL RADIO BUTTON 0 */
int GUI::playerBox() {
	return active_rButtons[0];
}

/* DETERMINA QUÉ OPCIÓN ESTÁ ACTIVA EN EL RADIO BUTTON 1 */
int GUI::ballBox() {
	return active_rButtons[1];
}

/* DETERMINA QUÉ OPCIÓN ESTÁ ACTIVA EN EL RADIO BUTTON 2 */
int GUI::ballTail() {
	return active_rButtons[2];
}