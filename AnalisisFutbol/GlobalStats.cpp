#include "GlobalStats.h"
#include "StatsAnalyzer.h"

// Iniciamos las variables
Mat GlobalStats::frame[N_VIDEOS];
Mat GlobalStats::filter[N_VIDEOS];

vector<Player> GlobalStats::playerV;
vector<Rect> GlobalStats::locations [N_VIDEOS];
vector<Player*> GlobalStats::playersToDelete;

/* VACÍA LOS VECTORES DE POSICIONES */
void GlobalStats::clearLocations() {
	for(int i=0; i<N_VIDEOS; i++) {
		GlobalStats::locations[i].clear();
	}
}

/* DETERMINA SI UN ELEMENTO YA HA SIDO DETECTADO */
bool GlobalStats::alreadyDetected(Point p) {
	bool found = false;
	int i=0;
	vector<Player>::iterator it = playerV.begin();
	while(!found && it!=playerV.end()) {
		found = StatsAnalyzer::isSamePoint(p,it->getPosition());
		it++;
	}
	return found;
}

/* CALCULA EL CENTRO DE UN RECTÁNGULO */
Point GlobalStats::getCenter(Rect r) {
	return Point(r.tl().x + r.width/2, r.br().y);
}

/* CALCULA EL RECTÁNGULO A PARTIR DEL CENTRO */
Rect GlobalStats::getPlayerRect(Point pos) {
	return Rect(pos.x-PLAYER_WIDTH/2,pos.y-PLAYER_HEIGHT,PLAYER_WIDTH,PLAYER_HEIGHT);
}

/* INDICA SE ESTÁN TODOS LOS JUGADORES DETECTADOS */
bool GlobalStats::allPlayersDetected() {
	return GlobalStats::totalPlayers() >= MAX_PLAYERS;
}

/* DEVUELVE EL NÚMERO DE JUGADORES DETECTADOS */
int GlobalStats::totalPlayers() {
	return playerV.size();
}

/* DETERMINA EL "COLOR" DE UN JUGADOR A PARTIR DE SU HISTOGRAMA */
Scalar GlobalStats::calculateColor(vector<Mat> hist) {
	int bgr[3];
	for(int i=0; i<hist.size(); i++) {
		vector<int> total;
		int nBins = hist[i].rows;
		for(int j=0; j<nBins; j++) {
			for(int k=0; k<hist[i].at<float>(j);k++) {
				total.push_back(j);
			}
		}
		int half = total.size()/2;
		if(total.size()%2==0){
			int bin1 = total.at(half);
			int bin2 = total.at(half+1);
			bgr[i] = (RGB/nBins*bin1 + RGB/nBins*(bin2+1) - 1) / 2;
			
		} else {
			int bin = total.at(half);
			bgr[i] = (RGB/nBins*bin + RGB/nBins*(bin+1) - 1) / 2;
		}
	}
	return Scalar(bgr[0],bgr[1],bgr[2]);
}