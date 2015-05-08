#include "From3DTo2D.h"

Mat From3DTo2D::projectionMatrix [N_VIDEOS];
Mat From3DTo2D::inverseProjectionMatrix [N_VIDEOS];
Mat From3DTo2D::field2D = imread(FIELD2D_PATH);

/* DETERMINA SI EL PUNTO PERTENECE A UNA POSICIÓN VÁLIDA */
bool From3DTo2D::isInRange(Point p) {
	return p.x >= 0 && p.y >= 0 && p.x <= SOCCER_FIELD_WIDTH && p.y <= SOCCER_FIELD_HEIGHT;
}

/* INICIAMOS LAS MATRICES DE PROYECCION */
void From3DTo2D::initProjectionMatrices() {
	for(int i=0; i<N_VIDEOS; i++) {
		switch(i) {
			case 0: {
				// SEQ0 POINTS
				Point2f real [4] = {Point2f(262,520), Point2f(280,106), Point2f(912,202), Point2f(892,698)};
				//A-B-C-D
				Point2f model [4] = {Point2f(PENALTY_AREA_WIDTH-1,SOCCER_FIELD_HEIGHT/2-PENALTY_AREA_HEIGHT/2-1),
					Point2f(0,SOCCER_FIELD_HEIGHT-1), Point2f(AREA_WIDTH-1,SOCCER_FIELD_HEIGHT/2+AREA_HEIGHT/2-1),
					Point2f(AREA_WIDTH-1,SOCCER_FIELD_HEIGHT/2-AREA_HEIGHT/2-1)};
				// Calculamos la matriz de proyeccion 0
				projectionMatrix[i] = getPerspectiveTransform(real,model);
				inverseProjectionMatrix[i] = getPerspectiveTransform(model,real);

				break;
			}
			case 1: {
				int SOCCER_FIELD_WIDTH_2 = SOCCER_FIELD_WIDTH / 2;
				// SEQ1 POINTS
				Point2f real [4] = {Point2f(940,1034), Point2f(956,98), Point2f(470,404), Point2f(1434, 402)};
				//P-Q-R-S
				Point2f model [4] = {Point2f(SOCCER_FIELD_WIDTH/2-1,0), Point2f(SOCCER_FIELD_WIDTH/2-1,SOCCER_FIELD_HEIGHT-1),
					Point2f(SOCCER_FIELD_WIDTH/2+CIRCLE_RADIUS-1,SOCCER_FIELD_HEIGHT/2-1), Point2f(SOCCER_FIELD_WIDTH/2-CIRCLE_RADIUS-1,SOCCER_FIELD_HEIGHT/2-1)};
				// Calculamos la matriz de proyeccion 1
				projectionMatrix[i] = getPerspectiveTransform(real,model);
				inverseProjectionMatrix[i] = getPerspectiveTransform(model,real);

				break;
			}
			case 2: {
				// SEQ2 POINTS
				Point2f real [4] = {Point2f(1628,100), Point2f(1018,708), Point2f(1644,522), Point2f(990,204)};
				//W-X-Y-Z
				Point2f model [4] = {Point2f(SOCCER_FIELD_WIDTH-1,SOCCER_FIELD_HEIGHT-1),
					Point2f(SOCCER_FIELD_WIDTH-AREA_WIDTH-1,SOCCER_FIELD_HEIGHT/2-AREA_HEIGHT/2-1),
					Point2f(SOCCER_FIELD_WIDTH-PENALTY_AREA_WIDTH-1,SOCCER_FIELD_HEIGHT/2-PENALTY_AREA_HEIGHT/2-1),
					Point2f(SOCCER_FIELD_WIDTH-AREA_WIDTH-1,SOCCER_FIELD_HEIGHT/2+AREA_HEIGHT/2-1)};
				// Calculamos la matriz de proyeccion 2
				projectionMatrix[i] = getPerspectiveTransform(real,model);
				inverseProjectionMatrix[i] = getPerspectiveTransform(model,real);

				break;
			}
			case 3: {
				// SEQ3 POINTS
				Point2f real [4] = {Point2f(278,92), Point2f(896,710), Point2f(260,524), Point2f(934,198)};
				//A-B-C-D
				Point2f model [4] = {Point2f(0,0), Point2f(AREA_WIDTH-1,SOCCER_FIELD_HEIGHT/2+AREA_HEIGHT/2-1),
					Point2f(PENALTY_AREA_WIDTH-1,SOCCER_FIELD_HEIGHT/2+PENALTY_AREA_HEIGHT/2-1),
					Point2f(AREA_WIDTH-1,SOCCER_FIELD_HEIGHT/2-AREA_HEIGHT/2-1)};
				// Calculamos la matriz de proyeccion 3
				projectionMatrix[i] = getPerspectiveTransform(real,model);
				inverseProjectionMatrix[i] = getPerspectiveTransform(model,real);

				break;
			}
			case 4: {
				int SOCCER_FIELD_WIDTH_2 = SOCCER_FIELD_WIDTH / 2;
				// SEQ4 POINTS
				Point2f real [4] = {Point2f(944,1024), Point2f(942,98), Point2f(456,396), Point2f(1426,396)};
				// P-Q-R-S
				Point2f model [4] = {Point2f(SOCCER_FIELD_WIDTH/2-1,SOCCER_FIELD_HEIGHT-1), Point2f(SOCCER_FIELD_WIDTH/2-1,0),
					Point2f(SOCCER_FIELD_WIDTH/2-CIRCLE_RADIUS-1,SOCCER_FIELD_HEIGHT/2-1), Point2f(SOCCER_FIELD_WIDTH/2+CIRCLE_RADIUS-1,SOCCER_FIELD_HEIGHT/2-1)};
				// Calculamos la matriz de proyeccion 4
				projectionMatrix[i] = getPerspectiveTransform(real,model);
				inverseProjectionMatrix[i] = getPerspectiveTransform(model,real);

				break;
			}
			case 5: {
				// SEQ5 POINTS
				Point2f real [4] = {Point2f(1028,708), Point2f(1650,528), Point2f(1000,206), Point2f(1642,104)};
				//W-X-Y-Z
				Point2f model [4] = {Point2f(SOCCER_FIELD_WIDTH-AREA_WIDTH-1,SOCCER_FIELD_HEIGHT/2+AREA_HEIGHT/2-1),
					Point2f(SOCCER_FIELD_WIDTH-PENALTY_AREA_WIDTH-1,SOCCER_FIELD_HEIGHT/2+PENALTY_AREA_HEIGHT/2-1),
					Point2f(SOCCER_FIELD_WIDTH-AREA_WIDTH-1,SOCCER_FIELD_HEIGHT/2-AREA_HEIGHT/2-1), Point2f(SOCCER_FIELD_WIDTH-1,0)};
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
        pv.push_back(modelPos);
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
	if(nVideo<=0) {
		circle(paint,player,radius+2,Scalar(50,50,50),2);
	}
	if(nVideo<=2) {
		color = Scalar(COLOR_WHITE);
	} else {
		color = Scalar(COLOR_BLACK);
	}
	circle(paint,player,radius,color,thickness);
}
