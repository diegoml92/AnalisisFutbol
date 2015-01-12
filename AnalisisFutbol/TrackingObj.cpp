#include "GUI.h"
#include "TrackingObj.h"
#include "BallDetector.h"
#include "PlayerClassifier.h"
#include "From3DTo2D.h"
#include "GlobalStats.h"

/* REALIZA EL SEGUIMIENTO DE LOS ELEMENTOS DEL PARTIDO */
void TrackingObj::trackObject(Mat filtro, Mat &partido, int nVideo, Mat paint) {
	Mat temp;		// Matriz auxiliar en la que buscaremos los contornos
	filtro.copyTo(temp);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE);
	if(hierarchy.size() > 0) {													// Si se encuentra alg�n contorno
		PlayerClassifier::clearVectors();										// Limpiamos los vectores de clasificaci�n
		BallDetector::clear();													// Limpiamos el vector de balones

		for( int i = 0; i < contours.size(); i++ ) {							// Recorremos los contornos
			Rect elem = boundingRect(Mat(contours[i]));							// Creamos el boundingBox
			if(PlayerClassifier::isPlayerSize(elem)) {							// Si cumple las restricciones de tama�o...
				PlayerClassifier::addPlayer(partido, filtro, elem);				// ...a�ade el jugador y lo clasifica
				Point playerPos = Point(elem.br().x-elem.width/2, elem.br().y); // Calculamos el punto en el que est� el jugador
				From3DTo2D::get2DPosition(playerPos, nVideo);					// Transformamos a posici�n 2D
				GlobalStats::locations[nVideo].push_back(playerPos);			// A�adimos al vector de elementos encontrados
				From3DTo2D::paint2DPositions(elem, nVideo, paint);				// Representamos las posiciones en el plano 2D
			} else if(BallDetector::isBallSize(elem)) {							// Si tiene tama�o y forma de bal�n...
				BallDetector::addBall(elem);									// ...se a�ade
			}
		}

		PlayerClassifier::sortVectors();										// Ordenamos los vectores de clasificaci�n
		PlayerClassifier::drawTeams(partido);									// Dibujamos la etiqueta de cada elemento
		BallDetector::selectBall(partido, filtro);								// Elige el bal�n entre los candidatos
		BallDetector::drawBall(partido);										// Dibujamos el bal�n
	}
}
