#include "PlayerClassifier.h"
#include "GUI.h"
#include "TrackingObj.h"
#include "From3DTo2D.h"
#include "GlobalStats.h"
#include "StatsAnalyzer.h"

vector<Player> PlayerClassifier::playerV;
std::list<Player> PlayerClassifier::playersToDelete;

/* DETERMINA SI EL PUNTO ESTÁ CERCA DEL BORDE DEL CAMPO */
bool PlayerClassifier::isFieldEdge(Point p) {
	return p.x < 20  || p.x > SOCCER_FIELD_WIDTH - 20;
}

/* AÑADE UN JUGADOR */
void PlayerClassifier::addPlayer(Mat frame, Mat filter, Point position) {
	vector<Rect> rects;
	// Descartamos si está demasiado cerca del borde del campo para evitar ruido
	if(!PlayerClassifier::isFieldEdge(position)) {
		for(int i=0; i<N_VIDEOS; i++) {
			Point realPos = From3DTo2D::getCameraPosition(position,i);
			if(TrackingObj::isInFocus(realPos)) {
				Rect playerBox = Rect((realPos).x-PLAYER_WIDTH/2,(realPos).y-PLAYER_HEIGHT,PLAYER_WIDTH,PLAYER_HEIGHT);
				if(TrackingObj::isInRange(&playerBox) && isPlayerSize(playerBox) && canBePlayer(filter(playerBox))) {
					rects.push_back(playerBox);
				}
			}
		}
		if(rects.size()>0) {
			vector<Mat> hist = calculateHistogram(frame, filter, rects);
			// No se ha asociado a ningún jugador perdido anteriormente
			if(!PlayerClassifier::recoverPlayer(position,hist)) {
				PlayerClassifier::playerV.push_back(Player(position, hist));
			}
		}
	}
}

/* DETERMINA SI UN ELEMENTO YA HA SIDO DETECTADO */
bool PlayerClassifier::alreadyDetected(Point p) {
	bool found = false;
	int i=0;
	vector<Player>::iterator it = playerV.begin();
	while(!found && it!=playerV.end()) {
		found = StatsAnalyzer::isSamePoint(p,it->getPosition());
		it++;
	}
	return found;
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

/* REALIZA EL SEGUIMIENTO DE LOS ELEMENTOS DEL PARTIDO */
void PlayerClassifier::objectDetection() {
	for(int i=0; i<N_VIDEOS; i++) {
		Mat temp;		// Matriz auxiliar en la que buscaremos los contornos
		GlobalStats::filter[i].copyTo(temp);
		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE);
		if(hierarchy.size() > 0) {									// Si se encuentra algún contorno
			for(int j = 0; j < contours.size(); j++ ) {				// Recorremos los contornos
				Rect elem = boundingRect(Mat(contours[j]));			// Creamos el boundingBox
				if(PlayerClassifier::isPlayerSize(elem) &&
					PlayerClassifier::canBePlayer(GlobalStats::filter[i](elem))) {
					GlobalStats::locations[i].push_back(elem);		// Añadimos al vector de elementos encontrados
				}
			}
		}
	}
}

/*
* AÑADE UN JUGADOR A LA LISTA DE BORRADO 
* Y LO BORRA DE LA LISTA DE JUGADORES ACTUALES
*/
void PlayerClassifier::addPlayerToDelete(vector<Player>::iterator* it) {
	Player player(**it);
	player.startDeletionCounter();
	PlayerClassifier::playersToDelete.push_back(player);
	*it = PlayerClassifier::playerV.erase(*it);
}

/* COMPRUEBA LOS JUGADORES A BORRAR */
void PlayerClassifier::checkPlayersToDelete() {
	std::list<Player>::iterator it = playersToDelete.begin();
	while (it!=playersToDelete.end()) {
		it->increaseDeletionCounter();
		if(it->getDeletionCounter() > TIME_TO_DELETE * FPS) {
			it = PlayerClassifier::playersToDelete.erase(it);
		} else {
			it++;
		}
	}
}

/* INTENTA ASOCIAR UNA NUEVA DETECCIÓN A ALGÚN JUGADOR PERDIDO */
bool PlayerClassifier::recoverPlayer(Point pos, vector<Mat> hist) {
	float min_dist = 5+2.5*TIME_TO_DELETE;
	Player* found = NULL;
	for(std::list<Player>::iterator it = PlayerClassifier::playersToDelete.begin();
			it != PlayerClassifier::playersToDelete.end(); it++) {
		float dist;
		if(StatsAnalyzer::isInRecoverRange(pos,it->getPosition(),it->getDeletionCounter(),&dist) &&
				dist < min_dist &&
				PlayerClassifier::compareHistogram(hist,it->getHistogram()) <= 0.4*N_CHANNELS) {
			found = &(*it);
			min_dist = dist;
		}
	}
	if(found!=NULL) {
		found->updateStats(pos);
		PlayerClassifier::playerV.push_back(*found);
		std::list<Player>::iterator it =
			std::find(PlayerClassifier::playersToDelete.begin(),PlayerClassifier::playersToDelete.end(),*found);
		if(it != PlayerClassifier::playersToDelete.end()) {
			PlayerClassifier::playersToDelete.erase(it);
		}
	}
	return found != NULL;
}
