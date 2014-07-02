#include "StatsAnalyzer.h"

Mat StatsAnalyzer::ballArea = Mat::zeros(VIDEO_HEIGHT/ANALYZER_VIDEO_SIZE_RELATION, VIDEO_WIDTH/ANALYZER_VIDEO_SIZE_RELATION, CV_32SC1);

/* INCREMENTA EL VALOR EN LA POSICIÓN INDICADA */
void StatsAnalyzer::addBallPosition(int x, int y) {
	ballArea.at<int>(y/ANALYZER_VIDEO_SIZE_RELATION,x/ANALYZER_VIDEO_SIZE_RELATION)++;
}

/* DEVUELVE LAS ESTADÍSTICAS DEL BALÓN */
Mat StatsAnalyzer::getBallStats() {
	Mat normalized = normalizeBallAreaStats();
	Mat stats(normalized.rows, normalized.cols, CV_8UC3);
	for(int i=0;i<normalized.cols;i++) {
		for(int j=0;j<normalized.rows;j++) {
			float val = normalized.at<float>(j,i);
			if(val < 0.1) {
				stats.at<Vec3b>(j,i) = Vec3b(255,255,255);
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
Mat StatsAnalyzer::normalizeBallAreaStats() {
	Mat result(ballArea.rows, ballArea.cols, CV_32SC1);
	double min, max;
	Point pMin, pMax;
	minMaxLoc(ballArea, &min, &max, &pMin, &pMax);

	for(int i=0;i<ballArea.cols;i++) {
		for(int j=0;j<ballArea.rows;j++) {
			int val = ballArea.at<int>(j,i);
			result.at<float>(j,i) = (val - min) / (max - min); // Siempre 0?
		}
	}
	return result;
}