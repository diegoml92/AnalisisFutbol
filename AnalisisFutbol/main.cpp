#include "config_file.h"
#include "VideoManager.h"
#include "TrackingObj.h"
#include "GUI.h"
#include "EventManager.h"
#include "FieldFilter.h"
#include "From3DTo2D.h"
#include "StatsAnalyzer.h"
#include "GlobalStats.h"

/* FUNCIÓN DE ENTRADA AL PROGRAMA */
int main(int argc, char* argv[]) {

	VideoManager::init();					// Cargamos el vídeo que vamos a utilizar

	Mat partido[N_VIDEOS];					// Irá almacenando cada fotograma del vídeo de entrada
	Mat umbral[N_VIDEOS];					// Almacenará el umbral actualizado según los valores del filtro

	Mat bg[N_VIDEOS];						// Almacenará los backgrounds obtenidos de cada secuencia
	for(int i=0; i<N_VIDEOS; i++) {
		std::stringstream path;
		path << VIDEO_PATH << i << BG_FORMAT;
		bg[i] = imread(path.str());
	}

	namedWindow(GUI_W);						// Creamos la ventana para la interfaz
	GUI::initGUI();							// Inicializamos la interfaz gráfica
	EventManager::initMouseListener();		// Inicializamos el controlador de eventos de ratón

	From3DTo2D::initProjectionMatrices();	// Inicializamos las matrices de proyección

	/*	
	*	Bucle en el que vamos pasando los frames del video con la función nextFrame,
    *   que coge el frame actual del video y lo guarda en la matriz partido. Cuando nextFrame
	*	devuelva false, el vídeo abrá acabado.
	*/
	while(VideoManager::nextFrame(partido)) {

		Mat paint;
		From3DTo2D::field2D.copyTo(paint);
        
        for(int i=0; i<N_VIDEOS; i++) {

            // Filtramos el campo en el partido
            umbral[i] = FieldFilter::discardField(partido[i].clone(), bg[i]);

            TrackingObj::trackObject(umbral[i],partido[i], i, paint);	// Hacemos el tracking de los elementos del campo

            GUI::showGUI();												// Mostramos la interfaz

        }

		vector<Point> painting_vector;

		for(int i=0; i<N_VIDEOS; i++) {
			if(GlobalStats::locations[i].size()) {
				GlobalStats::locations[i] = From3DTo2D::get2DPositionVector(GlobalStats::locations[i],i);
			}
		}

		for(int i=0; i<N_VIDEOS; i++) {
			for(vector<Point2f>::iterator it1=GlobalStats::locations[i].begin(); it1!=GlobalStats::locations[i].end(); ++it1) {
				bool found = false;
				for(int j=i+1; j<N_VIDEOS;j++) {
					for(vector<Point2f>::iterator it2=GlobalStats::locations[j].begin(); it2!=GlobalStats::locations[j].end(); ++it2) {
						if(StatsAnalyzer::isSamePoint(*it1, *it2)) {
							//*it2 = xxxx::calculateFinalPos(*it1, *it2);
							found = true;
							break;
						}
					}
				}
				if(!found) {
					From3DTo2D::paint2DPositions2(*it1,i,paint);
				}
			}
		}

		GlobalStats::clearLocations();

		imshow("2D FIELD",paint);

		//From3DTo2D::paint2DPositions_v(painting_vector,paint);

        Mat join = VideoManager::joinSequences(partido);

		pyrDown(join, join, Size(join.cols/2, join.rows/2));
		pyrDown(join, join, Size(join.cols/2, join.rows/2));

        imshow(VIDEO_W, join);
        waitKey(1);

	}

	destroyAllWindows();

	StatsAnalyzer::calculateAllStats();

	namedWindow(SGUI_W);					// Creamos la ventana para la interfaz
	EventManager::initMouseListener();		// Inicializamos el controlador de eventos de ratón
	GUI::initStatsGUI();

	while(1) {
		GUI::showStatsGUI();
		if (!GUI::isActiveIndividualMode())
		{
			if (GUI::isActiveComparativeMode())
			{
				GUI::showStatsWindow(0,1);
			} else {
				GUI::showStatsWindow(0,0);
			}
		} else {
			if (GUI::isActiveComparativeMode())
			{
				GUI::showStatsWindow(1,1);
			} else {
				GUI::showStatsWindow(1,0);
			}
		}
		waitKey(1);
	}

}