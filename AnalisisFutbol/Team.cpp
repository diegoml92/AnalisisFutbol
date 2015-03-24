#include "Team.h"
#include "StatsAnalyzer.h"

int Team::id = 0;

/* CONSTRUCTOR */
Team::Team() {
	Team::team_id = id++;
	Team::distance = 0;
	Player::id.push_back(id);
	Team::area = Mat::zeros((VIDEO_HEIGHT+8)/ANALYZER_VIDEO_SIZE_RELATION,
		                    VIDEO_WIDTH/ANALYZER_VIDEO_SIZE_RELATION, CV_32SC1);
}

/* CALCULA LAS ESTADÍSTICAS */
void Team::calculateStats() {
	for (int i=0; i<players.size() && i<N_PLAYERS; i++) {
		Player p = Team::players[i];
		Team::area += Team::players[i].getArea();
		Team::distance += Team::players[i].getDistance();
		Team::players[i].calculateStats();
	}
	Team::area_stats = StatsAnalyzer::getAreaStats(area);
}

/* DEVUELVE EL IDENTIFICADOR DEL EQUIPO */
int Team::getTeamId() {
	return team_id;
}

/* INICIALIZA EL HISTOGRAMA */
void Team::setHistogram(Mat hist) {
	Team::histogram = hist;
}
	
/* DEVUELVE EL HISTOGRAMA */
Mat Team::getHistogram() {
	return histogram;
}

/* DEVUELVE LA LISTA DE JUGADORES */
vector<Player> Team::getPlayers() {
	return Team::players;
}

/* DEVUELVE UN PUNTERO A LA LISTA DE JUGADORES */
vector<Player>* Team::getPlayersP() {
	return &(Team::players);
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

/* AÑADE UN NUEVO JUGADOR */
void Team::createPlayer(Point init) {
	players.push_back(Player(team_id,init));
}
