#include "Ball.h"
#include "StatsAnalyzer.h"

/* CONSTRUCTOR */
Ball::Ball(void) {
	Ball::lastPoint = Point3i(-1,-1,-1);
}

/* INCREMENTA EL VALOR EN LA POSICION INDICADA */
void Ball::addPosition(int x, int y) {
	StatsAnalyzer::addPosition(Ball::area, x, y);
}

/* INCREMENTA LA DISTANCIA RECORRIDA */
void Ball::addDistance(Point3i actualPoint) {
	StatsAnalyzer::addDistance(Ball::distance,actualPoint,Ball::lastPoint);
}

/* ANADE EL VALOR DE LA ALTURA */
void Ball::addHeight(float h) {
	StatsAnalyzer::addHeight(h, height);
}

/* CALCULA LAS ESTADÍSTICAS */
void Ball::calculateStats() {
	Ball::area_stats = StatsAnalyzer::getAreaStats(Ball::area);
}