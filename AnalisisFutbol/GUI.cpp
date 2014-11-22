#include "GUI.h"
#include "GlobalStats.h"

Mat GUI::interfaz;
Mat GUI::statsInterface;

vector<Point> GUI::rButtons;
vector<int> GUI::active_rButtons;

vector<Rect> GUI::buttons;
vector<bool> GUI::activeButtons;

vector<Point> GUI::rButtonsSts;
vector<int> GUI::active_rButtonsSts;

vector<Rect> GUI::checkBoxesSts;
vector<bool> GUI::activeCheckBoxesSts;

int GUI::hover = -1;

Mat soccer_field = imread("C:/Proyecto/soccer_field_small.jpg");

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
int GUI::MIN_B = 25;
int GUI::MAX_B = 85;
int GUI::MIN_G = 55;
int GUI::MAX_G = 135;
int GUI::MIN_R = 40;
int GUI::MAX_R = 110;

// Valores para el rango de tamaño de los jugadores
int GUI::MIN_WIDTH = 30;
int GUI::MIN_HEIGH = 65;
int GUI::MAX_WIDTH = 70;
int GUI::MAX_HEIGH = 130;

// Valores para el rango de tamaño del balón
int GUI::MIN_BALL_SIZE = 15;
int GUI::MAX_BALL_SIZE = 30;

// MAIN GUI

/* FUNCIÓN AUXILIAR ACTIVADA CUANDO SE USA UNA TRACKBAR */
void GUI::on_trackbar(int, void*) {}

