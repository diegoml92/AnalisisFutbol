#include "FieldFilter.h"

/* FILTRAMOS EL CAMPO PARA ENCONTRAR A LOS JUGADORES */
Mat FieldFilter::discardField(Mat partido, Mat bg) {

	/*const int C = -10;
	const int MASK_SIZE = 15;

	Mat umbral;
	//inRange(partido, Scalar(GUI::MIN_B, GUI::MIN_G, GUI::MIN_R), Scalar(GUI::MAX_B, GUI::MAX_G, GUI::MAX_R), umbral);

	cvtColor(partido,partido,CV_BGR2HSV);
	blur(partido,partido,Size(3,3));
	Mat umbral0,umbral1,umbral2;
	vector<Mat> planos;
	split(partido,planos);
	adaptiveThreshold(planos[0],umbral0,MAX_RGB,ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY,MASK_SIZE,C);
	adaptiveThreshold(planos[1],umbral1,MAX_RGB,ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY_INV,MASK_SIZE,C);
	adaptiveThreshold(planos[2],umbral2,MAX_RGB,ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY,MASK_SIZE,C);
	umbral = (umbral0 | umbral2) & umbral1 ;*/

	Mat thres, final, diff = abs(partido-bg);
	threshold(diff,thres,40,255,CV_THRESH_BINARY);
	//pyrDown(thres,thres,Size(thres.cols/2,thres.rows/2));
	//imshow("DIFFERENCES",thres);


	vector<Mat> planes;
	split(thres,planes);
	final = (planes[0] | planes[1] | planes[2]);
	//final = final > 0;

	/*Mat morfElement = getStructuringElement(MORPH_RECT, Size(7,15));
	morphologyEx(final,final,MORPH_CLOSE,morfElement);*/

	//pyrDown(umbral,umbral,Size(umbral.cols/2,umbral.rows/2));
	//pyrDown(umbral0,umbral0,Size(umbral0.cols/2,umbral0.rows/2));
	//pyrDown(umbral1,umbral1,Size(umbral1.cols/2,umbral1.rows/2));
	//pyrDown(umbral2,umbral2,Size(umbral2.cols/2,umbral2.rows/2));
	//imshow("Adaptive threshold", umbral);
	//imshow("AdaptiveThresh0", umbral0);
	//imshow("AdaptiveThresh1", umbral1);
	//imshow("AdaptiveThresh2", umbral2);
	//waitKey(0);

	return final;
}