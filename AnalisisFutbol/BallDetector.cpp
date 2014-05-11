#include "BallDetector.h"
#include "GUI.h"

vector<Rect> BallDetector::balls;						// Elementos que pueden ser el balón
Rect BallDetector::foundBall;							// Balón encontrado

/* LIMPIA EL VECTOR DE BALONES */
void BallDetector::clear() {
	balls.clear();
}

/* AÑADE UNA POSIBILIDAD A LA LISTA DE BALONES */
void BallDetector::addBall(Rect ball) {
	balls.push_back(ball);
}

/* DETERMINA SI CUMPLE EL TAMAÑO DEL BALÓN */
bool BallDetector::isBallSize(Rect ball) {
	return ball.width <= GUI::MAX_BALL_SIZE &&		// Si cumple las condiciones para ser el balón...
			ball.width >= GUI::MIN_BALL_SIZE &&
			ball.height <= GUI::MAX_BALL_SIZE &&
			ball.height >= GUI::MIN_BALL_SIZE &&
			isSquare(ball);
}

/* DETERMINA SI ES SIMILAR A UN CUADRADO */
bool BallDetector::isSquare(Rect ball) {
	float sq = ball.width / ball.height;
	return sq >= 0.8 && sq <= 1.2;
}

/* DETERMINA CUÁL ES EL BALÓN */
void BallDetector::selectBall(Mat partido, Mat filtro) {
	int max = 0;
	int k = 0;
	for(int i=0; i<balls.size(); i++) {
		Scalar mean = cv::mean( partido(balls[i]), filtro(balls[i]) );
		int n = mean[0] + mean[1] + mean[2];
		if(n > max) {
			k = i;
			max = n;
		}
	}
	if(max > 300)
		foundBall = balls[k];
	else
		foundBall = Rect();
}

/* DIBUJA EL BALÓN */
void BallDetector::drawBall(Mat partido) {
	switch (GUI::ballBox()) {
		case 1 : {
			rectangle(partido,foundBall,Scalar(255,255,255),1);
			break;
		}
		case 2 : {
			circle(partido,Point(foundBall.x + foundBall.width/2,
					foundBall.y + foundBall.height/2), foundBall.height/2, Scalar(255,255,255),-1);
			break;
		}
	}
}