#include "From3DTo2D.h"
#include "GlobalStats.h"

Mat From3DTo2D::projectionMatrix [N_VIDEOS];
Mat From3DTo2D::inverseProjectionMatrix [N_VIDEOS];

/* DETERMINA SI EL PUNTO PERTENECE A UNA POSICIÓN VÁLIDA */
bool From3DTo2D::isInRange(Point p) {
	return p.x >= 0 && p.y >= 0 && p.x < SOCCER_FIELD_WIDTH && p.y < SOCCER_FIELD_HEIGHT;
}

/* INICIAMOS LAS MATRICES DE PROYECCION */
void From3DTo2D::initProjectionMatrices() {
	for(int i=0; i<N_VIDEOS; i++) {
		switch(i) {
			case 0: {
				// SEQ0 POINTS
				Point2f real [4] = {Point2f(16,136), Point2f(280,183), Point2f(203,459), Point2f(24,231)};
				//A-B-C-D
				Point2f model [4] = {Point2f(0,0), Point2f(AREA_WIDTH-1,SOCCER_FIELD_HEIGHT/2-AREA_HEIGHT/2-1),
					Point2f(AREA_WIDTH-1,SOCCER_FIELD_HEIGHT/2+AREA_HEIGHT/2-1),
					Point2f(PENALTY_AREA_WIDTH-1,SOCCER_FIELD_HEIGHT/2-PENALTY_AREA_HEIGHT/2-1)};
				// Calculamos la matriz de proyeccion 0
				projectionMatrix[i] = getPerspectiveTransform(real,model);
				inverseProjectionMatrix[i] = getPerspectiveTransform(model,real);

				break;
			}
			case 1: {
				int SOCCER_FIELD_WIDTH_2 = SOCCER_FIELD_WIDTH / 2;
				// SEQ1 POINTS
				Point2f real [4] = {Point2f(470,517), Point2f(478,49), Point2f(235,202), Point2f(717, 201)};
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
				Point2f real [4] = {Point2f(814,50), Point2f(509,354), Point2f(822,261), Point2f(495,102)};
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
				Point2f real [4] = {Point2f(139,46), Point2f(448,355), Point2f(130,262), Point2f(467,99)};
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
				Point2f real [4] = {Point2f(472,512), Point2f(471,49), Point2f(228,198), Point2f(713,198)};
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
				Point2f real [4] = {Point2f(514,354), Point2f(825,264), Point2f(500,103), Point2f(821,52)};
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

/* CONVERTIMOS COORDENADAS DE LA CÁMARA A POSICIONES 2D */
Point2f From3DTo2D::get2DPosition(Point2f p, int nVideo) {
	vector<Point2f> pv;
	pv.push_back(p);
	perspectiveTransform(pv,pv,projectionMatrix[nVideo]);
	return pv.at(0);
}

/* CONVERTIMOS COORDENADAS DE LA CÁMARA A POSICIONES 2D */
vector<Point2f> From3DTo2D::get2DPositionVector(vector<Point2f> p, int nVideo) {
	perspectiveTransform(p,p,projectionMatrix[nVideo]);
	return p;
}

/* CONVERTIMOS COORDENADAS DEL 2D A COORDENADAS DE LA CÁMARA */
Point From3DTo2D::getCameraPosition(Point modelPos, int nVideo) {
	vector<Point2f> pv;
        pv.push_back(modelPos);
        perspectiveTransform(pv,pv,inverseProjectionMatrix[nVideo]);
        return pv.at(0);
}

/* CONVIERTE LA LOCALIZACIONES DE EN LA CÁMARA A SUS EQUIVALENTES 2D*/
void From3DTo2D::calculateLocations2D() {
	for(int i=0; i<N_VIDEOS; i++) {
		// Obtiene la posición del bounding box en la cámara
		for(int j=0; j<GlobalStats::locations[i].size(); j++) {
			GlobalStats::locations2D[i].push_back(GlobalStats::getCenter(GlobalStats::locations[i].at(j)));
		}

		// Transformamos a posición 2D
		if(GlobalStats::locations[i].size()) {
			GlobalStats::locations2D[i] = From3DTo2D::get2DPositionVector(GlobalStats::locations2D[i],i);
		}

	}
}