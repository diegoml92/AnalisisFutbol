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

	Mat frame[N_VIDEOS];					// Irá almacenando cada fotograma del vídeo de entrada
	Mat filter[N_VIDEOS];					// Almacenará el umbral actualizado según los valores del filtro
	Mat bg[N_VIDEOS];						// Almacenará los backgrounds de cada secuencia
	Mat hsv[N_VIDEOS];
	for(int i=0; i<N_VIDEOS; i++) {
		std::stringstream path;
		path << VIDEO_PATH << i << BG_FORMAT;
		bg[i] = imread(path.str());
	}

	/*VideoWriter outputVideo2D,outputVideoCams;
	Size size1 = Size(SOCCER_FIELD_WIDTH,SOCCER_FIELD_HEIGHT);
	Size size2 = Size(VIDEO_WIDTH*3/4,(VIDEO_HEIGHT*2+8)/4);
	outputVideo2D.open("video/field2D_out_2.avi", -1, 25, size1, true);
    outputVideoCams.open("video/sequences_out_2.avi", -1, 25, size2, true);

    if (!outputVideo2D.isOpened() && !outputVideoCams.isOpened())
    {
        return -1;
    }*/

	From3DTo2D::initProjectionMatrices();	// Inicializamos las matrices de proyección

	/*	
	*	Bucle en el que vamos pasando los frames del video con la función nextFrame,
    *   que coge el frame actual del video y lo guarda en la matriz frame. Cuando nextFrame
	*	devuelva false, el vídeo abrá acabado.
	*/
	while(VideoManager::nextFrame(frame) && waitKey(1)!=27) {

		double init_time,end_time;
		init_time = getTickCount();

		Mat paint;
		From3DTo2D::field2D.copyTo(paint);

		double a,b,a_1,b_1,a_2,b_2;
		
		a = getTickCount();

		for(int i=0; i<N_VIDEOS; i++) {
			a_1 = getTickCount();
			filter[i] = FieldFilter::discardField(frame[i].clone(), bg[i]);	// Filtramos el campo en el frame
			b_1 = getTickCount();
			a_2 = getTickCount();
			cvtColor(frame[i],hsv[i],COLOR_BGR2HSV);
			b_2 = getTickCount();
		}

		b = getTickCount();

		std::cout<<"Filtro+HSV: "<<(b-a)/getTickFrequency()<<std::endl;
		std::cout<<"   +Filtro:     "<<(b_1-a_1)/getTickFrequency()<<std::endl;
		std::cout<<"   +cvtColor:   "<<(b_2-a_2)/getTickFrequency()<<std::endl;

		a = getTickCount();
        
		// Hacemos tracking del jugador
		for(int i=0; i<GlobalStats::detectedPlayers.size(); i++) {
			a_1 = getTickCount();
			TrackingObj::trackPlayers(hsv,filter,&GlobalStats::detectedPlayers.at(i),i);
			b_1 = getTickCount();
			a_2 = getTickCount();
			From3DTo2D::paint2DPositions2(GlobalStats::detectedPlayers.at(i),-1,paint);
			b_2 = getTickCount();
		}

		b = getTickCount();

		std::cout<<"Meanshit:   "<<(b-a)/getTickFrequency()<<std::endl;
		std::cout<<"   +Tracking:   "<<(b_1-a_1)/getTickFrequency()<<std::endl;
		std::cout<<"   +Paint:      "<<(b_2-a_2)/getTickFrequency()<<std::endl;

		a = getTickCount();

		for(int i=GlobalStats::playersToDelete.size()-1; i>=0; i--) {
			int k = GlobalStats::playersToDelete.at(i);
			GlobalStats::detectedPlayers.erase(GlobalStats::detectedPlayers.begin()+k-1);
			GlobalStats::playersToDelete.pop_back();
		}

		b = getTickCount();

		std::cout<<"Deleting:   "<<(b-a)/getTickFrequency()<<std::endl;

		a = getTickCount();

        for(int i=0; i<N_VIDEOS; i++) {
            TrackingObj::objectDetection(filter[i],frame[i], i, paint);		// Hacemos el tracking de los elementos del campo
        }

		b = getTickCount();

		std::cout<<"Detection:  "<<(b-a)/getTickFrequency()<<std::endl;

		a = getTickCount();

		// Obtención de posiciones 2D de jugadores detectados
		for(int i=0; i<N_VIDEOS; i++) {
			if(GlobalStats::locations[i].size()) {
				GlobalStats::locations[i] = From3DTo2D::get2DPositionVector(GlobalStats::locations[i],i);
			}
		}

		b = getTickCount();

		std::cout<<"Pos2D:      "<<(b-a)/getTickFrequency()<<std::endl;

		a = getTickCount();

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
					//if(GlobalStats::detectedPlayers.size()<=25) {
						GlobalStats::detectedPlayers.push_back(*it1);
					//}
				}
			}
		}

		b = getTickCount();

		std::cout<<"Duplicados: "<<(b-a)/getTickFrequency()<<std::endl;

		a = getTickCount();

		GlobalStats::addStats();

		GlobalStats::clearLocations();

        Mat join = VideoManager::joinSequences(frame);

		pyrDown(join, join, Size(join.cols/2, join.rows/2));
		
		pyrDown(join, join, Size(join.cols/2, join.rows/2));

        imshow("2D FIELD",paint);
		imshow(VIDEO_W, join);

		//outputVideo2D<<paint;
		//outputVideoCams<<join;

		b = getTickCount();

		std::cout<<"Resto:      "<<(b-a)/getTickFrequency()<<std::endl;

		end_time = getTickCount();

		std::cout<<"TOTAL:      "<<(end_time-init_time)/getTickFrequency()<<std::endl;
		std::cout<<"--------------------------------------"<<std::endl;
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

	return 0;

}
