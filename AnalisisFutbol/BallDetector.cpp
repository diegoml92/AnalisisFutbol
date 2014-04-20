#include "BallDetector.h"
#include "GUI.h"

vector<Rect> BallDetector::balls;						// Elementos que pueden ser el bal�n
Rect BallDetector::foundBall;							// Bal�n encontrado

/* A�ADE UNA POSIBILIDAD A LA LISTA DE BALONES */
void BallDetector::addBall(Rect ball) {
	balls.push_back(ball);
}

/* DETERMINA SI CUMPLE EL TAMA�O DEL BAL�N */
bool BallDetector::isBallSize(Rect ball) {
	return ball.width <= GUI::MAX_BALL_SIZE &&		// Si cumple las condiciones para ser el bal�n...
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

/* DETERMINA CU�L ES EL BAL�N */
void BallDetector::selectBall() {
	if(balls.size() > 0) {
		foundBall = balls[0];
	}
}

/* DIBUJA EL BAL�N */
void BallDetector::drawBall(Mat partido) {
	switch (GUI::ballBox()) {
		case 1 : {
			rectangle(partido,foundBall,Scalar(255,255,255),1);
			break;
		}
		case 2 : {
			circle(partido,Point(foundBall.x + foundBall.width/2,
					foundBall.y + foundBall.height/2), 10, Scalar(255,255,255),-1);
			break;
		}
	}
}