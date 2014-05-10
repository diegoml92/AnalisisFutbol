#include "FieldFilter.h"
#include "GUI.h"

/* FILTRAMOS EL CAMPO PARA ENCONTRAR A LOS JUGADORES */
Mat FieldFilter::discardField(Mat partido) {
	Mat umbral;
	//inRange(partido, Scalar(GUI::MIN_B, GUI::MIN_G, GUI::MIN_R), Scalar(GUI::MAX_B, GUI::MAX_G, GUI::MAX_R), umbral);

	Mat umbral0,umbral1,umbral2;
	vector<Mat> planos;
	split(partido,planos);
	adaptiveThreshold(planos[0],umbral0,MAX_RGB,ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY,45,-15);
	adaptiveThreshold(planos[1],umbral1,MAX_RGB,ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY,45,-15);
	adaptiveThreshold(planos[2],umbral2,MAX_RGB,ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY,45,-15);
	umbral = umbral0 | umbral1 | umbral2;

	Mat morfElement = getStructuringElement(MORPH_RECT, Size(9,9));
	morphologyEx(umbral,umbral,MORPH_CLOSE,morfElement);

	//imshow("Adaptive threshold", umbral);
	//imshow("AdaptiveThresh0", umbral0);
	//imshow("AdaptiveThresh1", umbral1);
	//imshow("AdaptiveThresh2", umbral2);
	//waitKey(0);

	return umbral;
}