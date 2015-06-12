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
		vector<Mat> hist = calculateHistogram(frame, filter, playerV, position);
		GlobalStats::playerV.push_back(Player(position, hist));
	}
}

/* CALCULA EL HISTOGRAMA DEL JUGADOR */
vector<Mat> PlayerClassifier::calculateHistogram(Mat frame, Mat filter, vector<Rect> rects, Point pos) {

	int channel_B [] = {0};
	int channel_G [] = {1};
	int channel_R [] = {2};
	int nBins = 16;
	float range [] = {0,256};
	const float *ranges = {range};
	vector<Mat> hist_v;
	Mat hist[3];
	for(int i=0; i<rects.size(); i++) {
		Mat mask = filter(rects[i]);
		Mat src = frame(rects.at(i));
		calcHist(&src,1,channel_B,mask,hist[0],1,&nBins,&ranges,true,true);
		calcHist(&src,1,channel_G,mask,hist[1],1,&nBins,&ranges,true,true);
		calcHist(&src,1,channel_R,mask,hist[2],1,&nBins,&ranges,true,true);
	}
	for(int i=0 ; i<3; i++) {
		hist_v.push_back(hist[i]);
	}
	return hist_v;
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
