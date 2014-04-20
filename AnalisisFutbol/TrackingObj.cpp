#include "GUI.h"
#include "TrackingObj.h"
#include "PlayerClassifier.h"

/* REALIZA EL SEGUIMIENTO DE LOS ELEMENTOS DEL PARTIDO */
void TrackingObj::trackObject(Mat filtro, Mat &partido) {
	Mat temp;		// Matriz auxiliar en la que buscaremos los contornos
	filtro.copyTo(temp);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE);
	if(hierarchy.size() > 0) {											// Si se encuentra algún contorno
		PlayerClassifier::clearVectors();								// Limpiamos los vectores de clasificación

		for( int i = 0; i < contours.size(); i++ ) {					// Recorremos los contornos
			Rect player = boundingRect(Mat(contours[i]));				// Creamos el boundingBox
			if(PlayerClassifier::isPlayerSize(player)) {				// Si cumple la restricciones de tamaño...
				PlayerClassifier::addPlayer(partido, filtro, player);	// ...añade al jugador y lo clasifica
			} else if(player.width <= GUI::MAX_BALL_SIZE &&				// Si cumple las condiciones para ser el balón...
						player.width >= GUI::MIN_BALL_SIZE &&
						player.height <= GUI::MAX_BALL_SIZE &&
						player.height >= GUI::MIN_BALL_SIZE) {
				switch (GUI::ballBox()) {
					case 1 : {
						rectangle(partido,player,Scalar(255,255,255),1);
						break;
					}
					case 2 : {
						circle(partido,Point(player.x + player.width/2,
								player.y + player.height/2), 4, Scalar(255,255,255),-1);
						break;
					}
				}
				
			}
		}
		PlayerClassifier::sortVectors();		// Ordenamos los vectores de clasificación
		PlayerClassifier::drawTeams(partido);	// Dibujamos la etiqueta de cada elemento
	}
}
