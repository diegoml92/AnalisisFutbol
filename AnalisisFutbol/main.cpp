#include "config_file.h"
#include "VideoManager.h"
#include "TrackingObj.h"

// Valores que marca en rango de filtro del césped
int MIN_B = 5;
int MAX_B = 90;
int MIN_G = 75;
int MAX_G = 135;
int MIN_R = 35;
int MAX_R = 110;

// Valores para el rango de tamaño de los jugadores
int MIN_WIDTH = 10, MIN_HEIGH = 25;
int MAX_WIDTH = 45, MAX_HEIGH = 80;

/* Función auxiliar llamada cada vez que varía el valor de una barra de desplazamiento */
void on_trackbar(int, void*) {}	// No hace nada

/* Función que crea las barras de ajuste del filtro */
void crearTrackbars() {
	
	// Creamos la ventana en la que estarán las barras de desplazamiento
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
	string areaWindow = "Control de tamaño";
	namedWindow(areaWindow, 0);
	createTrackbar("Ancho MIN", areaWindow, &MIN_WIDTH, 100, on_trackbar);
	createTrackbar("Ancho MAX", areaWindow, &MAX_WIDTH, 100, on_trackbar);
	createTrackbar("Alto MIN", areaWindow, &MIN_HEIGH, 100, on_trackbar);
	createTrackbar("Alto MAX", areaWindow, &MAX_HEIGH, 100, on_trackbar);
}

/* Función principal del programa */
int main(int argc, char* argv[]) {

	// Cargamos el vídeo que vamos a utilizar
	VideoManager::init();

	Mat partido;	// Irá almacenando cada fotograma del vídeo de entrada
	Mat umbral;		// Mostrará el umbral actualizado según los valores del filtro

	Mat bordes;


	vector<Vec4i> lineas;		// Almacena las líneas del campo

	// Si la calibración está activa creamos las trackbars
	if(CALIB) {
		crearTrackbars();
		crearTrackbars2();
	}

	// Bucle infinito en el que vamos pasando los frames del video (habría que determinar
	// que cuando finalice el video se salga del bucle para que no haya errores)
	// Con la función read, se coge el frame actual de video, se guarda en la matriz
	// partido y se pasa al siguiente frame
	while(VideoManager::nextFrame(&partido)) {

		// Obtenemos de partido el umbral según los rangos definidos por B,G,R MIN y MAX
		inRange(partido, Scalar(MIN_B, MIN_G, MIN_R), Scalar(MAX_B, MAX_G, MAX_R), umbral);

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

		// Mostramos la imagen original
		imshow(VIDEO_W, partido);
		// Mostramos también el threshold
		imshow(THRESHOLD_W, morf);

		// No aparecerá la imagen si no utlizamos este waitKey
		while(waitKey()!=13);
	}
}