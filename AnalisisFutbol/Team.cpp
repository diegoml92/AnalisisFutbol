#include "Team.h"
#include "StatsAnalyzer.h"

int Team::id = 0;

/* CONSTRUCTOR */
Team::Team(void) {
	Team::team_id = id++;
	Player::id.push_back(0);
}

/* CALCULA LAS ESTADÍSTICAS */
void Team::calculateStats() {
	for (vector<Player>::iterator it = Team::players.begin() ; it != Team::players.end(); ++it) {
		Player p = *it;
		p.calculateStats();
		Team::area += p.getArea();
		Team::distance += p.getDistance();
	}
	Team::area_stats = StatsAnalyzer::getAreaStats(area);
}

/* DEVUELVE LA MATRIZ DE POSICIONES ACUMULADAS */
Mat Team::getArea() {
	return Team::area;
}

/* DEVUELVE LAS ESTADÍSITCAS DEL ÁREA RECORRIDA */
Mat Team::getAreaStats() {
	return Team::area_stats;
}

/* DEVUELVE LA DISTANCIA RECORRIDA */
float Team::getDistance() {
	return Team::distance;
}
