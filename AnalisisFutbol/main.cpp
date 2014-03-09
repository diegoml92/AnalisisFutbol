// Se incluyen las librer�as necesarias para el proyecto
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>

// Indicamos el namespace para no tener que escribirlo delante de cada operaci�n que lo requiera
using namespace cv;

// Valores l�mite que marcar�n el rango para el posterior filtrado
int MIN_B = 5;
int MAX_B = 90;
int MIN_G = 75;
int MAX_G = 135;
int MIN_R = 35;
int MAX_R = 110;

// Contante que indica el valor m�ximo que se puede dar a una trackbar
const int MAX_RGB = 255;

// Valores para el tama�o de los jugadores
int MIN_WIDTH = 10, MIN_HEIGH = 25;
int MAX_WIDTH = 45, MAX_HEIGH = 80;

// Nombres de las ventanas que ser�n abieras m�s adelante
const string windowNameThreshold = "Imagen filtrada";
const string windowNameOriginal = "Imagen original";
const string windowNameTrackbars = "Barras de desplazamiento";

// Ruta del fichero donde se encuentra el video a analizar
const string filename = "C:/Proyecto/Barca_Madrid.mp4";

// Vectores para la clasificaci�n por equipos
vector<vector<Rect>> clasif;
vector<vector<Rect>>::iterator it;

vector<Mat> clasifHists;
vector<Mat>::iterator itHist;

/* Compara el histograma de los jugadores y los clasifica por equipos */
void comparePlayer(Mat partido, Mat umbral, Rect rect) {

	int channels [] = {0,1,2};
	int nBins = 256;
	float range [] = {0,256};
	const float *ranges = {range};
	Mat hist;
	calcHist(&partido(rect),1,channels,umbral(rect),hist,1,&nBins,&ranges);
	normalize(hist,hist,1,0,NORM_MINMAX,-1,Mat());
	if(clasifHists.empty()) {
		clasifHists.push_back(hist);
		clasif.push_back(vector<Rect>());
		clasif.back().push_back(rect);
	} else {
		bool found = false;
		itHist = clasifHists.begin();
		int k = 0;
		while(itHist!=clasifHists.end() && !found) {
			found = compareHist(*itHist,hist,CV_COMP_BHATTACHARYYA) < 0.45;
			itHist++;
			k++;
		}
		if(found) {
			it = clasif.begin();
			clasif[k-1].push_back(rect);
		} else {
			clasifHists.push_back(hist);
			clasif.push_back(vector<Rect>());
			clasif.back().push_back(rect);
		}
	}
}

void sortVectors() {
	bool swapped = false;
	if(clasif.size()>1) {
		vector<Rect> aux;
		Mat auxHist;
		for(int i=0;i<clasif.size()-1;i++) {
			for(int j=0;j<clasif.size()-i-1;j++) {
				if(clasif[j].size() < clasif[j+1].size()) {
					aux = clasif[j];
					clasif[j] = clasif[j+1];
					clasif[j+1] = aux;

					auxHist = clasifHists[j];
					clasifHists[j] = clasifHists[j+1];
					clasifHists[j+1] = auxHist;
				}
			}
		}
	}
}


/* Funci�n auxiliar llamada cada vez que var�a el valor de una barra de desplazamiento */
void on_trackbar(int, void*) {}	// No hace nada

/* Funci�n que crea las barras de ajuste del filtro */
void crearTrackbars() {
	
	// Creamos la ventana en la que estar�n las barras de desplazamiento
	namedWindow(windowNameTrackbars, 0);

	// Crea la barra
	createTrackbar("MIN_B", windowNameTrackbars, &MIN_B, MAX_RGB, on_trackbar);
	createTrackbar("MAX_B", windowNameTrackbars, &MAX_B, MAX_RGB, on_trackbar);
	createTrackbar("MIN_G", windowNameTrackbars, &MIN_G, MAX_RGB, on_trackbar);
	createTrackbar("MAX_G", windowNameTrackbars, &MAX_G, MAX_RGB, on_trackbar);
	createTrackbar("MIN_R", windowNameTrackbars, &MIN_R, MAX_RGB, on_trackbar);
	createTrackbar("MAX_R", windowNameTrackbars, &MAX_R, MAX_RGB, on_trackbar);

}

void crearTrackbars2() {
	string areaWindow = "Control de tama�o";
	namedWindow(areaWindow, 0);
	createTrackbar("Ancho MIN", areaWindow, &MIN_WIDTH, 100, on_trackbar);
	createTrackbar("Ancho MAX", areaWindow, &MAX_WIDTH, 100, on_trackbar);
	createTrackbar("Alto MIN", areaWindow, &MIN_HEIGH, 100, on_trackbar);
	createTrackbar("Alto MAX", areaWindow, &MAX_HEIGH, 100, on_trackbar);
}

