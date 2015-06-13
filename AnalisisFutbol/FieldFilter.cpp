#include "FieldFilter.h"
#include "From3DTo2D.h"
#include "GlobalStats.h"

Mat FieldFilter::mask[N_VIDEOS];
Mat FieldFilter::bg[N_VIDEOS];

/* INICIA LOS ELEMENTOS NECESARIOS PARA EL FILTRADO */
void FieldFilter::initFilter() {
	for(int i=0; i<N_VIDEOS; i++) {
		std::stringstream path;
		path << VIDEO_PATH << i << BG_FORMAT;
		bg[i] = imread(path.str());

		FieldFilter::initFilterMask(i);
	}
}

/* FILTRAMOS EL CAMPO PARA ENCONTRAR A LOS JUGADORES */
void FieldFilter::discardField() {
	for (int i=0; i<N_VIDEOS; i++) {
		Mat thres, tmp, final, diff = abs(GlobalStats::frame[i]-bg[i]);
		threshold(diff,thres,40,255,CV_THRESH_BINARY);

		vector<Mat> planes;
		split(thres,planes);
		tmp = (planes[0] | planes[1] | planes[2]);
		tmp.copyTo(final,mask[i]);
		GlobalStats::filter[i]=final;
	}
}

/* SE INICIALIZA LA MÁSCARA PARA EL FILTRO */
void FieldFilter::initFilterMask(int nCam) {
	if(nCam!=2 && nCam!=5) {
		FieldFilter::mask[nCam] = Mat::zeros(Size(VIDEO_WIDTH,VIDEO_HEIGHT),CV_8UC1);
	} else {
		FieldFilter::mask[nCam] = Mat::zeros(Size(VIDEO_WIDTH,VIDEO_HEIGHT+4),CV_8UC1);
	}
	Point points [1][4];
	points [0][0] = From3DTo2D::getRealPosition(Point(0-FIELD_MARGIN,0-FIELD_MARGIN),nCam);
	points [0][1] = From3DTo2D::getRealPosition(Point(SOCCER_FIELD_WIDTH+FIELD_MARGIN,0-FIELD_MARGIN),nCam);
	points [0][2] = From3DTo2D::getRealPosition(Point(SOCCER_FIELD_WIDTH+FIELD_MARGIN,
		                                         SOCCER_FIELD_HEIGHT+FIELD_MARGIN),nCam);
	points [0][3] = From3DTo2D::getRealPosition(Point(0-FIELD_MARGIN,SOCCER_FIELD_HEIGHT+FIELD_MARGIN),nCam);
	const Point* ppt[1] = { points[0] };
	int npt[] = { 4 };
	fillPoly(mask[nCam],ppt,npt,1,Scalar(255));
}