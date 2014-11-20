#include "Player.h"
#include "StatsAnalyzer.h"

vector<int> Player::id;

/* CONSTRUCTOR */
Player::Player(int team_id) {
	Player::team_id = team_id;
	Player::player_id = Player::id.at(team_id)++;
	Player::lastPoint = Point2i(-1,-1);
}

/* INCREMENTA EL VALOR EN LA POSICION INDICADA */
void Player::addPosition(int x, int y) {
	StatsAnalyzer::addPosition(Player::area, x, y);
}

/* INCREMENTA LA DISTANCIA RECORRIDA */
void Player::addDistance(Point2i actualPoint) {
	StatsAnalyzer::addDistance(Player::distance,actualPoint,Player::lastPoint);
}

/* CALCULA LAS ESTADÍSTICAS */
void Player::calculateStats() {
	Player::area_stats = StatsAnalyzer::getAreaStats(Player::area);
}