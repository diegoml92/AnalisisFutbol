#include "config_file.h"
#include "VideoManager.h"

// Indicamos el namespace para no tener que escribirlo delante de cada operaci�n que lo requiera
using namespace cv;

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


vector<vector<Rect>> clasif;
vector<vector<Rect>>::iterator it;

vector<vector<Mat>> clasifHists;
vector<vector<Mat>>::iterator itHist;

/* Compara el histograma de los jugadores y los clasifica por equipos */
void comparePlayer(Mat partido, Mat umbral, Rect rect) {

	int channels [] = {0,1,2};
	int nBins = 256;
	float range [] = {0,256};
	const float *ranges = {range};
	Mat hist_B, hist_G, hist_R;
	vector<Mat> planes, hist_v;
	split(partido(rect),planes);
	calcHist(&planes[0],1,channels,umbral(rect),hist_B,1,&nBins,&ranges);
	calcHist(&planes[1],1,channels,umbral(rect),hist_G,1,&nBins,&ranges);
	calcHist(&planes[2],1,channels,umbral(rect),hist_R,1,&nBins,&ranges);
	hist_v.push_back(hist_B);
	hist_v.push_back(hist_G);
	hist_v.push_back(hist_R);
	if(clasifHists.empty()) {
		clasifHists.push_back(hist_v);
		clasif.push_back(vector<Rect>());
		clasif.back().push_back(rect);
	} else {
		bool found = false;
		itHist = clasifHists.begin();
		int k = 0;
		vector<Mat> temp;
		while(itHist!=clasifHists.end() && !found) {
			temp = *itHist;
			found = compareHist(temp[0],hist_B,CV_COMP_BHATTACHARYYA) +
					compareHist(temp[1],hist_G,CV_COMP_BHATTACHARYYA) +
					compareHist(temp[2],hist_R,CV_COMP_BHATTACHARYYA) < BHATTACHARYYA_THRES*3;
			itHist++;
			k++;
		}
		if(found) {
			clasif[k-1].push_back(rect);
		} else {
			clasifHists.push_back(hist_v);
			clasif.push_back(vector<Rect>());
			clasif.back().push_back(rect);
		}
	}
}

void sortVectors() {
	bool swapped = false;
	if(clasif.size()>1) {
		vector<Rect> aux;
		vector<Mat> auxHist;
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

		rectangle(filtro, Point(0,0), Point(MAX_BALL_SIZE,MAX_BALL_SIZE), Scalar(0), 0);
		rectangle(filtro, Point(0,0), Point(7,7), Scalar(0), 0);

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
			} else if(	minRect[index].width < MAX_BALL_SIZE &&
						minRect[index].height < MAX_BALL_SIZE) {
				rectangle(partido,minRect[index],Scalar(0,0,255),-1);
			}
		}

		sortVectors();

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
		trackObject(morf,partido);		

		// Mostramos la imagen original
		imshow(VIDEO_W, partido);
		// Mostramos tambi�n el threshold
		imshow(THRESHOLD_W, morf);

		// No aparecer� la imagen si no utlizamos este waitKey
		while(waitKey()!=13);
	}
}