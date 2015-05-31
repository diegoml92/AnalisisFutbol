#include "GlobalStats.h"
#include "StatsAnalyzer.h"

// Iniciamos las variables
bool GlobalStats::playersDetected = false;
vector<Team> GlobalStats::teams;
vector<Rect> GlobalStats::locations [N_VIDEOS];
vector<int> GlobalStats::playersToDelete;
vector<Scalar> GlobalStats::colors;

/* VACÍA LOS VECTORES DE POSICIONES */
void GlobalStats::clearLocations() {
	for(int i=0; i<N_VIDEOS; i++) {
		GlobalStats::locations[i].clear();
	}
}

bool GlobalStats::alreadyDetected(Point p) {
	bool found = false;
	int i=0;
	while(!found && i<teams.size()) {
		vector<Player> players = teams.at(i).getPlayers();
		vector<Player>::iterator it = players.begin();
		while(!found && it!=players.end()) {
			found = StatsAnalyzer::isSamePoint(p,it->getPosition());
			it++;
		}
		i++;
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
	return GlobalStats::totalPlayers() > 35;
}

int GlobalStats::totalPlayers() {
	int n=0;
	for(int i=0;i<teams.size();i++) {
		n+=teams[i].getPlayers().size();
	}
	return n;
}