/* INICIALIZA LA INTERFAZ GRÁFICA */
void GUI::initGUI() {

	interfaz = Mat(Size(500,300), CV_8UC3);			// Creamos la matriz en la que crearemos la interfaz
	interfaz.setTo(Scalar(GUI_COLOR), Mat());		// Establecemos el color de fondo

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
			circle(interfaz, rButtons[i*N_RBUTTONS + j], R_BUTTON_RADIUS, Scalar(COLOR_WHITE), -1);
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

	putText(interfaz, "None", Point(100, 45), CV_FONT_HERSHEY_PLAIN, 1, Scalar(COLOR_WHITE));

	putText(interfaz, "Team Color", Point(100, 65), CV_FONT_HERSHEY_PLAIN, 1, Scalar(COLOR_WHITE));
	circle(interfaz, Point(75, 60), 4, Scalar(COLOR_BLACK),-1);
	active_rButtons.push_back(1);	// Indicamos que el radio button activo es el 1

	putText(interfaz, "Black", Point(100, 85), CV_FONT_HERSHEY_PLAIN, 1, Scalar(COLOR_WHITE));

	line(interfaz, Point(240, 40), Point(240, 85), Scalar(GUI_AUX_COLOR), 2);

	/* RADIO 2 (BALL BOX) */

	aux = interfaz(Rect(Point(250 + 20,40), Point(250 + 60,80)));
	balon.copyTo(aux);				// Copiamos el icono

	putText(interfaz, "None", Point(100 + 250, 45), CV_FONT_HERSHEY_PLAIN, 1, Scalar(COLOR_WHITE));

	putText(interfaz, "White", Point(100 + 250, 65), CV_FONT_HERSHEY_PLAIN, 1, Scalar(COLOR_WHITE));
	circle(interfaz, Point(75 + 250, 60), 4, Scalar(COLOR_BLACK),-1);
	active_rButtons.push_back(1);	// Indicamos que el radio button activo es el 1

	putText(interfaz, "White (Filled)", Point(100 + 250, 85), CV_FONT_HERSHEY_PLAIN, 1, Scalar(COLOR_WHITE));

	/* RADIO 3 (BALL TAIL)*/

	aux = interfaz(Rect(Point(20,40 + 80), Point(60,80 + 80)));
	estela.copyTo(aux);				// Copiamos el icono

	putText(interfaz, "None", Point(100, 45 + 80), CV_FONT_HERSHEY_PLAIN, 1, Scalar(COLOR_WHITE));
	circle(interfaz, Point(75, 40 + 80), 4, Scalar(COLOR_BLACK),-1);
	active_rButtons.push_back(0);	// Indicamos que el radio button activo es el 0

	putText(interfaz, "White (N)", Point(100, 65 + 80), CV_FONT_HERSHEY_PLAIN, 1, Scalar(COLOR_WHITE));

	putText(interfaz, "Blue (N)", Point(100, 85 + 80), CV_FONT_HERSHEY_PLAIN, 1, Scalar(COLOR_WHITE));

	line(interfaz, Point(240, 40 + 80), Point(240, 85 + 80), Scalar(GUI_AUX_COLOR), 2);

	/* RADIO 4 (POSIBLE AMPLACIÓN)*/

	/*aux = interfaz(Rect(Point(20 + 250,40 + 80), Point(60 + 250,80 + 80)));
	estela.copyTo(aux);

	putText(interfaz, "None", Point(100 + 250, 45 + 80), CV_FONT_HERSHEY_PLAIN, 1, Scalar(COLOR_WHITE));
	circle(interfaz, Point(75 + 250, 40 + 80), 5, Scalar(COLOR_WHITE),-1);
	circle(interfaz, Point(75 + 250, 40 + 80), 4, Scalar(COLOR_BLACK),-1);

	putText(interfaz, "White", Point(100 + 250, 65 + 80), CV_FONT_HERSHEY_PLAIN, 1, Scalar(COLOR_WHITE));
	circle(interfaz, Point(75 + 250, 60 + 80), 5, Scalar(COLOR_WHITE),-1);

	putText(interfaz, "Blue", Point(100 + 250, 85 + 80), CV_FONT_HERSHEY_PLAIN, 1, Scalar(COLOR_WHITE));
	circle(interfaz, Point(75 + 250, 80 + 80), 5, Scalar(COLOR_WHITE),-1);*/

	/* BOTÓN 1 */

	aux = interfaz(Rect(Point(20,40 + 160), Point(60,80 + 160)));
	campo.copyTo(aux);					// Copiamos el icono

	aux = interfaz(buttons[0]);
	button_off.copyTo(aux);				// Copiamos el botón desactivado
	activeButtons.push_back(false);		// Marcamos el botón como desactivado 

	line(interfaz, Point(160, 200), Point(160,245), Scalar(GUI_AUX_COLOR), 2);

	/* BOTÓN 2 */

	aux = interfaz(Rect(Point(20 + 170, 40 + 160), Point(60 + 170,80 + 160)));
	tam_j.copyTo(aux);					// Copiamos el icono

	aux = interfaz(buttons[1]);
	button_off.copyTo(aux);				// Copiamos el botón desactivado
	activeButtons.push_back(false);		// Marcamos el botón como desactivado

	line(interfaz, Point(330, 200), Point(330,245), Scalar(GUI_AUX_COLOR), 2);

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
		circle(interfaz, rButtons[group*N_RBUTTONS + i], R_BUTTON_RADIUS, Scalar(COLOR_WHITE), -1);
		active_rButtons[group] = rb;
		circle(interfaz, rButtons[group*N_RBUTTONS + rb], R_BUTTON_ACTIVE_RADIUS, Scalar(COLOR_BLACK), -1);
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

// STATS GUI

/* INICIALIZA LA INTERFAZ GRÁFICA PARA LAS ESTADÍSTICAS */
void GUI::initStatsGUI() {
	statsInterface = Mat(Size(300,400), CV_8UC3);		// Creamos la matriz en la que crearemos la interfaz
	statsInterface.setTo(Scalar(GUI_COLOR), Mat());		// Establecemos el color de fondo

	/* RADIO BUTTON 0 - TEAM */
	rButtonsSts.push_back(Point(10, 25));
	rButtonsSts.push_back(Point(300/2 + 10, 25));

	circle(statsInterface, rButtonsSts[0], R_BUTTON_RADIUS, Scalar(COLOR_WHITE), -1);
	putText(statsInterface, "Equipo 1", Point(35, 30), CV_FONT_HERSHEY_PLAIN, 1, Scalar(COLOR_WHITE));
	circle(statsInterface, rButtonsSts[1], R_BUTTON_RADIUS, Scalar(COLOR_WHITE), -1);
	putText(statsInterface, "Equipo 2", Point(300/2 + 10 + 25, 30), CV_FONT_HERSHEY_PLAIN, 1, Scalar(COLOR_WHITE));
	
	circle(statsInterface, Point(10, 25), R_BUTTON_ACTIVE_RADIUS, Scalar(COLOR_BLACK),-1);
	active_rButtonsSts.push_back(0);

	/* RADIO BUTTON 1 - PLAYER (HIDDEN) */
	rectangle(statsInterface, Point(1,50), Point(150, 70+25*N_PLAYERS+5),Scalar(GUI_AUX_COLOR), 1);
	rectangle(statsInterface, Point(300-1,50), Point(150, 70+25*N_PLAYERS+5),Scalar(GUI_AUX_COLOR), 1);

	for(int i=0; i<N_RBUTTON_SGROUPS; i++) {
		for(int j=0; j<N_PLAYERS ; j++) {
			std::stringstream ss;
			if(j==0) {
				ss << "Portero";
			} else {
				ss << "Jugador" << j;
			}
			rButtonsSts.push_back(Point(10 + 150*i, 75+25*j));
			circle(statsInterface, rButtonsSts[(i*N_PLAYERS + j)+2], R_BUTTON_RADIUS, Scalar(COLOR_WHITE), -1);
			putText(statsInterface, ss.str(), Point(10 + 150*i + 25, 75+25*j + 5), CV_FONT_HERSHEY_PLAIN, 1, Scalar(COLOR_WHITE));
		}
		circle(statsInterface, Point(10 + 150*i, 75+25*(N_PLAYERS-1)), 4, Scalar(COLOR_BLACK),-1);
		active_rButtonsSts.push_back(N_PLAYERS-1);
	}
	// Marcamos los radio buttons activos
	int rButton1 = active_rButtonsSts[1];
	int rButton2 = active_rButtonsSts[2];
	circle(statsInterface, Point(10 , 75+25*rButton1), R_BUTTON_ACTIVE_RADIUS, Scalar(COLOR_BLACK),-1);
	circle(statsInterface, Point(10 + 150, 75+25*rButton2), R_BUTTON_ACTIVE_RADIUS, Scalar(COLOR_BLACK),-1);

	hidePlayers();
	
	/* CHECK BOXES */
	checkBoxesSts.push_back(Rect(Point(5,375),Point(5+10,375+10)));
	checkBoxesSts.push_back(Rect(Point(5+150,375),Point(5+150+10,375+10)));

	activeCheckBoxesSts.push_back(false);
	activeCheckBoxesSts.push_back(false);
	
	rectangle(statsInterface,checkBoxesSts[0],Scalar(COLOR_WHITE),-1);
	rectangle(statsInterface,checkBoxesSts[1],Scalar(COLOR_WHITE),-1);

	switchCheckBox(1);

	putText(statsInterface, "Individual", Point(5+20, 375+10), CV_FONT_HERSHEY_PLAIN, 1, Scalar(COLOR_WHITE));
	putText(statsInterface, "Comparativa", Point(5+150+20, 375+10), CV_FONT_HERSHEY_PLAIN, 1, Scalar(COLOR_WHITE));

}

/* MUESTRA LA INTERFAZ GRÁFICA PARA LAS ESTADÍSTICAS */
void GUI::showStatsGUI() {
	imshow(SGUI_W, statsInterface);
}

/* MUESTRA LAS ESTADÍSTICAS SELECCIONADAS */
void GUI::showStatsWindow(int individualMode, int comparativeMode) {
	Mat stats;
	if(individualMode==0) {
		if(comparativeMode==0) {	// 0,0
			int team = getActiveTeam();
			stats = Mat(Size(250,300), CV_8UC3);
			stats.setTo(Scalar(GUI_COLOR));
			line(stats,Point(0,25),Point(250,25),Scalar(GUI_AUX_COLOR),2);
			std::stringstream ssname;
			ssname << "Equipo " << team+1;
			std::stringstream ssdistance;
			ssdistance << GlobalStats::teams[team].getDistance() << " m";
			putText(stats,"Distancia:",Point(10,75),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);
			putText(stats,"Posesión:",Point(10,100),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);
			putText(stats,"Pases:",Point(10,125),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);

			putText(stats,ssname.str(),Point(100,20),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);
			putText(stats,ssdistance.str(),Point(100,75),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);
			putText(stats,"xx %",Point(100,100),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);
			putText(stats,"xx",Point(100,125),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);
		} else {					// 0,1
			stats = Mat(Size(250*2,300), CV_8UC3);
			stats.setTo(Scalar(GUI_COLOR));
			line(stats,Point(0,25),Point(250*2,25),Scalar(GUI_AUX_COLOR),2);
			line(stats,Point(250,0),Point(250,300),Scalar(GUI_AUX_COLOR),1);
			for(int i=0; i<2; i++) {
				std::stringstream ssname;
				ssname << "Equipo " << i+1;
				std::stringstream ssdistance;
				ssdistance << GlobalStats::teams[i].getDistance() << " m";
				putText(stats,"Distancia:",Point(10+250*i,75),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);
				putText(stats,"Posesión:",Point(10+250*i,100),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);
				putText(stats,"Pases:",Point(10+250*i,125),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);

				putText(stats,ssname.str(),Point(100+250*i,20),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);
				putText(stats,ssdistance.str(),Point(100+250*i,75),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);
				putText(stats,"xx %",Point(100+250*i,100),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);
				putText(stats,"xx",Point(100+250*i,125),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);
			}
		}
	} else {
		// AÑADIR ESTADÍSTICAS DE POSICIÓN 2D 
		if(comparativeMode==0) {	// 1,0
			stats = Mat(Size(250,300), CV_8UC3);
			stats.setTo(Scalar(GUI_COLOR));
			line(stats,Point(0,25),Point(250,25),Scalar(GUI_AUX_COLOR),2);

			int team = getActiveTeam();
			int player = getActivePlayer(team+1);

			std::stringstream ssteam;
			ssteam << "Equipo " << team+1;
			std::stringstream ssplayer;
			if(player==0) {
				ssplayer << "Portero ";
			} else {
				ssplayer << "Jugador " << player;
			}
			std::stringstream ssdistance;
			ssdistance << GlobalStats::teams[team].getPlayers()[player].getDistance() << " m";

			putText(stats,"Nombre:",Point(10,20),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);
			putText(stats,"Equipo:",Point(10,75),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);
			putText(stats,"Distancia:",Point(10,100),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);
			putText(stats,"Pases:",Point(10,125),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);

			putText(stats,ssplayer.str(),Point(100,20),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);
			putText(stats,ssteam.str(),Point(100,75),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);
			putText(stats,ssdistance.str(),Point(100,100),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);
			putText(stats,"xx",Point(100,125),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);

			Mat aux = stats(Rect(5,160,240,136));
			soccer_field.copyTo(aux);

		} else {					// 1,1
			stats = Mat(Size(250*2,300), CV_8UC3);
			stats.setTo(Scalar(GUI_COLOR));
			line(stats,Point(0,25),Point(250*2,25),Scalar(GUI_AUX_COLOR),2);
			line(stats,Point(250,0),Point(250,300),Scalar(GUI_AUX_COLOR),1);
			for(int i=0; i<2; i++) {
				int player = getActivePlayer(i+1);

				std::stringstream ssteam;
				ssteam << "Equipo " << i+1;
				std::stringstream ssplayer;
				if(player==0) {
					ssplayer << "Portero ";
				} else {
					ssplayer << "Jugador " << player;
				}
				std::stringstream ssdistance;
				ssdistance << GlobalStats::teams[i].getPlayers()[player].getDistance() << " m";

				putText(stats,"Nombre:",Point(10+250*i,20),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);
				putText(stats,"Equipo:",Point(10+250*i,75),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);
				putText(stats,"Distancia:",Point(10+250*i,100),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);
				putText(stats,"Pases:",Point(10+250*i,125),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);

				putText(stats,ssplayer.str(),Point(100+250*i,20),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);
				putText(stats,ssteam.str(),Point(100+250*i,75),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);
				putText(stats,ssdistance.str(),Point(100+250*i,100),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);
				putText(stats,"xx",Point(100+250*i,125),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);

				Mat aux = stats(Rect(5+250*i,160,240,136));
				soccer_field.copyTo(aux);
			}
		}
	}
	imshow(STATS_W,stats);
}

/* CAMBIA EL ESTADO DEL CHECKBOX */
void GUI::switchCheckBox(int i) {
	Rect aux = checkBoxesSts[i];
	if(activeCheckBoxesSts[i]) {
		rectangle(statsInterface,aux,Scalar(COLOR_WHITE),-1);
		if(i==0) {
			hidePlayers();
		} else {
			if(activeCheckBoxesSts[0]) {
				hidePlayers();
				unhidePlayers(active_rButtonsSts[0]+1);
			}
		}
	} else {
		line(statsInterface, aux.tl(), aux.br(), Scalar(COLOR_BLACK),1);
		line(statsInterface, aux.tl()+Point(0,aux.height), aux.br()-Point(0,aux.height), Scalar(COLOR_BLACK),1);

		if(i==0) {
			if(activeCheckBoxesSts[1]) {
				unhidePlayers(0);
			} else {
				unhidePlayers(active_rButtonsSts[0]+1);
			}
		} else {
			if(activeCheckBoxesSts[0]) {
				unhidePlayers(0);
			}
		}
	}
	activeCheckBoxesSts[i] = !activeCheckBoxesSts[i];
}

/* ACTIVA EL RADIO BUTTON CORRESPONDIENTE */
void GUI::activateRadioButtonSts(int rb, int group) {
	int i = active_rButtonsSts[group];
	if(i!=rb) { // Si rb no es el radio button activo, lo activamos y desactivamos el radio button i
		int offset = 0;

		active_rButtonsSts[group] = rb;
		if(group != 0) {
			offset = 2+N_PLAYERS*(group-1);
		} else if(isActiveIndividualMode() && !isActiveComparativeMode()) {
			hidePlayers();
			unhidePlayers(active_rButtonsSts[group]+1);
		}
		circle(statsInterface, rButtonsSts[offset + i], R_BUTTON_RADIUS, Scalar(COLOR_WHITE), -1);
		circle(statsInterface, rButtonsSts[offset + rb], R_BUTTON_ACTIVE_RADIUS, Scalar(COLOR_BLACK), -1);
	}
}

/* DETERMINA SI ESTÁ ACTIVADO EL CHECKBOX 0 */
bool GUI::isActiveIndividualMode() {
	return activeCheckBoxesSts[0];
}

/* DETERMINA SI ESTÁ ACTIVADO EL CHECKBOX 1 */
bool GUI::isActiveComparativeMode() {
	return activeCheckBoxesSts[1];
}

/* FUNCIÓN QUE DEVUELVE EL EQUIPO SELECCIONADO */
int GUI::getActiveTeam() {
	return active_rButtonsSts[0];
}

/* FUNCIÓN QUE DEVUELVE EL JUGADOR SELECCIONADO */
int GUI::getActivePlayer(int team) {
	return active_rButtonsSts[team];
}

/* FUNCIÓN QUE OCULTA ELEMENTOS EN LA INTERFAZ */
void GUI::hidePlayers() {
	rectangle(statsInterface, Point(2,51), Point(300-2, 70+25*N_PLAYERS+5-1),Scalar(75,75,75), -1);
}

/* FUNCIÓN QUE MUESTRA ELEMENTOS EN LA INTERFAZ */
void GUI::unhidePlayers(int rButton) {
	if(rButton==0) {
		for(int i=0; i<N_RBUTTON_SGROUPS; i++) {
			for(int j=0; j<N_PLAYERS ; j++) {
				std::stringstream ss;
				if(j==0) {
					ss << "Portero";
				} else {
					ss << "Jugador" << j;
				}
				circle(statsInterface, rButtonsSts[(i*N_PLAYERS + j)+2], R_BUTTON_RADIUS, Scalar(COLOR_WHITE), -1);
				putText(statsInterface, ss.str(), Point(10 + 150*i + 25, 75+25*j + 5), CV_FONT_HERSHEY_PLAIN, 1, Scalar(COLOR_WHITE));
			}
		}
		// Marcamos los radio buttons activos
		int rButton1 = active_rButtonsSts[1];
		int rButton2 = active_rButtonsSts[2];
		circle(statsInterface, Point(10 , 75+25*rButton1), R_BUTTON_ACTIVE_RADIUS, Scalar(COLOR_BLACK),-1);
		circle(statsInterface, Point(10 + 150, 75+25*rButton2), R_BUTTON_ACTIVE_RADIUS, Scalar(COLOR_BLACK),-1);
	} else if(rButton==1) {
		for(int i=0; i<N_RBUTTON_SGROUPS-1; i++) {
			for(int j=0; j<N_PLAYERS ; j++) {
				std::stringstream ss;
				if(j==0) {
					ss << "Portero";
				} else {
					ss << "Jugador" << j;
				}
				circle(statsInterface, rButtonsSts[(i*N_PLAYERS + j)+2], R_BUTTON_RADIUS, Scalar(COLOR_WHITE), -1);
				putText(statsInterface, ss.str(), Point(10 + 150*i + 25, 75+25*j + 5), CV_FONT_HERSHEY_PLAIN, 1, Scalar(COLOR_WHITE));
			}
		}
		// Marcamos los radio buttons activos
		int rButton1 = active_rButtonsSts[1];
		circle(statsInterface, Point(10 , 75+25*rButton1), R_BUTTON_ACTIVE_RADIUS, Scalar(COLOR_BLACK),-1);
	} else if(rButton==2) {
		for(int i=1; i<N_RBUTTON_SGROUPS; i++) {
			for(int j=0; j<N_PLAYERS ; j++) {
				std::stringstream ss;
				if(j==0) {
					ss << "Portero";
				} else {
					ss << "Jugador" << j;
				}
				circle(statsInterface, rButtonsSts[(i*N_PLAYERS + j)+2], R_BUTTON_RADIUS, Scalar(COLOR_WHITE), -1);
				putText(statsInterface, ss.str(), Point(10 + 150*i + 25, 75+25*j + 5), CV_FONT_HERSHEY_PLAIN, 1, Scalar(COLOR_WHITE));
			}
		}
		// Marcamos los radio buttons activos
		int rButton2 = active_rButtonsSts[2];
		circle(statsInterface, Point(10 + 150, 75+25*rButton2), R_BUTTON_ACTIVE_RADIUS, Scalar(COLOR_BLACK),-1);
	}
}
