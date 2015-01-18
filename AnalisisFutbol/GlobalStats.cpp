#include "GlobalStats.h"

// Iniciamos las variables
Team GlobalStats::teams[N_TEAMS] = {Team(), Team()};
Ball GlobalStats::ball = Ball();
vector<Point2f> GlobalStats::locations [N_VIDEOS];

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
