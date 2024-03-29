#include "StatsAnalyzer.h"
#include "GlobalStats.h"
#include "PlayerClassifier.h"

/* INCREMENTA EL VALOR EN LA POSICIÓN INDICADA */
void StatsAnalyzer::addPosition(Mat m, Point p) {
	m.at<int>(p.y/ANALYZER_VIDEO_SIZE_RELATION,p.x/ANALYZER_VIDEO_SIZE_RELATION)++;
}

/* CALCULA LA DISTANCIA ENTRE DOS PUNTOS */
float StatsAnalyzer::distance(Point actualPoint, Point lastPoint) {
	float dist = 0;
	if(lastPoint.x >= 0) {
		dist = norm(actualPoint-lastPoint)/10.0;
	}
	return dist;
}

/* INCREMENTA LA DISTANCIA RECORRIDA */
void StatsAnalyzer::addDistanceAndSpeed(float* dist, Point actualPoint, Point* lastPoint,
										float* speed, int* nSpeed, float* maxSpeed, int* retries) {
	float d = StatsAnalyzer::distance(actualPoint, *lastPoint);

	if(d>0) {
		float actualSpeed = d*FPS*MS_TO_KMH / (SAMPLING_RATE * (*retries + 1));
		if(actualSpeed < MAX_SPEED) {
			*retries = 0;
			*dist += d;
			*speed += actualSpeed;
			if(actualSpeed > *maxSpeed) {
				*maxSpeed = actualSpeed;
			}
			*nSpeed += 1;
			*lastPoint = actualPoint;
		} else {
			*retries += 1;
		}
	}
}

/* ACTUALIZA LAS ESTADÍSTICAS PARA UN JUGADOR RECUPERADO */
void StatsAnalyzer::updateStats(float* dist, Point actualPoint, Point* lastPoint, float* speed,
								int* nSpeed, float* maxSpeed, int* retries, int nFrames) {
	float d = StatsAnalyzer::distance(actualPoint, *lastPoint);

	if(d>0) {
		float actualSpeed = d*FPS*MS_TO_KMH / ( nFrames + SAMPLING_RATE * (*retries));
		if(actualSpeed < MAX_SPEED) {
			*retries = 0;
			*dist += d;
			*speed += actualSpeed;
			if(actualSpeed > *maxSpeed) {
				*maxSpeed = actualSpeed;
			}
			*nSpeed += 1;
			*lastPoint = actualPoint;
		} else {
			*retries += 1;
		}
	}
}

/* DEVUELVE LAS ESTADÍSTICAS DE POSICIONES */
Mat StatsAnalyzer::getAreaStats(Mat m) {
	Mat normalized = normalizeAreaStats(m);
	Mat stats;
	GlobalStats::soccer_field.copyTo(stats);
	for(int i=0;i<normalized.rows;i++) {
		for(int j=0;j<normalized.cols;j++) {
			float val = normalized.at<float>(i,j);
			if(val > 0) {
				if(val < 0.1) {
					stats.at<Vec3b>(i,j) = Vec3b(255,255,255);
				} else if(val < 0.2) {
					stats.at<Vec3b>(i,j) = Vec3b(255,255,0);
				} else if(val < 0.3) {
					stats.at<Vec3b>(i,j) = Vec3b(0,255,0);
				} else if(val < 0.4) {
					stats.at<Vec3b>(i,j) = Vec3b(50,255,150);
				} else if(val < 0.5) {
					stats.at<Vec3b>(i,j) = Vec3b(0,255,255);
				} else if(val < 0.6) {
					stats.at<Vec3b>(i,j) = Vec3b(25,135,255);
				} else if(val < 0.7) {
					stats.at<Vec3b>(i,j) = Vec3b(0,150,255);
				} else if(val < 0.8) {
					stats.at<Vec3b>(i,j) = Vec3b(0,85,255);
				} else if(val < 0.9) {
					stats.at<Vec3b>(i,j) = Vec3b(0,0,255);
				} else {
					stats.at<Vec3b>(i,j) = Vec3b(0,0,185);
				}
			}
		}
	}
	return stats;
}

/* NORMALIZA LOS VALORES DE LA MATRIZ ENTRE 0 Y 1 */
Mat StatsAnalyzer::normalizeAreaStats(Mat m) {
	Mat result (m.rows, m.cols, CV_32SC1);
	double min, max;
	Point pMin, pMax;
	minMaxLoc(m, &min, &max, &pMin, &pMax);

	if(max>0) {
		for(int i=0;i<m.rows;i++) {
			for(int j=0;j<m.cols;j++) {
				int val = m.at<int>(i,j);
				result.at<float>(i,j) = (val - min) / (max - min);
			}
		}
	}

	return result;
}

/* CALCULA TODAS LAS ESTADÍSTICAS */
void StatsAnalyzer::calculateAllStats() {
	for(int i=0; i<PlayerClassifier::playerV.size(); i++) {
		PlayerClassifier::playerV[i].calculateStats();
	}
}

/* DETERMINA SI DOS PUNTOS SE CORRESPONDEN CON EL MISMO ELEMENTO */
bool StatsAnalyzer::isSamePoint(Point p1, Point p2) {
	return distance(p1,p2) < 3;
}

/* DETERMINA SI EL PUNTO ESTÁ EN EL RANGO PARA SER RECUPERADO */
bool StatsAnalyzer::isInRecoverRange(Point p1, Point p2, int time, float* dist) {
	time /= FPS;
	*dist = distance(p1,p2);
	return *dist < 5 + 2.5*time;
}
