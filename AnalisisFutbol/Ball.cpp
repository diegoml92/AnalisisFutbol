#include "Ball.h"
#include "StatsAnalyzer.h"

/* CONSTRUCTOR */
Ball::Ball(void) {
	Ball::distance = 0;
	for(int i=0; i<MAX_BALL_HEIGHT; i++) {
		Ball::height[i] = 0;
	}
	Ball::area = Mat::zeros((VIDEO_HEIGHT+8)/ANALYZER_VIDEO_SIZE_RELATION,
		                    VIDEO_WIDTH/ANALYZER_VIDEO_SIZE_RELATION, CV_32SC1);
	Ball::lastPoint = Point3i(-1,-1,-1);
}

/* INCREMENTA EL VALOR EN LA POSICION INDICADA */
void Ball::addPosition(Point3i p) {
	StatsAnalyzer::addPosition(Ball::area, Point(p.x,p.y));
	Ball::distance = StatsAnalyzer::addDistance(Ball::distance,p,Ball::lastPoint);
	Ball::lastPoint = p;
}

/* ANADE EL VALOR DE LA ALTURA */
void Ball::addHeight(float h) {
	StatsAnalyzer::addHeight(h, height);
}

/* CALCULA LAS ESTADÍSTICAS */
void Ball::calculateStats() {
	Ball::area_stats = StatsAnalyzer::getAreaStats(Ball::area);
}

/* DEVUELVE LA MATRIZ DE POSICIONES ACUMULADAS */
Mat Ball::getArea() {
	return Ball::area;
}

/* DEVUELVE LAS ESTADÍSITCAS DEL ÁREA RECORRIDA */
Mat Ball::getAreaStats() {
	return Ball::area_stats;
}

/* DEVUELVE LA DISTANCIA RECORRIDA */
float Ball::getDistance() {
	return Ball::distance;
}