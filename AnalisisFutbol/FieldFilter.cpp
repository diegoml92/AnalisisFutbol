#include "FieldFilter.h"

/* FILTRAMOS EL CAMPO PARA ENCONTRAR A LOS JUGADORES */
Mat FieldFilter::discardField(Mat partido, Mat bg) {

	Mat thres, final, diff = abs(partido-bg);
	threshold(diff,thres,40,255,CV_THRESH_BINARY);

	vector<Mat> planes;
	split(thres,planes);
	final = (planes[0] | planes[1] | planes[2]);

	return final;
}