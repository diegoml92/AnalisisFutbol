#include "GlobalStats.h"
#include "StatsAnalyzer.h"

// Iniciamos las variables
Team GlobalStats::teams[N_TEAMS] = {Team(), Team()};
vector<Point2f> GlobalStats::locations [N_VIDEOS];
vector<Point> GlobalStats::detectedPlayers;
vector<int> GlobalStats::playersToDelete;

/* VACÍA LOS VECTORES DE POSICIONES */
void GlobalStats::clearLocations() {
	for(int i=0; i<N_VIDEOS; i++) {
		GlobalStats::locations[i].clear();
	}
}

/* AÑADE LAS ESTADÍSTICAS ACTUALES */
void GlobalStats::addStats() {
	for(int i=0; i<N_TEAMS; i++) {
		for(int j=0; j<N_PLAYERS; j++) {
			Player p = teams[i].getPlayers()[j];
			//Point loc = p.getAssociatedLocation();
			//if(loc != null) {
				//p.addPosition(loc);
			//}
		}
	}
}

bool GlobalStats::alreadyDetected(Point p) {
	vector<Point>::iterator it = detectedPlayers.begin();
	bool found = false;
	while(!found && it!=detectedPlayers.end()) {
		found = StatsAnalyzer::isSamePoint(p,*it);
		it++;
	}
	return found;
}