/* Funci�n que hace el seguimiento de los elementos del partido */
void trackObject(Mat filtro, Mat &partido) {
	Mat temp;
	filtro.copyTo(temp);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE);
	double refArea=0;
	if(hierarchy.size() > 0) {
		int numObjects = hierarchy.size();

		clasif.clear();
		clasifHists.clear();

		for(int index = 0; index >= 0; index = hierarchy[index][0]) {

			vector<Rect> minRect( contours.size() );
			for( int i = 0; i < contours.size(); i++ ) {
				minRect[i] = boundingRect( Mat(contours[i]) );
			}
			if( (minRect[index].width>MIN_WIDTH && minRect[index].width<MAX_WIDTH) &&
				(minRect[index].height>MIN_HEIGH && minRect[index].height<MAX_HEIGH) ) {

				comparePlayer(partido,filtro,minRect[index]);

				bool found = false;
				vector<Rect> s;
				it = clasif.begin();
				while(it!=clasif.end() && !found) {
					s = *it;
					found = std::find(s.begin(),s.end(),minRect[index]) != s.end();
					it++;
				}

				rectangle(partido,minRect[index],mean(partido(*s.begin())),2,8);

			}		////**********************************
			else {
				//drawContours(partido,contours,index,Scalar(0,0,255));
			}		//********************************
		}

		sortVectors();

		std::cout<<clasif.size()<<std::endl;
		for(int i=0;i<clasif.size();i++) {
			std::cout<<"Grupo #"<<i+1<<" --> "<<clasif[i].size()<<" miembros."<<std::endl;
		}


		for(int i=0;i<clasif.size();i++) {
			for(int j=0;j<clasif[i].size();j++) {
				if(i==0) {
					putText(partido,"Equipo1",Point(clasif[i][j].x,clasif[i][j].y),1,2,Scalar(255,255,255));
				} else if(i==1) {
					putText(partido,"Equipo2",Point(clasif[i][j].x,clasif[i][j].y),1,2,Scalar(255,255,255));
				} else {
					putText(partido,"Otros",Point(clasif[i][j].x,clasif[i][j].y),1,2,Scalar(255,255,255));
				}
			}
		}
	}
}

/* Funci�n principal del programa */
int main(int argc, char* argv[]) {

	// Cargamos el v�deo que vamos a utilizar
	VideoCapture video;
	video.open(filename);

	Mat partido;	// Ir� almacenando cada fotograma del v�deo de entrada
	Mat umbral;		// Mostrar� el umbral actualizado seg�n los valores del filtro

	Mat bordes;


	vector<Vec4i> lineas;		// Almacena las l�neas del campo

	// Variable booleana que nos permitir� decidir si utilizar trackbars o no. Cuando el filtro del c�sped sea
	// lo suficientemente bueno podemos desactivarlo
	bool calib = false;
	// Se indica si queremos hacer el seguimiento de los jugadores. Hasta que no tengamos preparado el filtro del
	// campo no nos interesa activar esta opci�n
	bool tracking = false;

	// Vamos a saltar los primeros minutos de video, ya que contienen informaci�n que no nos interesa
	// (himnos y protocolos antes de comenzar el partido) para ello, lo que hacemos es utilizar una propiedad
	// de la captura que nos permite averiguar el n�mero total de frames de un v�deo, y posteriormente calculamos:
	// Ms / Mt * CV_CAP_PROP_FRAME_COUNT, y ese ser� el n�mero de frames a saltar. Para saltarlos vamos a
	// utilizar la funci�n set, cambiando la propiedad adecuada (0.0505)
	video.set(CV_CAP_PROP_POS_FRAMES, 0.001 * video.get(CV_CAP_PROP_FRAME_COUNT));

	// Si la calibraci�n est� activa creamos las trackbars
	if(calib) {
		crearTrackbars();
	}

	crearTrackbars2();

	// Bucle infinito en el que vamos pasando los frames del video (habr�a que determinar
	// que cuando finalice el video se salga del bucle para que no haya errores)
	while(1) {

		// Con la funci�n read, se coge el frame actual de video, se guarda en la matriz
		// partido y se pasa al siguiente frame
		video.read(partido);

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
		trackObject(morf,partido);		

		// Mostramos la imagen original
		imshow(windowNameOriginal, partido);
		// Mostramos tambi�n el threshold
		imshow(windowNameThreshold, morf);

		// No aparecer� la imagen si no utlizamos este waitKey
		while(waitKey()!=13);
	}
}