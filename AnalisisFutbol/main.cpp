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
        
		// Hacemos tracking del jugador
		for(int i=0; i<GlobalStats::detectedPlayers.size(); i++) {
			std::cout<<"2DPos Before: "<<GlobalStats::detectedPlayers.at(i)<<std::endl;
			TrackingObj::trackPlayers(partido,bg,&GlobalStats::detectedPlayers.at(i),i);
			std::cout<<"2DPos After : "<<GlobalStats::detectedPlayers.at(i)<<std::endl<<std::endl<<std::endl;
			From3DTo2D::paint2DPositions2(GlobalStats::detectedPlayers.at(i),-1,paint);
		}

		for(int i=GlobalStats::playersToDelete.size()-1; i>=0; i--) {
			int k = GlobalStats::playersToDelete.at(i);
			GlobalStats::detectedPlayers.erase(GlobalStats::detectedPlayers.begin()+k-1);
			GlobalStats::playersToDelete.pop_back();
		}

        for(int i=0; i<N_VIDEOS; i++) {
            umbral[i] = FieldFilter::discardField(partido[i].clone(), bg[i]);	// Filtramos el campo en el partido
            TrackingObj::objectDetection(umbral[i],partido[i], i, paint);		// Hacemos el tracking de los elementos del campo
			GUI::showGUI();														// Mostramos la interfaz
        }

		// Obtención de posiciones 2D de jugadores detectados
		for(int i=0; i<N_VIDEOS; i++) {
			if(GlobalStats::locations[i].size()) {
				GlobalStats::locations[i] = From3DTo2D::get2DPositionVector(GlobalStats::locations[i],i);
			}
		}

		// Eliminación de duplicidades (elementos localizados en varias cámaras)
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
				if(!found && From3DTo2D::isInRange(*it1) && !GlobalStats::alreadyDetected(*it1)) {
					From3DTo2D::paint2DPositions2(*it1,i,paint);
					//asignar();
					GlobalStats::detectedPlayers.push_back(*it1);
				}
			}
		}

		GlobalStats::addStats();

		GlobalStats::clearLocations();

		imshow("2D FIELD",paint);

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

	while(waitKey(1)!=27) {
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
	}

}
