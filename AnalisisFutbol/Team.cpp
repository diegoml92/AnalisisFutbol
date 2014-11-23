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
	Team::area = Mat::zeros((VIDEO_HEIGHT+8)/ANALYZER_VIDEO_SIZE_RELATION,
		                    VIDEO_WIDTH/ANALYZER_VIDEO_SIZE_RELATION, CV_32SC1);
}

/* CALCULA LAS ESTADÍSTICAS */
void Team::calculateStats() {
	for (int i=0; i<N_PLAYERS; i++) {
		Player p = Team::players[i];
		Team::area += Team::players[i].getArea();
		Team::distance += Team::players[i].getDistance();
		Team::players[i].calculateStats();
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
