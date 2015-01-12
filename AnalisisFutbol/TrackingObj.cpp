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
	if(hierarchy.size() > 0) {													// Si se encuentra algún contorno
		PlayerClassifier::clearVectors();										// Limpiamos los vectores de clasificación
		BallDetector::clear();													// Limpiamos el vector de balones

		for( int i = 0; i < contours.size(); i++ ) {							// Recorremos los contornos
			Rect elem = boundingRect(Mat(contours[i]));							// Creamos el boundingBox
			if(PlayerClassifier::isPlayerSize(elem)) {							// Si cumple las restricciones de tamaño...
				PlayerClassifier::addPlayer(partido, filtro, elem);				// ...añade el jugador y lo clasifica
				Point playerPos = Point(elem.br().x-elem.width/2, elem.br().y); // Calculamos el punto en el que está el jugador
				From3DTo2D::get2DPosition(playerPos, nVideo);					// Transformamos a posición 2D
				GlobalStats::locations[nVideo].push_back(playerPos);			// Añadimos al vector de elementos encontrados
				From3DTo2D::paint2DPositions(elem, nVideo, paint);				// Representamos las posiciones en el plano 2D
			} else if(BallDetector::isBallSize(elem)) {							// Si tiene tamaño y forma de balón...
				BallDetector::addBall(elem);									// ...se añade
			}
		}

		PlayerClassifier::sortVectors();										// Ordenamos los vectores de clasificación
		PlayerClassifier::drawTeams(partido);									// Dibujamos la etiqueta de cada elemento
		BallDetector::selectBall(partido, filtro);								// Elige el balón entre los candidatos
		BallDetector::drawBall(partido);										// Dibujamos el balón
	}
}
