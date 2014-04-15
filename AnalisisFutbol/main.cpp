#include "config_file.h"
#include "VideoManager.h"
#include "TrackingObj.h"
#include "GUI.h"
#include <iostream>

/* TRATA LOS EVENTOS DEL RATÓN */
void myMouseCallback(int ev, int x, int y, int flags, void* params);

/* Función principal del programa */
int main(int argc, char* argv[]) {

	// Cargamos el vídeo que vamos a utilizar
	VideoManager::init();

	Mat partido;	// Irá almacenando cada fotograma del vídeo de entrada
	Mat umbral;		// Mostrará el umbral actualizado según los valores del filtro

	Mat bordes;

	namedWindow(GUI_W);
	GUI::initGUI();
	setMouseCallback(GUI_W, myMouseCallback);

	//vector<Vec4i> lineas;		// Almacena las líneas del campo

	// Si la calibración está activa creamos las trackbars
	if(CALIB) {
		// Creamos la ventana en la que estarán las barras de desplazamiento
		namedWindow(GUI_W, 0);
		GUI::fieldThresTrackbars();
		GUI::playerSizeTrackbars();
	}

	// Bucle infinito en el que vamos pasando los frames del video (habría que determinar
	// que cuando finalice el video se salga del bucle para que no haya errores)
	// Con la función read, se coge el frame actual de video, se guarda en la matriz
	// partido y se pasa al siguiente frame
	while(VideoManager::nextFrame(&partido)) {

		// Obtenemos de partido el umbral según los rangos definidos por B,G,R MIN y MAX
		inRange(partido, Scalar(GUI::MIN_B, GUI::MIN_G, GUI::MIN_R), Scalar(GUI::MAX_B, GUI::MAX_G, GUI::MAX_R), umbral);

		// Queremos invertir el umbral para que la idea quede más clara
		threshold(umbral, umbral, 0.5, 255, THRESH_BINARY_INV);

/*
		// Localiza los bordes en una imagen
		Canny(umbral, bordes, 1000, 1300, 3);
		// Localiza líneas en imágenes binarias
		HoughLinesP(bordes, lineas, 1, CV_PI/180, 50, 20, 50);

		for( size_t i = 0; i < lineas.size(); i++ ) {
			Vec4i l = lineas[i];
			line( umbral, Point(l[0], l[1]), Point(l[2], l[3]), 0, 3, CV_AA);
		}
*/
		Mat morf,morfElement = getStructuringElement(MORPH_RECT, Size(3,3));
		dilate(umbral,morf,morfElement);
		morphologyEx(morf,morf,MORPH_CLOSE,morfElement);

		TrackingObj::trackObject(morf,partido);

		GUI::showGUI();

		// Mostramos la imagen original
		//imshow(VIDEO_W, partido);
		// Mostramos también el threshold
		//imshow(THRESHOLD_W, morf);

		// No aparecerá la imagen si no utlizamos este waitKey
		//while(waitKey()!=13);
		waitKey(30);
	}
}

void myMouseCallback(int ev, int x, int y, int flags, void* params) {
	switch(ev) {
		case CV_EVENT_MOUSEMOVE: {
			if(y >= BUTTON_TOP_MARGIN && y <= BUTTON_TOP_MARGIN + BUTTON_HEIGHT) {
				if(x >= BUTTON_LEFT_MARGIN && x <= BUTTON_LEFT_MARGIN + BUTTON_WIDTH) {
					GUI::setHoverButton(0);
				} else if(x >= BUTTON_LEFT_MARGIN + BUTTON_SHIFT && x <= BUTTON_LEFT_MARGIN + BUTTON_SHIFT + BUTTON_WIDTH) {
					GUI::setHoverButton(1);
				} else if(x >= BUTTON_LEFT_MARGIN + BUTTON_SHIFT * 2 && x <= BUTTON_LEFT_MARGIN + BUTTON_SHIFT * 2 + BUTTON_WIDTH) {
					GUI::setHoverButton(2);
				}
			}
			break;
		}
	}
}