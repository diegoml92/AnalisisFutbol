#include "FieldFilter.h"
#include "GUI.h"

/* FILTRAMOS EL CAMPO PARA ENCONTRAR A LOS JUGADORES */
Mat FieldFilter::discardField(Mat partido) {
	Mat umbral;
	inRange(partido, Scalar(GUI::MIN_B, GUI::MIN_G, GUI::MIN_R), Scalar(GUI::MAX_B, GUI::MAX_G, GUI::MAX_R), umbral);
	return umbral;
}