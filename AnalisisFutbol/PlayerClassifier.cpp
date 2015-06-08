#include "PlayerClassifier.h"
#include "GUI.h"
#include "TrackingObj.h"
#include "From3DTo2D.h"
#include "GlobalStats.h"

/* AÑADE UN JUGADOR */
void PlayerClassifier::addPlayer(Mat frame, Mat filter, Point position) {
	vector<Rect> playerV;
	for(int i=0; i<N_VIDEOS; i++) {
		Point realPos = From3DTo2D::getRealPosition(position,i);
		if(TrackingObj::isInFocus(realPos)) {
			Rect playerBox = Rect((realPos).x-PLAYER_WIDTH/2,(realPos).y-PLAYER_HEIGHT,PLAYER_WIDTH,PLAYER_HEIGHT);
			if(TrackingObj::isInRange(&playerBox) && isPlayerSize(playerBox) && canBePlayer(filter(playerBox))) {
				playerV.push_back(playerBox);
			}
		}
	}
	if(playerV.size()>0) {
		Mat hist = comparePlayer(frame, filter, playerV, position);
		GlobalStats::playerV.push_back(Player(position, hist));
	}
}

/*	
*	CLASIFICA LOS ELEMENTOS DETECTADOS:
*	Compara el histograma de los jugadores y los clasifica por equipos
*/
Mat PlayerClassifier::comparePlayer(Mat frame, Mat filter, vector<Rect> rects, Point pos) {

	int channels [] = {0,1,2};
	int nBins = 32;
	float range [] = {0,256};
	const float *ranges = {range};
	Mat hist;
	for(int i=0; i<rects.size(); i++) {
		Mat src = frame(rects.at(i));
		calcHist(&src,1,channels,filter(rects[i]),hist,1,&nBins,&ranges,true,true);
	}
	return hist;
}

/* DETERMINA SI CUMPLE EL TAMAÑO PROPIO DE UN JUGADOR */
bool PlayerClassifier::isPlayerSize(Rect player) {
	return player.height>MIN_HEIGH && player.height<MAX_HEIGH
			&& player.width>MIN_WIDTH && player.width<MAX_WIDTH;
}

/* DESCARTA FALSOS POSITIVOS (LÍNEAS, ETC) EN FUNCIÓN DE EL NÚMERO DE PÍXELES BLANCOS */
bool PlayerClassifier::canBePlayer(Mat roi) {
	return countNonZero(roi) / (float)(roi.cols*roi.rows) > 0.35;
}

/* DESCARTA FALSOS POSITIVOS (LÍNEAS, ETC) EN FUNCIÓN DE EL NÚMERO DE PÍXELES BLANCOS */
bool PlayerClassifier::canBePlayer2(Mat roi) {
	return countNonZero(roi) / (float)(roi.cols*roi.rows) > 0.2;
}
