#include "config_file.h"
#include "VideoManager.h"
#include "TrackingObj.h"
#include "GUI.h"
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

	From3DTo2D::initProjectionMatrices();	// Inicializamos las matrices de proyección

	for(int i=0; i<N_VIDEOS; i++) {			// Inicializamos imágenes de bg y máscaras
		std::stringstream path;
		path << VIDEO_PATH << i << BG_FORMAT;
		bg[i] = imread(path.str());

		FieldFilter::initFilterMask(i);
	}

	VideoWriter outputVideo2D,outputVideoCams;
	if(SAVE_RESULT_SEQ) {
		Size size1 = Size(SOCCER_FIELD_WIDTH,SOCCER_FIELD_HEIGHT);
		Size size2 = Size(VIDEO_WIDTH*3/2,(VIDEO_HEIGHT*2+4)/2);
		outputVideo2D.open("video/field2D_out_2.avi", -1, 25, size1, true);
		outputVideoCams.open("video/sequences_out_2.avi", -1, 25, size2, true);

		if (!outputVideo2D.isOpened() && !outputVideoCams.isOpened())
		{
			return -1;
		}
	}

	/*	
	*	Bucle en el que vamos pasando los frames del video con la función nextFrame,
    *   que coge el frame actual del video y lo guarda en la matriz frame. Cuando nextFrame
	*	devuelva false, el vídeo abrá acabado.
	*/
	while(VideoManager::nextFrame(frame) && waitKey(1)!=27) {

		int64 init_time,end_time;
		init_time = getTickCount();

		Mat paint;
		From3DTo2D::field2D.copyTo(paint);

		int64 a,b;
		
		a = getTickCount();

		for(int i=0; i<N_VIDEOS; i++) {
			filter[i] = FieldFilter::discardField(frame[i].clone(), bg[i], i);	// Filtramos el campo en el frame
		}

		b = getTickCount();

		std::cout<<"Filtro    : "<<(b-a)/getTickFrequency()<<std::endl;

		a = getTickCount();

		// Hacemos tracking del jugador
		for(vector<Player>::iterator itP = GlobalStats::playerV.begin(); itP!=GlobalStats::playerV.end(); itP++) {
			TrackingObj::trackPlayers(frame,filter,&*itP);
		}

		b = getTickCount();

		std::cout<<"   +Tracking:   "<<(b-a)/getTickFrequency()<<std::endl;
		
		a = getTickCount();
		// Borrado de jugadores descartados
		for(vector<Player*>::iterator it = GlobalStats::playersToDelete.begin();
				it!=GlobalStats::playersToDelete.end(); it++) {
			vector<Player>::iterator itP = std::find(GlobalStats::playerV.begin(),GlobalStats::playerV.end(),**it);
			if(itP != GlobalStats::playerV.end()) {
				GlobalStats::playerV.erase(itP);
			}
		}
		GlobalStats::playersToDelete.clear();

		b = getTickCount();

		std::cout<<"Borrado:    "<<(b-a)/getTickFrequency()<<std::endl;
		
		/*a = getTickCount();

		for(vector<Player>::iterator itP = GlobalStats::playerV.begin(); itP!=GlobalStats::playerV.end(); itP++) {
			for(int i=0; i<N_VIDEOS; i++) {
				if(itP->getBPos(i)) {
					Rect rectP = GlobalStats::getPlayerRect(itP->getCamPos(i));
					if(TrackingObj::isInRange(&rectP)) {
						filter[i](rectP).setTo(0);
					}
				}
			}
		}
		b = getTickCount();

		std::cout<<"Cleaning:   "<<(b-a)/getTickFrequency()<<std::endl;
		*/
		//a = getTickCount();

		/*for(int i=GlobalStats::playersToDelete.size()-1; i>=0; i--) {
			int k = GlobalStats::playersToDelete.at(i);
			GlobalStats::detectedPlayers.erase(GlobalStats::detectedPlayers.begin()+k-1);
			GlobalStats::playersToDelete.pop_back();
		}*/

		//b = getTickCount();

		//std::cout<<"Deleting:   "<<(b-a)/getTickFrequency()<<std::endl;

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
				for(unsigned j=0; j<GlobalStats::locations[i].size(); j++) {
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
					if(!found && From3DTo2D::isInRange(*it1) && !GlobalStats::alreadyDetected(*it1) && !GlobalStats::allPlayersDetected()) {
						PlayerClassifier::addPlayer(frame[i],filter[i],*it1);
					}
				}
			}
		}

		b = getTickCount();

		std::cout<<"Duplicados: "<<(b-a)/getTickFrequency()<<std::endl;

		a = getTickCount();

		//Pintar jugadores
		for(vector<Player>::iterator it = GlobalStats::playerV.begin(); it!=GlobalStats::playerV.end(); it++) {
			Point p = it->getPosition();
			Scalar colour;
			bool exists = false;
			for(int k=0; k<N_VIDEOS; k++) {
				Point realP;
				if(it->getBPos(k)) {
					realP = it->getCamPos(k);
				} else {
					realP = From3DTo2D::getRealPosition(p,k);
				}
				if (TrackingObj::isInFocus(realP)) {
					Rect paintR = GlobalStats::getPlayerRect(realP);
					if(TrackingObj::isInRange(&paintR)) {
						exists = true;
						colour = mean(frame[k](paintR), filter[k](paintR));
						rectangle(frame[k],paintR,colour,2);
						std::stringstream text;
						text << it->getPlayerId();
						putText(frame[k],text.str(),Point(realP.x-15,realP.y+15),0,0.5,colour,2);
					}
				}
			}
			if(exists) {
				circle(paint,p,3,colour,2);
			}
		}
		
		b = getTickCount();

		std::cout<<"Pintado   : "<<(b-a)/getTickFrequency()<<std::endl;

		a = getTickCount();

		GlobalStats::clearLocations();

		Mat join = VideoManager::joinSequences(frame);

		pyrDown(join, join, Size(join.cols/2, join.rows/2));

		if(SAVE_RESULT_SEQ) {
			outputVideo2D<<paint;
			outputVideoCams<<join;
		} else {
			imshow("2D FIELD",paint);
			imshow(VIDEO_W, join);
		}

		b = getTickCount();

		std::cout<<"Resto:      "<<(b-a)/getTickFrequency()<<std::endl;

		end_time = getTickCount();

		std::cout<<"TOTAL:      "<<(end_time-init_time)/getTickFrequency()<<
			" ("<<0.04*FPS/((end_time-init_time)/getTickFrequency())<<")"<<std::endl;
		std::cout<<"PLAYERS:    "<<GlobalStats::totalPlayers()<<std::endl;
		for(vector<Player>::iterator it = GlobalStats::playerV.begin();
			it != GlobalStats::playerV.end(); it ++) {
			std::cout<<it->getPlayerId()<<" ";
		}
		std::cout<<std::endl;
		std::cout<<"TO DELETE : "<<GlobalStats::playersToDelete.size()<<std::endl;
		std::cout<<"--------------------------------------"<<std::endl;
	}

	destroyAllWindows();

	StatsAnalyzer::calculateAllStats();

	int command = 0;
	while(command != 27) {
		GUI::showStatsWindow();
		if(command == 13) {
			if(GUI::nPlayer < GlobalStats::totalPlayers()-1) {
				GUI::nPlayer++;
			} else {
				GUI::nPlayer = 0;
			}
			std::cout<<"Player: "<<GUI::nPlayer<<std::endl;
		}
		command = waitKey(1);
	}

	return 0;

}
