#include "config_file.h"
#include "VideoManager.h"
#include "TrackingObj.h"
#include "GUI.h"
#include "EventManager.h"
#include "FieldFilter.h"

/* FUNCI�N DE ENTRADA AL PROGRAMA */
int main(int argc, char* argv[]) {

	VideoManager::init();					// Cargamos el v�deo que vamos a utilizar

	Mat partido;							// Ir� almacenando cada fotograma del v�deo de entrada
	Mat umbral;								// Almacenar� el umbral actualizado seg�n los valores del filtro
	Mat bordes;								// Mostrar� los contornos encontrados

	namedWindow(GUI_W);						// Creamos la ventana para la interfaz
	GUI::initGUI();							// Inicializamos la interfaz gr�fica
	EventManager::initMouseListener();		// Inicializamos el controlador de eventos de rat�n

	/*	
	*	Bucle infinito en el que vamos pasando los frames del video con la funci�n nextFrame,
	*	que coge el frame actual del video y lo guarda en la matriz partido. Cuando nextFrame
	*	devuelva false, el v�deo abr� acabado.
	*/
	while(VideoManager::nextFrame(&partido)) {

		// Obtenemos de partido el umbral seg�n los rangos definidos por B,G,R MIN y MAX
		umbral = FieldFilter::discardField(partido);
		
		// Queremos invertir el umbral para que los jugadores aparezcan en blanco
		//threshold(umbral, umbral, 0.5, 255, THRESH_BINARY_INV);

		// Realizamos algunas operaciones morfol�gicas para mejorar el filtro
		Mat morf,morfElement = getStructuringElement(MORPH_RECT, Size(5,5));
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
		//pyrDown(umbral, umbral, Size(umbral.cols/2, umbral.rows/2));
		//imshow(THRESHOLD_W, umbral);				// Mostramos tambi�n el threshold

		//while(waitKey()!=13);
		waitKey(1);									// No aparecer� la imagen si no utlizamos este waitKey
		
	}
}