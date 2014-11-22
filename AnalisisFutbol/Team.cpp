#include "Team.h"
#include "StatsAnalyzer.h"

int Team::id = 0;

/* CONSTRUCTOR */
Team::Team(void) {
	Team::team_id = id++;
	Team::distance = 0;
	Player::id.push_back(0);
	for(int i=0; i<N_PLAYERS; i++) {
		Team::players[i] = Player(Team::team_id);
	}
}

/* CALCULA LAS ESTADÍSTICAS */
void Team::calculateStats() {
	for (int i=0; i<N_PLAYERS; i++) {
		Player p = Team::players[i];
		p.calculateStats();
		Team::area += p.getArea();
		Team::distance += p.getDistance();
	}
	Team::area_stats = StatsAnalyzer::getAreaStats(area);
}

/* DEVUELVE LA LISTA DE JUGADORES */
Player* Team::getPlayers() {
	return Team::players;
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
