#include "GUI.h"

Mat GUI::interfaz;

//Point GUI::rb1a, GUI::rb1b, GUI::rb1c, GUI::rb2a,
//	GUI::rb2b, GUI::rb2c, GUI::rb3a, GUI::rb3b, GUI::rb3c;
//Rect GUI::button1, GUI::button2, GUI::button3;
vector<Rect> GUI::buttons;



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
int GUI::MIN_WIDTH = 10;
int GUI::MIN_HEIGH = 25;
int GUI::MAX_WIDTH = 45;
int GUI::MAX_HEIGH = 80;

void GUI::on_trackbar(int, void*) {}

void GUI::fieldThresTrackbars() {
	// Creamos las trackbars
	createTrackbar("MIN_B", GUI_W, &MIN_B, MAX_RGB, on_trackbar);
	createTrackbar("MAX_B", GUI_W, &MAX_B, MAX_RGB, on_trackbar);
	createTrackbar("MIN_G", GUI_W, &MIN_G, MAX_RGB, on_trackbar);
	createTrackbar("MAX_G", GUI_W, &MAX_G, MAX_RGB, on_trackbar);
	createTrackbar("MIN_R", GUI_W, &MIN_R, MAX_RGB, on_trackbar);
	createTrackbar("MAX_R", GUI_W, &MAX_R, MAX_RGB, on_trackbar);
}

void GUI::playerSizeTrackbars() {
	// Creamos las trackbars
	createTrackbar("Ancho MIN", GUI_W, &MIN_WIDTH, 100, on_trackbar);
	createTrackbar("Ancho MAX", GUI_W, &MAX_WIDTH, 100, on_trackbar);
	createTrackbar("Alto MIN", GUI_W, &MIN_HEIGH, 100, on_trackbar);
	createTrackbar("Alto MAX", GUI_W, &MAX_HEIGH, 100, on_trackbar);
}

