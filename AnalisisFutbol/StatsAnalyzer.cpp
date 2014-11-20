#include "StatsAnalyzer.h"

/* INCREMENTA EL VALOR EN LA POSICIÓN INDICADA */
void StatsAnalyzer::addPosition(Mat m, int x, int y) {
	m.at<int>(y/ANALYZER_VIDEO_SIZE_RELATION,x/ANALYZER_VIDEO_SIZE_RELATION)++;
}

/* INCREMENTA LA DISTANCIA RECORRIDA */
void StatsAnalyzer::addDistance(float dist, Point3i actualPoint, Point3i lastPoint) {
	dist += distance(actualPoint, lastPoint);
}
void StatsAnalyzer::addDistance(float dist, Point2i actualPoint, Point2i lastPoint) {
	addDistance(dist,Point3i(actualPoint.x, actualPoint.y, 0),Point3i(lastPoint.x, lastPoint.y, 0));
}

/* CALCULA LA DISTANCIA ENTRE DOS PUNTOS */
float StatsAnalyzer::distance(Point3i actualPoint, Point3i lastPoint) {
	float dist = 0;
	if(lastPoint.x >= 0) {
		dist = norm(actualPoint-lastPoint);
	}
	lastPoint = actualPoint;
	return dist;
}

/* AÑADE EL VALOR DE LA ALTURA */
void StatsAnalyzer::addHeight(float h, float height[]) {
	int hh = h;
	if(hh < MAX_BALL_HEIGHT) {
		height[hh]++;
	} else {
		height[MAX_BALL_HEIGHT-1]++;
	}
}

/* DEVUELVE LAS ESTADÍSTICAS DE POSICIONES */
Mat StatsAnalyzer::getAreaStats(Mat m) {
	Mat normalized = normalizeAreaStats(m);
	Mat stats(normalized.rows, normalized.cols, CV_8UC3);
	for(int i=0;i<normalized.cols;i++) {
		for(int j=0;j<normalized.rows;j++) {
			float val = normalized.at<float>(j,i);
			if(val < 0.1) {
				stats.at<Vec3b>(j,i) = Vec3b(COLOR_WHITE);
			} else if(val < 0.2) {
				stats.at<Vec3b>(j,i) = Vec3b(255,255,0);
			} else if(val < 0.3) {
				stats.at<Vec3b>(j,i) = Vec3b(0,255,0);
			} else if(val < 0.4) {
				stats.at<Vec3b>(j,i) = Vec3b(50,255,150);
			} else if(val < 0.5) {
				stats.at<Vec3b>(j,i) = Vec3b(0,255,255);
			} else if(val < 0.6) {
				stats.at<Vec3b>(j,i) = Vec3b(25,135,255);
			} else if(val < 0.7) {
				stats.at<Vec3b>(j,i) = Vec3b(0,150,255);
			} else if(val < 0.8) {
				stats.at<Vec3b>(j,i) = Vec3b(0,85,255);
			} else if(val < 0.9) {
				stats.at<Vec3b>(j,i) = Vec3b(0,0,255);
			} else {
				stats.at<Vec3b>(j,i) = Vec3b(0,0,185);
			}
		}
	}
	return stats;
}

/* NORMALIZA LOS VALORES DE LA MATRIZ ENTRE 0 Y 1 */
Mat StatsAnalyzer::normalizeAreaStats(Mat m) {
	Mat result(m.rows, m.cols, CV_32SC1);
	double min, max;
	Point pMin, pMax;
	minMaxLoc(m, &min, &max, &pMin, &pMax);

	for(int i=0;i<m.cols;i++) {
		for(int j=0;j<m.rows;j++) {
			int val = m.at<int>(j,i);
			result.at<float>(j,i) = (val - min) / (max - min);
		}
	}
	return result;
}