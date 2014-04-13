#include "config_file.h"
#include "VideoManager.h"
#include "TrackingObj.h"

// Valores que marca en rango de filtro del c�sped
int MIN_B = 5;
int MAX_B = 90;
int MIN_G = 75;
int MAX_G = 135;
int MIN_R = 35;
int MAX_R = 110;

// Valores para el rango de tama�o de los jugadores
int MIN_WIDTH = 10, MIN_HEIGH = 25;
int MAX_WIDTH = 45, MAX_HEIGH = 80;

/* Funci�n auxiliar llamada cada vez que var�a el valor de una barra de desplazamiento */
void on_trackbar(int, void*) {}	// No hace nada

/* Funci�n que crea las barras de ajuste del filtro */
void crearTrackbars() {
	
	// Creamos la ventana en la que estar�n las barras de desplazamiento
	namedWindow(GUI_W, 0);

	// Crea la barra
	createTrackbar("MIN_B", GUI_W, &MIN_B, MAX_RGB, on_trackbar);
	createTrackbar("MAX_B", GUI_W, &MAX_B, MAX_RGB, on_trackbar);
	createTrackbar("MIN_G", GUI_W, &MIN_G, MAX_RGB, on_trackbar);
	createTrackbar("MAX_G", GUI_W, &MAX_G, MAX_RGB, on_trackbar);
	createTrackbar("MIN_R", GUI_W, &MIN_R, MAX_RGB, on_trackbar);
	createTrackbar("MAX_R", GUI_W, &MAX_R, MAX_RGB, on_trackbar);

}

void crearTrackbars2() {
	string areaWindow = "Control de tama�o";
	namedWindow(areaWindow, 0);
	createTrackbar("Ancho MIN", areaWindow, &MIN_WIDTH, 100, on_trackbar);
	createTrackbar("Ancho MAX", areaWindow, &MAX_WIDTH, 100, on_trackbar);
	createTrackbar("Alto MIN", areaWindow, &MIN_HEIGH, 100, on_trackbar);
	createTrackbar("Alto MAX", areaWindow, &MAX_HEIGH, 100, on_trackbar);
}

/* Funci�n principal del programa */
int main(int argc, char* argv[]) {

	// Cargamos el v�deo que vamos a utilizar
	VideoManager::init();

	Mat partido;	// Ir� almacenando cada fotograma del v�deo de entrada
	Mat umbral;		// Mostrar� el umbral actualizado seg�n los valores del filtro

	Mat bordes;


	vector<Vec4i> lineas;		// Almacena las l�neas del campo

	// Si la calibraci�n est� activa creamos las trackbars
	if(CALIB) {
		crearTrackbars();
		crearTrackbars2();
	}

	// Bucle infinito en el que vamos pasando los frames del video (habr�a que determinar
	// que cuando finalice el video se salga del bucle para que no haya errores)
	// Con la funci�n read, se coge el frame actual de video, se guarda en la matriz
	// partido y se pasa al siguiente frame
	while(VideoManager::nextFrame(&partido)) {

		// Obtenemos de partido el umbral seg�n los rangos definidos por B,G,R MIN y MAX
		inRange(partido, Scalar(MIN_B, MIN_G, MIN_R), Scalar(MAX_B, MAX_G, MAX_R), umbral);

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