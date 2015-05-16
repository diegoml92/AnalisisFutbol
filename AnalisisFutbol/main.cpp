#include "config_file.h"
#include "VideoManager.h"
#include "TrackingObj.h"
#include "GUI.h"
#include "EventManager.h"
#include "FieldFilter.h"
#include "From3DTo2D.h"
#include "StatsAnalyzer.h"
#include "GlobalStats.h"
#include "PlayerClassifier.h"

/* FUNCIÓN DE ENTRADA AL PROGRAMA */
int main(int argc, char* argv[]) {

	VideoManager::init();					// Cargamos el vídeo que vamos a utilizar

	Mat frame[N_VIDEOS];					// Irá almacenando cada fotograma del vídeo de entrada
	Mat filter[N_VIDEOS];					// Almacenará el umbral actualizado según los valores del filtro
	Mat bg[N_VIDEOS];						// Almacenará los backgrounds de cada secuencia

	for(int i=0; i<N_VIDEOS; i++) {
		std::stringstream path;
		path << VIDEO_PATH << i << BG_FORMAT;
		bg[i] = imread(path.str());
	}

	VideoWriter outputVideo2D,outputVideoCams;
	if(SAVE_RESULT_SEQ) {
		Size size1 = Size(SOCCER_FIELD_WIDTH,SOCCER_FIELD_HEIGHT);
		Size size2 = Size(VIDEO_WIDTH*3/4,(VIDEO_HEIGHT*2+8)/4);
		outputVideo2D.open("video/field2D_out_2.avi", -1, 25, size1, true);
		outputVideoCams.open("video/sequences_out_2.avi", -1, 25, size2, true);

		if (!outputVideo2D.isOpened() && !outputVideoCams.isOpened())
		{
			return -1;
		}
	}

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
			filter[i] = FieldFilter::discardField(frame[i].clone(), bg[i]);	// Filtramos el campo en el frame
		}

		b = getTickCount();

		std::cout<<"Filtro    : "<<(b-a)/getTickFrequency()<<std::endl;

		a = getTickCount();
        
		// Hacemos tracking del jugador
		int indexforthis = 0;
		for(vector<Team>::iterator it = GlobalStats::teams.begin(); it!=GlobalStats::teams.end(); it++) {
			a_1 = getTickCount();
			vector<Player>* playerV = it->getPlayersP();
			for(vector<Player>::iterator itP = playerV->begin(); itP!=playerV->end(); itP++) {
				TrackingObj::trackPlayers(frame,filter,&*itP,indexforthis);
			}
			b_1 = getTickCount();
			indexforthis++;
		}

		b = getTickCount();

		std::cout<<"Meanshit:   "<<(b-a)/getTickFrequency()<<std::endl;
		std::cout<<"   +Tracking:   "<<(b_1-a_1)/getTickFrequency()<<std::endl;

		a = getTickCount();

		/*for(int i=GlobalStats::playersToDelete.size()-1; i>=0; i--) {
			int k = GlobalStats::playersToDelete.at(i);
			GlobalStats::detectedPlayers.erase(GlobalStats::detectedPlayers.begin()+k-1);
			GlobalStats::playersToDelete.pop_back();
		}*/

		b = getTickCount();

		std::cout<<"Deleting:   "<<(b-a)/getTickFrequency()<<std::endl;

		a = getTickCount();

        if(!GlobalStats::allPlayersDetected()) {
			for(int i=0; i<N_VIDEOS; i++) {
				TrackingObj::objectDetection(filter[i],frame[i], i, paint);		// Hacemos el tracking de los elementos del campo
			}

			b = getTickCount();

			std::cout<<"Detection:  "<<(b-a)/getTickFrequency()<<std::endl;

			a = getTickCount();

			vector<Point2f> locations2D [N_VIDEOS];
			// Obtención de posiciones 2D de jugadores detectados
			for(int i=0; i<N_VIDEOS; i++) {
				for(int j=0; j<GlobalStats::locations[i].size(); j++) {
					locations2D[i].push_back(GlobalStats::getCenter(GlobalStats::locations[i].at(j)));
				}
				if(GlobalStats::locations[i].size()) {
					locations2D[i] = From3DTo2D::get2DPositionVector(locations2D[i],i);
				}
			}

			b = getTickCount();

			std::cout<<"Pos2D:      "<<(b-a)/getTickFrequency()<<std::endl;

			a = getTickCount();

			// Eliminación de duplicidades (elementos localizados en varias cámaras)
			for(int i=0; i<N_VIDEOS; i++) {
				for(vector<Point2f>::iterator it1=locations2D[i].begin(); it1!=locations2D[i].end(); ++it1) {
					bool found = false;
					for(int j=i+1; j<N_VIDEOS;j++) {
						for(vector<Point2f>::iterator it2=locations2D[j].begin(); it2!=locations2D[j].end(); ++it2) {
							if(StatsAnalyzer::isSamePoint(*it1, *it2)) {
								found = true;
								break;
							}
						}
					}
					if(!found && From3DTo2D::isInRange(*it1) && !GlobalStats::alreadyDetected(*it1) && !GlobalStats::totalPlayers()<35) {
						PlayerClassifier::addPlayer(frame[i],filter[i],*it1);
					}
				}
			}
		}

		b = getTickCount();

		std::cout<<"Duplicados: "<<(b-a)/getTickFrequency()<<std::endl;

		a = getTickCount();

		for(int i=0; i<GlobalStats::teams.size(); i++) {
			Team t = GlobalStats::teams.at(i);
			vector<Player> playerV = t.getPlayers();
			for(vector<Player>::iterator it = playerV.begin(); it!=playerV.end(); it++) {
				if(GlobalStats::colors.size()<(i+1)) {
					RNG rng(getTickCount());
					GlobalStats::colors.push_back(Scalar(rng.uniform(0,256),rng.uniform(0,256),rng.uniform(0,256)));
				}
				Scalar colour = GlobalStats::colors.at(i);
				Point p = it->getPosition();
				circle(paint,p,3,colour,2);
				for (int k=0; k<N_VIDEOS; k++) {
					Point realP = From3DTo2D::getRealPosition(p,k);
					Rect paintR(Point(realP.x - PLAYER_WIDTH/2, realP.y - PLAYER_HEIGHT),
								Point(realP.x + PLAYER_WIDTH/2, realP.y));
					rectangle(frame[k],paintR,colour,2);
				}
			}
		}
		
		b = getTickCount();

		std::cout<<"Pintado   : "<<(b-a)/getTickFrequency()<<std::endl;

		a = getTickCount();

		GlobalStats::clearLocations();

        Mat join = VideoManager::joinSequences(frame);

		pyrDown(join, join, Size(join.cols/2, join.rows/2));

        imshow("2D FIELD",paint);
		imshow(VIDEO_W, join);

		if(SAVE_RESULT_SEQ) {
			outputVideo2D<<paint;
			outputVideoCams<<join;
		}

		b = getTickCount();

		std::cout<<"Resto:      "<<(b-a)/getTickFrequency()<<std::endl;

		end_time = getTickCount();

		std::cout<<"TOTAL:      "<<(end_time-init_time)/getTickFrequency()<<std::endl;
		std::cout<<"PLAYERS:    "<<GlobalStats::totalPlayers()<<std::endl;
		std::cout<<"TEAMS:      "<<GlobalStats::teams.size()<<std::endl;
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
