#include "Player.h"
#include "StatsAnalyzer.h"
#include "From3DTo2D.h"
#include "VideoManager.h"

int Player::id = 0;

/* CONSTRUCTOR */
Player::Player(Point pos, vector<Mat> histogram) {
	this->player_id = Player::id++;
	this->histogram = histogram;
	this->distance = 0;
	this->avgSpeed = 0;
	this->maxSpeed = 0;
	this->area = Mat::zeros(SOCCER_FIELD_HEIGHT/ANALYZER_VIDEO_SIZE_RELATION,
		                      SOCCER_FIELD_WIDTH/ANALYZER_VIDEO_SIZE_RELATION, CV_32SC1);
	for(int i=0; i<N_VIDEOS; i++) {
		this->bPos[i] = false;
	}
	this->lastPoint = pos;
	this->nSpeed = 0;
	this->deletionCounter = 0;
	this->retries = 0;
}

/* CONSTRUCTOR AUXILIAR */
Player::Player(Player* p) {
	this->player_id = p->id;
	this->histogram = p->histogram;
	this->distance = p->distance;
	this->avgSpeed = p->avgSpeed;
	this->maxSpeed = p->maxSpeed;
	this->area = p->area;
	this->lastPoint = p->lastPoint;
	this->nSpeed = p->nSpeed;
	for(int i=0; i<N_VIDEOS; i++) {
		this->bPos[i] = false;
	}
	this->deletionCounter = p->deletionCounter;
	this->retries = p->retries;
}

/* INCREMENTA EL VALOR EN LA POSICION INDICADA */
void Player::addPosition(Point p) {
	if(From3DTo2D::isInRange(p)) {
		StatsAnalyzer::addPosition(this->area, p);
		if(VideoManager::getActualFrame() % (int)SAMPLING_RATE == 0) {
			StatsAnalyzer::addDistanceAndSpeed(&(this->distance),p,&(this->lastPoint),
				&(this->avgSpeed),&(this->nSpeed),&(this->maxSpeed),&(this->retries));
		}
	}
}

/* ACTUALIZA ESTAD�STICAS SI SE RECUPERA EL JUGADOR */
void Player::updateStats(Point pos) {
	StatsAnalyzer::updateStats(&(this->distance),pos,&(this->lastPoint),&(this->avgSpeed),
		&(this->nSpeed),&(this->maxSpeed),&(this->retries),this->deletionCounter);
}

/* CALCULA LAS ESTAD�STICAS */
void Player::calculateStats() {
	this->area_stats = StatsAnalyzer::getAreaStats(this->area);
	this->avgSpeed /= this->nSpeed;
}

/* DEVUELVE LA �LTIMA POSICI�N TRACKEADA (SI LA HAY) */
Point Player::getPosition() {
	Point position = this->lastPoint;
	vector<Point> positions;
	for(int i=0; i<N_VIDEOS; i++) {
		if(this->getBPos(i)) {
			positions.push_back(From3DTo2D::get2DPosition(this->getCamPos(i),i));
		}
	}
	if(!positions.empty()) {
		position = Point(0,0);
		for(int i=0; i<positions.size(); i++) {
			position += positions[i];
		}
		position = Point(position.x/positions.size(), position.y/positions.size());
	}
	return position;
}

/* DEVUELVE LAS ESTAD�SITCAS DEL �REA RECORRIDA */
Mat Player::getAreaStats() {
	return this->area_stats;
}

/* DEVUELVE LA DISTANCIA RECORRIDA */
float Player::getDistance() {
	return this->distance;
}

/* DEVUELVE LA VELOCIDAD M�XIMA */
float Player::getMaxSpeed() {
	return this->maxSpeed;
}

/* DEVUELVE LA VELOCIDAD MEDIA */
float Player::getAvgSpeed() {
	return this->avgSpeed;
}

/* DEVUELVE EL PUNTO ACUTAL */
Point Player::getLastPosition() {
	return this->lastPoint;
}

/* DEVUELVE EL HISTOGRAMA DEL JUGADOR */
vector<Mat> Player::getHistogram() {
	return this->histogram;
}

/* DEVUELVE EL PLAYER ID */
int Player::getPlayerId() {
	return this->player_id;
}

/* DEVUELVE EL VALOR i */
bool Player::getBPos(int i) {
	return this->bPos[i];
}

/* DEVUELVE EL VALOR i */
Point Player::getCamPos(int i) {
	return this->camPos[i];
}

/* ACTIVA LA C�MARA i */
void Player::setCamPos(int i, Point p) {
	this->bPos[i] = true;
	this->camPos[i] = p;
}

/* DESACTIVA LA C�MARA i */
void Player::unSetCamPos(int i) {
	this->bPos[i] = false;
}

/* DEVUELVE EL �LTIMO DESPLAZAMIENTO EN LA C�MARA i */
Vec2i Player::getShift(int i) {
	return this->shift[i];
}

/* ESTABLECE EL DESPLAZAMIENTO */
void Player::setShift(Point shift, int i) {
	this->shift[i] = Vec2i(shift);
}

/* INICIA EL CONTADOR DE BORRADO A 0 */
void Player::startDeletionCounter() {
	this->deletionCounter = 0;
}

/* INCREMENTA EL CONTADOR DE BORRADO */
void Player::increaseDeletionCounter() {
	this->deletionCounter++;
}

/* DEVUELVE EL VALOR DEL CONTADOR DE BORRADO */
int Player::getDeletionCounter() {
	return this->deletionCounter;
}

bool Player::operator==(const Player other) const {
	return player_id == other.player_id;
}