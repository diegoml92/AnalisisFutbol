#include "gui.h"

// Valores que marca en rango de filtro del césped
int gui::MIN_B = 5;
int gui::MAX_B = 90;
int gui::MIN_G = 75;
int gui::MAX_G = 135;
int gui::MIN_R = 35;
int gui::MAX_R = 110;

// Valores para el rango de tamaño de los jugadores
int gui::MIN_WIDTH = 10;
int gui::MIN_HEIGH = 25;
int gui::MAX_WIDTH = 45;
int gui::MAX_HEIGH = 80;

void gui::on_trackbar(int, void*) {}

void gui::fieldThresTrackbars() {
	// Creamos las trackbars
	createTrackbar("MIN_B", GUI_W, &MIN_B, MAX_RGB, on_trackbar);
	createTrackbar("MAX_B", GUI_W, &MAX_B, MAX_RGB, on_trackbar);
	createTrackbar("MIN_G", GUI_W, &MIN_G, MAX_RGB, on_trackbar);
	createTrackbar("MAX_G", GUI_W, &MAX_G, MAX_RGB, on_trackbar);
	createTrackbar("MIN_R", GUI_W, &MIN_R, MAX_RGB, on_trackbar);
	createTrackbar("MAX_R", GUI_W, &MAX_R, MAX_RGB, on_trackbar);
}

void gui::playerSizeTrackbars() {
	// Creamos las trackbars
	createTrackbar("Ancho MIN", GUI_W, &MIN_WIDTH, 100, on_trackbar);
	createTrackbar("Ancho MAX", GUI_W, &MAX_WIDTH, 100, on_trackbar);
	createTrackbar("Alto MIN", GUI_W, &MIN_HEIGH, 100, on_trackbar);
	createTrackbar("Alto MAX", GUI_W, &MAX_HEIGH, 100, on_trackbar);
}