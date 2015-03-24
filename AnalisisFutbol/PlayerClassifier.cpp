#include "PlayerClassifier.h"
#include "GUI.h"
#include "TrackingObj.h"
#include "From3DTo2D.h"
#include "GlobalStats.h"
#include "Team.h"

/* AÑADE UN JUGADOR */
void PlayerClassifier::addPlayer(Mat partido, Mat filtro, Point player) {
	vector<Rect> playerV;
	for(int i=0; i<N_VIDEOS; i++) {
		Point realPos = From3DTo2D::getRealPosition(player,i);
		if(TrackingObj::isInFocus(realPos)) {
			Rect playerBox = Rect((realPos).x-PLAYER_WIDTH/2,(realPos).y-PLAYER_HEIGHT,PLAYER_WIDTH,PLAYER_HEIGHT);
			if(TrackingObj::isInRange(&playerBox) && isPlayerSize(playerBox) && canBePlayer(filtro(playerBox))) {
				playerV.push_back(playerBox);
			}
		}
	}
	if(playerV.size()>0) {
		comparePlayer(partido,filtro,playerV,player);	// Hacemos la comparación
	}
}

/*	
*	CLASIFICA LOS ELEMENTOS DETECTADOS:
*	Compara el histograma de los jugadores y los clasifica por equipos
*/
void PlayerClassifier::comparePlayer(Mat partido, Mat umbral, vector<Rect> rects, Point pos) {

	int channels [] = {0,1,2};
	int nBins = 256;
	float range [] = {0,256};
	const float *ranges = {range};
	vector<Mat> hist_v; 
	for(int i=0; i<rects.size(); i++) {
		Mat hist, src = partido(rects.at(i));
		calcHist(&src,1,channels,umbral(rects[i]),hist,1,&nBins,&ranges);
		hist_v.push_back(hist);
	}
	/******************************************/
	Mat finalHist = hist_v[0];//CALCULAR HISTOGRAMA MEDIO!!!
	if(GlobalStats::teams.empty()) {
		Team t = Team();
		t.setHistogram(finalHist);
		t.createPlayer(pos);
		GlobalStats::teams.push_back(t);
	} else {
		bool found = false;
		vector<Team>::iterator it = GlobalStats::teams.begin();
		while(it!=GlobalStats::teams.end() && !found) {
			found = compareHist(it->getHistogram(),finalHist,CV_COMP_BHATTACHARYYA) < BHATTACHARYYA_THRES;
			it++;
		}
		if(found) {
			it--;
			it->createPlayer(pos);
		} else {
			Team t = Team();
			t.setHistogram(finalHist);
			t.createPlayer(pos);
			GlobalStats::teams.push_back(t);
		}
	}
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
