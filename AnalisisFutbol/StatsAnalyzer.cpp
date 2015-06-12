#include "StatsAnalyzer.h"
#include "GlobalStats.h"

Mat StatsAnalyzer::soccer_field = imread(FIELD2DSMALL_PATH);

/* INCREMENTA EL VALOR EN LA POSICIÓN INDICADA */
void StatsAnalyzer::addPosition(Mat m, Point p) {
	m.at<int>(p.y/ANALYZER_VIDEO_SIZE_RELATION,p.x/ANALYZER_VIDEO_SIZE_RELATION)++;
}

/* INCREMENTA LA DISTANCIA RECORRIDA */
void StatsAnalyzer::addDistanceAndSpeed(float* dist, Point actualPoint, Point lastPoint,
										float* speed, int* nSpeed, float* maxSpeed) {
	float d = distance(actualPoint, lastPoint);
	*dist += d;

	if(d>0) {
		float actualSpeed = d*FPS*0.1*MS_TO_KMH;
		*speed += actualSpeed;
		if(actualSpeed > *maxSpeed) {
			*maxSpeed = actualSpeed;
		}
		*nSpeed+=1;
	}
}

/* CALCULA LA DISTANCIA ENTRE DOS PUNTOS */
float StatsAnalyzer::distance(Point actualPoint, Point lastPoint) {
	float dist = 0;
	if(lastPoint.x >= 0) {
		dist = (float) norm(actualPoint-lastPoint)/10;
	}
	return dist;
}

/* DEVUELVE LAS ESTADÍSTICAS DE POSICIONES */
Mat StatsAnalyzer::getAreaStats(Mat m) {
	Mat normalized = normalizeAreaStats(m);
	Mat stats;
	soccer_field.copyTo(stats);
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
	for(int i=0; i<GlobalStats::playerV.size(); i++) {
		GlobalStats::playerV[i].calculateStats();
	}
}

/* DETERMINA SI DOS PUNTOS SE CORRESPONDEN CON EL MISMO ELEMENTO */
bool StatsAnalyzer::isSamePoint(Point p1, Point p2) {
	return distance(p1,p2) < 3;
}