void GUI::initGUI() {

	for(int i=0; i<C_NBUTTONS; i++) {
		buttons.push_back(Rect(Point(BUTTON_LEFT_MARGIN + BUTTON_SHIFT * i,
										BUTTON_TOP_MARGIN), Size(BUTTON_SIZE)));
	}

	/*button1 = Rect(Point(BUTTON_LEFT_MARGIN, BUTTON_TOP_MARGIN), Size(BUTTON_SIZE));
	button2 = Rect(Point(BUTTON_LEFT_MARGIN + BUTTON_SHIFT, BUTTON_TOP_MARGIN), Size(BUTTON_SIZE));
	button3 = Rect(Point(BUTTON_LEFT_MARGIN + BUTTON_SHIFT * 2, BUTTON_TOP_MARGIN), Size(BUTTON_SIZE));*/
	

	// Creamos la matriz en la que crearemos la interfaz
	interfaz = Mat(Size(500,300), CV_8UC3);

	// Establecemos el color de fondo
	interfaz.setTo(Scalar(250,175,0), Mat());

	// Añadimos los elemtos de la interfaz
	Mat aux;

	/* RADIO 1 */

	aux = interfaz(Rect(Point(20,40), Point(60,80)));
	jugador.copyTo(aux);

	putText(interfaz, "None", Point(100, 45), CV_FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255));
	circle(interfaz, Point(75, 40), 5, Scalar(255,255,255),-1);

	putText(interfaz, "Team Color", Point(100, 65), CV_FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255));
	circle(interfaz, Point(75, 60), 5, Scalar(255,255,255),-1);
	circle(interfaz, Point(75, 60), 4, Scalar(0,0,0),-1);

	putText(interfaz, "Black", Point(100, 85), CV_FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255));
	circle(interfaz, Point(75, 80), 5, Scalar(255,255,255),-1);

	line(interfaz, Point(240, 40), Point(240, 85), Scalar(235,220,155), 2);

	/* RADIO 2 */

	aux = interfaz(Rect(Point(250 + 20,40), Point(250 + 60,80)));
	balon.copyTo(aux);

	putText(interfaz, "None", Point(100 + 250, 45), CV_FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255));
	circle(interfaz, Point(75 + 250, 40), 5, Scalar(255,255,255),-1);

	putText(interfaz, "White", Point(100 + 250, 65), CV_FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255));
	circle(interfaz, Point(75 + 250, 60), 5, Scalar(255,255,255),-1);
	circle(interfaz, Point(75 + 250, 60), 4, Scalar(0,0,0),-1);

	putText(interfaz, "White (Filled)", Point(100 + 250, 85), CV_FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255));
	circle(interfaz, Point(75 + 250, 80), 5, Scalar(255,255,255),-1);

	/* RADIO 3 */

	aux = interfaz(Rect(Point(20,40 + 80), Point(60,80 + 80)));
	estela.copyTo(aux);

	putText(interfaz, "None", Point(100, 45 + 80), CV_FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255));
	circle(interfaz, Point(75, 40 + 80), 5, Scalar(255,255,255),-1);
	circle(interfaz, Point(75, 40 + 80), 4, Scalar(0,0,0),-1);

	putText(interfaz, "White", Point(100, 65 + 80), CV_FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255));
	circle(interfaz, Point(75, 60 + 80), 5, Scalar(255,255,255),-1);

	putText(interfaz, "Blue", Point(100, 85 + 80), CV_FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255));
	circle(interfaz, Point(75, 80 + 80), 5, Scalar(255,255,255),-1);

	line(interfaz, Point(240, 40 + 80), Point(240, 85 + 80), Scalar(235,220,155), 2);

	/* RADIO 4 */

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
	campo.copyTo(aux);

	//aux = interfaz(Rect(Point(75, 40 + 170), Point(75 + 60, 40 + 195)));
	aux = interfaz(buttons[0]);
	button_off.copyTo(aux);

	/*rectangle(interfaz, Point(75, 40 + 170), Point(125, 40 + 190), Scalar(71,71,71), -1);
	circle(interfaz, Point(80, 40 + 180), 11, Scalar(195,195,195), -1);
	putText(interfaz, "OFF", Point(93, 40 + 186), CV_FONT_HERSHEY_PLAIN, 1, Scalar(0,0,235), 2);*/

	line(interfaz, Point(160, 200), Point(160,245), Scalar(235,220,155), 2);

	/* BOTÓN 2 */

	aux = interfaz(Rect(Point(20 + 170, 40 + 160), Point(60 + 170,80 + 160)));
	tam_j.copyTo(aux);

	//aux = interfaz(Rect(Point(75 + 170, 40 + 170), Point(75 + 170 + 60, 40 + 195)));
	aux = interfaz(buttons[1]);
	button_off.copyTo(aux);

	/*rectangle(interfaz, Point(75 + 170, 40 + 170), Point(125 + 170, 40 + 190), Scalar(71,71,71), -1);
	circle(interfaz, Point(80 + 170, 40 + 180), 11, Scalar(195,195,195), -1);
	putText(interfaz, "OFF", Point(93 + 170, 40 + 186), CV_FONT_HERSHEY_PLAIN, 1, Scalar(0,0,235), 2);*/

	line(interfaz, Point(330, 200), Point(330,245), Scalar(235,220,155), 2);

	/* BOTÓN 3 */

	aux = interfaz(Rect(Point(20 + 340, 40 + 160), Point(60 + 340,80 + 160)));
	tam_b.copyTo(aux);

	//aux = interfaz(Rect(Point(75 + 340, 40 + 170), Point(75 + 340 + 60, 40 + 195)));
	aux = interfaz(buttons[2]);
	button_off.copyTo(aux);

	/*rectangle(interfaz, Point(75 + 340, 40 + 170), Point(125 + 340, 40 + 190), Scalar(71,71,71), -1);
	circle(interfaz, Point(80 + 340, 40 + 180), 11, Scalar(195,195,195), -1);
	putText(interfaz, "OFF", Point(93 + 340, 40 + 186), CV_FONT_HERSHEY_PLAIN, 1, Scalar(0,0,235), 2);*/
	

	//OK
	/*rectangle(interfaz, Point(75, 40 + 220), Point(125, 40 + 240), Scalar(71,71,71), -1);
	circle(interfaz, Point(120, 40 + 230), 11, Scalar(195,195,195), -1);
	putText(interfaz, "ON", Point(80, 40 + 236), CV_FONT_HERSHEY_PLAIN, 1, Scalar(100,235,50), 2);*/

}

void GUI::showGUI() {
	imshow(GUI_W, interfaz);
} 

void GUI::setHoverButton(int i) {
	// COMPROBAR SI ESTÁ A ON O OFF
	Mat aux = interfaz(buttons[i]);
	button_off_hover.copyTo(aux);
}