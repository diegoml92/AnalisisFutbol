#include "Player.h"
#include "StatsAnalyzer.h"
#include "From3DTo2D.h"
#include "VideoManager.h"

int Player::id = 0;

/* CONSTRUCTOR */
Player::Player(Point pos, vector<Mat> histogram) {
	Player::player_id = Player::id++;
	Player::histogram = histogram;
	Player::distance = 0;
	Player::avgSpeed = 0;
	Player::maxSpeed = 0;
	Player::area = Mat::zeros(SOCCER_FIELD_HEIGHT/ANALYZER_VIDEO_SIZE_RELATION,
		                      SOCCER_FIELD_WIDTH/ANALYZER_VIDEO_SIZE_RELATION, CV_32SC1);
	for(int i=0; i<N_VIDEOS; i++) {
		Player::bPos[i] = false;
	}
	Player::lastPoint = pos;
	Player::nSpeed = 0;
}

/* INCREMENTA EL VALOR EN LA POSICION INDICADA */
void Player::addPosition(Point p) {
	if(From3DTo2D::isInRange(p)) {
		StatsAnalyzer::addPosition(Player::area, p);
		if(VideoManager::getActualFrame() % 5 == 0) {
			StatsAnalyzer::addDistanceAndSpeed(&(Player::distance),p,Player::lastPoint,
				&(Player::avgSpeed),&(Player::nSpeed),&(Player::maxSpeed));
			Player::lastPoint = p;
		}
	}
}

/* CALCULA LAS ESTADÍSTICAS */
void Player::calculateStats() {
	Player::area_stats = StatsAnalyzer::getAreaStats(Player::area);
	Player::avgSpeed /= Player::nSpeed;
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

/* DEVUELVE EL HISTOGRAMA DEL JUGADOR */
vector<Mat> Player::getHistogram() {
	return Player::histogram;
}

/* DEVUELVE EL PLAYER ID */
int Player::getPlayerId() {
	return Player::player_id;
}

/* DEVUELVE EL VALOR i */
bool Player::getBPos(int i) {
	return Player::bPos[i];
}

/* DEVUELVE EL VALOR i */
Point Player::getCamPos(int i) {
	return Player::camPos[i];
}

/* ACTIVA LA CÁMARA i */
void Player::setCamPos(int i, Point p) {
	Player::bPos[i] = true;
	Player::camPos[i] = p;
}

/* DESACTIVA LA CÁMARA i */
void Player::unSetCamPos(int i) {
	Player::bPos[i] = false;
}

bool Player::operator==(const Player &other) const {
	return this == &other;
}