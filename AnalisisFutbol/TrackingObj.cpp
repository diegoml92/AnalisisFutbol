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
	if(hierarchy.size() > 0) {		// Si se encuentra alg�n contorno
		int numObjects = hierarchy.size();

		PlayerClassifier::clearVectors();	// Limpiamos los vectores de clasificaci�n

		for(int index = 0; index >= 0; index = hierarchy[index][0]) {

			vector<Rect> minRect( contours.size() );		// Almacenar� los boundingBox de cada contorno
			for( int i = 0; i < contours.size(); i++ ) {
				minRect[i] = boundingRect( Mat(contours[i]) );
			}
			// Si cumple las restricciones de tama�o
			if( (minRect[index].width>GUI::MIN_WIDTH && minRect[index].width<GUI::MAX_WIDTH) &&
				(minRect[index].height>GUI::MIN_HEIGH && minRect[index].height<GUI::MAX_HEIGH) ) {

				PlayerClassifier::comparePlayer(partido,filtro,minRect[index]); // Hacemos la comparaci�n
				PlayerClassifier::findAndDraw(minRect[index], partido);			// Dibujamos al jugador

			} else if(minRect[index].width <= GUI::MAX_BALL_SIZE &&		// Cumple las condiciones para ser el bal�n
						minRect[index].width >= GUI::MIN_BALL_SIZE &&
						minRect[index].height <= GUI::MAX_BALL_SIZE &&
						minRect[index].height >= GUI::MIN_BALL_SIZE) {
				switch (GUI::ballBox()) {
					case 1 : {
						rectangle(partido,minRect[index],Scalar(255,255,255),1);
						break;
					}
					case 2 : {
						circle(partido,Point(minRect[index].x + minRect[index].width/2,
								minRect[index].y + minRect[index].height/2), 4, Scalar(255,255,255),-1);
						break;
						}
				}
				
			}
		}

		PlayerClassifier::sortVectors();		// Ordenamos los vectores de clasificaci�n
		PlayerClassifier::drawTeams(partido);	// Dibujamos la etiqueta de cada elemento
	}
}
