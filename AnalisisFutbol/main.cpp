#include "config_file.h"
#include "VideoManager.h"
#include "TrackingObj.h"
#include "GUI.h"
#include "EventManager.h"

/* FUNCIÓN DE ENTRADA AL PROGRAMA */
int main(int argc, char* argv[]) {

	VideoManager::init();					// Cargamos el vídeo que vamos a utilizar

	Mat partido;							// Irá almacenando cada fotograma del vídeo de entrada
	Mat umbral;								// Almacenará el umbral actualizado según los valores del filtro
	Mat bordes;								// Mostrará los contornos encontrados

	namedWindow(GUI_W);						// Creamos la ventana para la interfaz
	GUI::initGUI();							// Inicializamos la interfaz gráfica
	EventManager::initMouseListener();		// Inicializamos el controlador de eventos de ratón

	/*	
	*	Bucle infinito en el que vamos pasando los frames del video con la función nextFrame,
	*	que coge el frame actual del video y lo guarda en la matriz partido. Cuando nextFrame
	*	devuelva false, el vídeo abrá acabado.
	*/
	while(VideoManager::nextFrame(&partido)) {

		// Obtenemos de partido el umbral según los rangos definidos por B,G,R MIN y MAX
		inRange(partido, Scalar(GUI::MIN_B, GUI::MIN_G, GUI::MIN_R), Scalar(GUI::MAX_B, GUI::MAX_G, GUI::MAX_R), umbral);

		// Queremos invertir el umbral para que los jugadores aparezcan en blanco
		threshold(umbral, umbral, 0.5, 255, THRESH_BINARY_INV);

		/*
		vector<Vec4i> lineas;
		// Localiza los bordes en una imagen
		Canny(umbral, bordes, 1000, 1300, 3);
		// Localiza líneas en imágenes binarias
		HoughLinesP(bordes, lineas, 1, CV_PI/180, 50, 20, 50);

		for( size_t i = 0; i < lineas.size(); i++ ) {
			Vec4i l = lineas[i];
			line( umbral, Point(l[0], l[1]), Point(l[2], l[3]), 0, 3, CV_AA);
		}
		*/

		//rectangle(partido,Rect(Point(0,0),Size(GUI::MIN_BALL_SIZE,GUI::MIN_BALL_SIZE)),Scalar(71,71,71));
		//rectangle(partido,Rect(Point(0,0),Size(GUI::MAX_BALL_SIZE,GUI::MAX_BALL_SIZE)),Scalar(71,71,71));

		//rectangle(partido,Rect(Point(0,0),Size(GUI::MIN_WIDTH,GUI::MIN_HEIGH)),Scalar(71,71,71));
		//rectangle(partido,Rect(Point(0,0),Size(GUI::MAX_WIDTH,GUI::MAX_HEIGH)),Scalar(71,71,71));

		// Realizamos algunas operaciones morfológicas para mejorar el filtro
		Mat morf,morfElement = getStructuringElement(MORPH_RECT, Size(3,3));
		dilate(umbral,morf,morfElement);
		morphologyEx(morf,morf,MORPH_CLOSE,morfElement);

		TrackingObj::trackObject(umbral,partido);	// Hacemos el tracking de los elementos del campo

		GUI::showGUI();								// Mostramos la interfaz
		if(GUI::isActiveBallSize()) {				// Creamos las trackbars si corresponde
			namedWindow(BALL_SIZE_W);
			GUI::ballSizeTrackbars();
		} else {
			destroyWindow(BALL_SIZE_W);
		}
		if(GUI::isActiveFieldThres()) {				// Creamos las trackbars si corresponde
			namedWindow(FIELD_FILTER_W);
			GUI::fieldThresTrackbars();
		} else {
			destroyWindow(FIELD_FILTER_W);
		}
		if(GUI::isActivePlayerSize()) {				// Creamos las trackbars si corresponde
			namedWindow(PLAYER_SIZE_W);
			GUI::playerSizeTrackbars();
		} else {
			destroyWindow(PLAYER_SIZE_W);
		}

		pyrDown(partido, partido, Size(partido.cols/2, partido.rows/2));
		imshow(VIDEO_W, partido);					// Mostramos la imagen original
		//imshow(THRESHOLD_W, umbral);				// Mostramos también el threshold

		//while(waitKey()!=13);
		waitKey(1);									// No aparecerá la imagen si no utlizamos este waitKey
	}
}