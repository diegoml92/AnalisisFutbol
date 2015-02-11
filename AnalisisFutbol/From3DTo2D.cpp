#include "From3DTo2D.h"

Mat From3DTo2D::projectionMatrix [N_VIDEOS];
Mat From3DTo2D::inverseProjectionMatrix [N_VIDEOS];
Mat From3DTo2D::field2D = imread(FIELD2D_PATH);

/* INICIAMOS LAS MATRICES DE PROYECCION */
void From3DTo2D::initProjectionMatrices() {
	for(int i=0; i<N_VIDEOS; i++) {
		switch(i) {
			case 0: {
				// SEQ0 POINTS
				Point2f real [4] = {Point2f(282-DESP, 518), Point2f(603-DESP, 517), Point2f(459-DESP, 299), Point2f(720-DESP, 298)};
				//A-B-C-D
				Point2f model [4] = {Point2f(0, 0+LONG), Point2f(55, 0+LONG), Point2f(0, 183.2+LONG), Point2f(55, 183.2+LONG)};
				// Calculamos la matriz de proyeccion 0
				projectionMatrix[i] = getPerspectiveTransform(real,model);
				inverseProjectionMatrix[i] = getPerspectiveTransform(model,real);

				break;
			}
			case 1: {
				// SEQ1 POINTS
				Point2f real [4] = {Point2f(946, 516), Point2f(473, 395), Point2f(950, 297), Point2f(1432, 395)};
				//P-Q-R-S
				Point2f model [4] = {Point2f(0+600, 356-91.5), Point2f(91.5+600, 356+0), Point2f(0+600, 356+91.5), Point2f(600-91.5, 356+0)};
				// Calculamos la matriz de proyeccion 1
				projectionMatrix[i] = getPerspectiveTransform(real,model);
				inverseProjectionMatrix[i] = getPerspectiveTransform(model,real);

				break;
			}
			case 2: {
				// SEQ2 POINTS
				Point2f real [4] = {Point2f(1645, 521), Point2f(1969, 521), Point2f(1525, 300), Point2f(1790, 300)};
				//W-X-Y-Z
				Point2f model [4] = {Point2f(1200-55, 0+LONG), Point2f(1200, 0+LONG), Point2f(1200-55, 183.2+LONG), Point2f(1200, 183.2+LONG)};
				// Calculamos la matriz de proyeccion 2
				projectionMatrix[i] = getPerspectiveTransform(real,model);
				inverseProjectionMatrix[i] = getPerspectiveTransform(model,real);

				break;
			}
			case 3: {
				// SEQ3 POINTS
				Point2f real [4] = {Point2f(470-DESP,299), Point2f(739-DESP, 299), Point2f(290-DESP, 523), Point2f(617-DESP,523)};
				//A-B-C-D
				Point2f model [4] = {Point2f(0, 0+LONG), Point2f(55, 0+LONG), Point2f(0, 183.2+LONG), Point2f(55, 183.2+LONG)};
				// Calculamos la matriz de proyeccion 3
				projectionMatrix[i] = getPerspectiveTransform(real,model);
				inverseProjectionMatrix[i] = getPerspectiveTransform(model,real);

				break;
			}
			case 4: {
				// SEQ4 POINTS
				Point2f real [4] = {Point2f(943, 300), Point2f(1427, 398), Point2f(944, 516), Point2f(456, 398)};
				// P-Q-R-S
				Point2f model [4] = {Point2f(0+600, 356-91.5), Point2f(91.5+600, 356+0), Point2f(0+600, 356+91.5), Point2f(600-91.5, 356+0)};
				// Calculamos la matriz de proyeccion 4
				projectionMatrix[i] = getPerspectiveTransform(real,model);
				inverseProjectionMatrix[i] = getPerspectiveTransform(model,real);

				break;
			}
			case 5: {
				// SEQ5 POINTS
				Point2f real [4] = {Point2f(1533, 304), Point2f(1797, 304), Point2f(1647, 525), Point2f(1962, 525)};
				//W-X-Y-Z
				Point2f model [4] = {Point2f(1200-55, 0+LONG), Point2f(1200, 0+LONG), Point2f(1200-55, 183.2+LONG), Point2f(1200, 183.2+LONG)};
				// Calculamos la matriz de proyeccion 5
				projectionMatrix[i] = getPerspectiveTransform(real,model);
				inverseProjectionMatrix[i] = getPerspectiveTransform(model,real);

				break;
			}
		}
	}
}

/* CONVERTIMOS COORDENADAS DEL MUNDO A POSICIONES 2D */
Point2f From3DTo2D::get2DPosition(Point2f p, int nVideo) {
	vector<Point2f> pv;
	pv.push_back(p);
	perspectiveTransform(pv,pv,projectionMatrix[nVideo]);
	return pv.at(0);
}

/* CONVERTIMOS COORDENADAS DEL MUNDO A POSICIONES 2D */
vector<Point2f> From3DTo2D::get2DPositionVector(vector<Point2f> p, int nVideo) {
	perspectiveTransform(p,p,projectionMatrix[nVideo]);
	return p;
}

/* CONVERTIMOS COORDENADAS DEL MODELO AL COORDENADAS REALES */
Point From3DTo2D::getRealPosition(Point modelPos, int nVideo) {
	vector<Point2f> pv;
        pv.push_back(p);
        perspectiveTransform(pv,pv,inverseProjectionMatrix[nVideo]);
        return pv.at(0);
}

/* PINTAMOS LAS POSICIONES EN EL PLANO 2D */
void From3DTo2D::paint2DPositions(Rect player, int nVideo, Mat paint) {
	Point center = player.br();
	center.x -= (player.width/2);
	vector<Point2f> point_array;
	point_array.push_back(center);
	perspectiveTransform(point_array,point_array, projectionMatrix[nVideo]);
	// DEBUG!!
	Scalar color;
	int radius;
	int thickness;
	switch(nVideo) {
		case 0:
			radius = 7;
			thickness = 2;
			color = Scalar(0,128,255);
			break;
		case 1:
			radius = 7;
			thickness = 2;
			color = Scalar(255,50,255);
			break;
		case 2:
			radius = 7;
			thickness = 2;
			color = Scalar(0,255,0);
			break;
		case 3:
			radius = 5;
			thickness = -1;
			color = Scalar(255,0,0);
			break;
		case 4:
			radius = 5;
			thickness = -1;
			color = Scalar(0,255,255);
			break;
		case 5:
			radius = 5;
			thickness = -1;
			color = Scalar(0,0,255);
			break;
	}
	circle(paint,point_array.at(0),radius,color,thickness);
}

/* PINTAMOS LAS POSICIONES EN EL PLANO 2D */
void From3DTo2D::paint2DPositions2(Point player, int nVideo, Mat paint) {
	// DEBUG!!
	Scalar color;
	int radius=3;
	int thickness=1;
	if(nVideo<=2) {
		color = Scalar(COLOR_WHITE);
	} else {
		color = Scalar(COLOR_BLACK);
	}
	circle(paint,player,radius,color,thickness);
}
