#include "Player.h"
#include "StatsAnalyzer.h"

vector<int> Player::id;

/* CONSTRUCTOR */
Player::Player(int team_id, Point pos) {
	Player::team_id = team_id;
	Player::player_id = Player::id.at(team_id)++;
	Player::distance = 0;
	Player::avgSpeed = 0;
	Player::maxSpeed = 0;
	Player::area = Mat::zeros((VIDEO_HEIGHT+8)/ANALYZER_VIDEO_SIZE_RELATION,
		                      VIDEO_WIDTH/ANALYZER_VIDEO_SIZE_RELATION, CV_32SC1);
	Player::lastPoint = pos;
	Player::nSpeed = 0;
}

/* INCREMENTA EL VALOR EN LA POSICION INDICADA */
void Player::addPosition(Point p) {
	StatsAnalyzer::addPosition(Player::area, p);
	StatsAnalyzer::addDistanceAndSpeed(&(Player::distance),p,Player::lastPoint,
		&(Player::avgSpeed),&(Player::nSpeed),&(Player::maxSpeed));
	Player::lastPoint = p;
}

/* CALCULA LAS ESTADÍSTICAS */
void Player::calculateStats() {
	Player::area_stats = StatsAnalyzer::getAreaStats(Player::area);
	Player::avgSpeed /= Player::nSpeed;
}

/* DEVUELVE LA MATRIZ DE POSICIONES ACUMULADAS */
Mat Player::getArea() {
	return Player::area;
}

/* DEVUELVE LAS ESTADÍSITCAS DEL ÁREA RECORRIDA */
Mat Player::getAreaStats() {
	return Player::area_stats;
}

/* DEVUELVE LA DISTANCIA RECORRIDA */
float Player::getDistance() {
	return Player::distance;
}

/* DEVUELVE LA VELOCIDAD MÁXIMA */
float Player::getMaxSpeed() {
	return Player::maxSpeed;
}

/* DEVUELVE LA VELOCIDAD MEDIA */
float Player::getAvgSpeed() {
	return Player::avgSpeed;
}

/* DEVUELVE EL PUNTO ACUTAL */
Point Player::getPosition() {
	return Point(lastPoint.x,lastPoint.y);
}