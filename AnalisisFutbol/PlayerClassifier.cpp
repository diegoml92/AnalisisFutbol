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
		vector<Mat> hist = calculateHistogram(frame, filter, playerV);
		// No se ha asociado a ningún jugador perdido anteriormente
		if(!GlobalStats::recoverPlayer(position,hist)) {
			GlobalStats::playerV.push_back(Player(position, hist));
		}
	}
}

/* CALCULA EL HISTOGRAMA DEL JUGADOR */
vector<Mat> PlayerClassifier::calculateHistogram(Mat frame, Mat filter, vector<Rect> rects) {

	int channel_B [] = {0};
	int channel_G [] = {1};
	int channel_R [] = {2};
	int nBins = N_BINS;
	float range [] = {0,RGB};
	const float *ranges = {range};
	vector<Mat> hist_v;
	Mat hist[N_CHANNELS];
	for(int i=0; i<rects.size(); i++) {
		Mat mask = filter(rects[i]);
		Mat src = frame(rects.at(i));
		calcHist(&src,1,channel_B,mask,hist[0],1,&nBins,&ranges,true,true);
		calcHist(&src,1,channel_G,mask,hist[1],1,&nBins,&ranges,true,true);
		calcHist(&src,1,channel_R,mask,hist[2],1,&nBins,&ranges,true,true);
	}
	for(int i=0 ; i<N_CHANNELS; i++) {
		hist_v.push_back(hist[i]);
	}
	return hist_v;
}

/* CALCULA EL HISTOGRAMA DEL JUGADOR */
vector<Mat> PlayerClassifier::calculateHistogram(Point pos, int nCam) {
	vector<Rect> v;
	v.push_back(GlobalStats::getPlayerRect(pos));
	return PlayerClassifier::calculateHistogram(GlobalStats::frame[nCam],GlobalStats::filter[nCam],v);
}

/* COMPARA LOS HISTOGRAMAS DE LOS JUGADORES */
double PlayerClassifier::compareHistogram(vector<Mat> playerHist, vector<Mat> newHist) {
	double diff = 0;
	for(int i=0; i<N_CHANNELS; i++) {
		diff += compareHist(playerHist.at(i),newHist.at(i),CV_COMP_BHATTACHARYYA);
	}
	return diff;
}

/* DETERMINA SI CUMPLE EL TAMAÑO PROPIO DE UN JUGADOR */
bool PlayerClassifier::isPlayerSize(Rect player) {
	return player.height>MIN_HEIGH && player.height<MAX_HEIGH
			&& player.width>MIN_WIDTH && player.width<MAX_WIDTH;
}

/* DETERMINA CUÁL DE LOS ELEMENTOS ASIGNADOS A UN JUGADOR ES EL CORRECTO */
Point PlayerClassifier::findBestMatch(Player* player) {
	float min = N_CHANNELS; // La distancia bhattacharya es mámixo 1; Como trabajamos con
							// cada canal por separado, el máximo será 3.
	int cam = -1;
	for(int i=0; i<N_VIDEOS; i++) {
		if (player->getBPos(i)) {
			float res = PlayerClassifier::compareHistogram(player->getHistogram(),
				calculateHistogram(player->getCamPos(i), i));
			if(res < min) {
				min = res;
				cam = i;
			}
		}
	}
	// Desactivamos todas las localizaciones excepto la de la mejor opcion
	for(int i=0; i<N_VIDEOS; i++) {
		if (player->getBPos(i) && i!=cam) {
			player->unSetCamPos(i);
		}
	}
	return From3DTo2D::get2DPosition(player->getCamPos(cam),cam);
}

/* DESCARTA FALSOS POSITIVOS (LÍNEAS, ETC) EN FUNCIÓN DE EL NÚMERO DE PÍXELES BLANCOS */
bool PlayerClassifier::canBePlayer(Mat roi, float val) {
	return countNonZero(roi) / (float)(roi.cols*roi.rows) >= val;
}
