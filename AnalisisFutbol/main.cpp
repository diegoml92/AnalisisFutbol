#include "config_file.h"
#include "VideoManager.h"
#include "TrackingObj.h"
#include "GUI.h"

/* Funci�n principal del programa */
int main(int argc, char* argv[]) {

	// Cargamos el v�deo que vamos a utilizar
	VideoManager::init();

	Mat partido;	// Ir� almacenando cada fotograma del v�deo de entrada
	Mat umbral;		// Mostrar� el umbral actualizado seg�n los valores del filtro

	Mat bordes;


	//vector<Vec4i> lineas;		// Almacena las l�neas del campo

	// Si la calibraci�n est� activa creamos las trackbars
	if(CALIB) {
		// Creamos la ventana en la que estar�n las barras de desplazamiento
		namedWindow(GUI_W, 0);
		gui::fieldThresTrackbars();
		gui::playerSizeTrackbars();
	}

	// Bucle infinito en el que vamos pasando los frames del video (habr�a que determinar
	// que cuando finalice el video se salga del bucle para que no haya errores)
	// Con la funci�n read, se coge el frame actual de video, se guarda en la matriz
	// partido y se pasa al siguiente frame
	while(VideoManager::nextFrame(&partido)) {

		// Obtenemos de partido el umbral seg�n los rangos definidos por B,G,R MIN y MAX
		inRange(partido, Scalar(gui::MIN_B, gui::MIN_G, gui::MIN_R), Scalar(gui::MAX_B, gui::MAX_G, gui::MAX_R), umbral);

		// Queremos invertir el umbral para que la idea quede m�s clara
		threshold(umbral, umbral, 0.5, 255, THRESH_BINARY_INV);

/*
		// Localiza los bordes en una imagen
		Canny(umbral, bordes, 1000, 1300, 3);
		// Localiza l�neas en im�genes binarias
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

		// Mostramos la imagen original
		imshow(VIDEO_W, partido);
		// Mostramos tambi�n el threshold
		imshow(THRESHOLD_W, morf);

		// No aparecer� la imagen si no utlizamos este waitKey
		while(waitKey()!=13);
	}
}