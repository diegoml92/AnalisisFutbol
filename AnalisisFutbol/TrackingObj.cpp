#include "GUI.h"
#include "TrackingObj.h"
#include "BallDetector.h"
#include "PlayerClassifier.h"
#include "From3DTo2D.h"
#include "GlobalStats.h"

/* LLEVA A CABO EL SEGUIMIENTO DE LOS JUGADORES  */
void TrackingObj::tracking(Point pos) {
	float rango[] = {0,255};
        const float* ranges [] = {rango};
        int channels [] = {0};
        int histSize [] = {128};

        Rect playerBox = Rect(Point(pos.x-PLAYER_WIDTH/2,pos.y-PLAYER_HEIGHT),PLAYER_WIDTH,PLAYER_HEIGHT);

        Mat hsv_roi, roi = frame(playerBox);
        cvtColor(roi,hsv_roi,COLOR_BGR2HSV);

        Mat thres, diff = abs(frame-bg);
        threshold(diff,thres,40,255,CV_THRESH_BINARY);
        vector<Mat> planes;
        split(thres,planes);
        Mat filter;
        filter = (planes[0] | planes[1] | planes[2]);

        Mat mask = filter(playerBox);
        Mat roi_hist;
        Mat images [] = {hsv_roi};

        calcHist(&hsv_roi,1,channels,mask,roi_hist,1,histSize,ranges);
        normalize(roi_hist,roi_hist,0,255,NORM_MINMAX);

	TermCriteria term_crit(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 5, 1);

        //int team_id = 0;
        //Player player(team_id);
}


/* TRACKING DE LOS JUGADORES */
void TrackingObj::trackPlayers(Point detectedPlayer) {
	bool detected;
	Point newPos;
	for(int i=0; i<N_VIDEOS; i++) {
		Point realPos = From3DTo2D::getRealPosition(detectedPlayer,i);
		if(isInRange(realPos)) {
			tracking(realPos);
			bool detected = true;
		}
	}
}

/* REALIZA EL SEGUIMIENTO DE LOS ELEMENTOS DEL PARTIDO */
void TrackingObj::objectDetection(Mat filtro, Mat &partido, int nVideo, Mat paint) {
	Mat temp;		// Matriz auxiliar en la que buscaremos los contornos
	filtro.copyTo(temp);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE);
	if(hierarchy.size() > 0) {													// Si se encuentra algún contorno
		PlayerClassifier::clearVectors();										// Limpiamos los vectores de clasificación
		BallDetector::clear();													// Limpiamos el vector de balones

		for( int i = 0; i < contours.size(); i++ ) {							// Recorremos los contornos
			Rect elem = boundingRect(Mat(contours[i]));							// Creamos el boundingBox
			if(PlayerClassifier::isPlayerSize(elem)) {							// Si cumple las restricciones de tamaño...
				PlayerClassifier::addPlayer(partido, filtro, elem);				// ...añade el jugador y lo clasifica
				Point playerPos = Point(elem.br().x-elem.width/2, elem.br().y); // Calculamos el punto en el que está el jugador
				From3DTo2D::get2DPosition(playerPos, nVideo);					// Transformamos a posición 2D
				GlobalStats::locations[nVideo].push_back(playerPos);			// Añadimos al vector de elementos encontrados
				From3DTo2D::paint2DPositions(elem, nVideo, paint);				// Representamos las posiciones en el plano 2D
			} else if(BallDetector::isBallSize(elem)) {							// Si tiene tamaño y forma de balón...
				BallDetector::addBall(elem);									// ...se añade
			}
		}

		PlayerClassifier::sortVectors();										// Ordenamos los vectores de clasificación
		PlayerClassifier::drawTeams(partido);									// Dibujamos la etiqueta de cada elemento
		BallDetector::selectBall(partido, filtro);								// Elige el balón entre los candidatos
		BallDetector::drawBall(partido);										// Dibujamos el balón
	}
}
