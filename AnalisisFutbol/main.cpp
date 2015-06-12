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

	// DEBUG!!!
	// Guardamos los resultados de la ejecución
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
	*	devuelva false, la secuencia abrá acabado. También finaliza si se pulsa ESC.
	*/
	while(VideoManager::nextFrame(frame) && waitKey(1)!=27) {

		// DEBUG!!!
		int64 init_time,end_time,a,b;
		init_time = getTickCount();

		// Cargamos el campo 2D en field
		Mat field;
		From3DTo2D::field2D.copyTo(field);
		
		a = getTickCount();

		// Filtramos el campo en el frame
		for(int i=0; i<N_VIDEOS; i++) {
			filter[i] = FieldFilter::discardField(frame[i].clone(), bg[i], i);
		}

		b = getTickCount();

		std::cout<<"Filtro    : "<<(b-a)/getTickFrequency()<<std::endl;

		a = getTickCount();

		// Hacemos tracking de cada jugador
		for(vector<Player>::iterator itP = GlobalStats::playerV.begin(); itP!=GlobalStats::playerV.end(); itP++) {
			TrackingObj::trackPlayers(frame,filter,&*itP);
		}

		b = getTickCount();

		std::cout<<"Tracking  : "<<(b-a)/getTickFrequency()<<std::endl;
		
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

		std::cout<<"Deleting  : "<<(b-a)/getTickFrequency()<<std::endl;
		
		/*a = getTickCount();
		// Eliminamos del filtro los jugadores ya trackeados para que no sean detectados de nuevo
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

		a = getTickCount();

        if(!GlobalStats::allPlayersDetected()) {
			// Se lleva a cabo la detección de los elementos del campo
			for(int i=0; i<N_VIDEOS; i++) {
				TrackingObj::objectDetection(filter[i],frame[i], i);
			}

			b = getTickCount();

			std::cout<<"Detection : "<<(b-a)/getTickFrequency()<<std::endl;

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

			std::cout<<"Pos2D     : "<<(b-a)/getTickFrequency()<<std::endl;

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
			Scalar colour = GlobalStats::calculateColor(it->getHistogram());
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
						rectangle(frame[k],paintR,colour,2);
						std::stringstream text;
						text << it->getPlayerId();
						putText(frame[k],text.str(),Point(realP.x-15,realP.y+15),0,0.5,colour,2);
					}
				}
			}
			if(exists) {
				circle(field,p,3,colour,2);
			}
		}
		
		b = getTickCount();

		std::cout<<"Painting  : "<<(b-a)/getTickFrequency()<<std::endl;

		a = getTickCount();

		// Limpiamos el vector de posiciones
		GlobalStats::clearLocations();
		// Unimos las secuencias en una sola para mostrarla
		Mat join = VideoManager::joinSequences(frame);
		pyrDown(join, join, Size(join.cols/2, join.rows/2));

		// DEBUG!!!
		if(SAVE_RESULT_SEQ) {
			outputVideo2D<<field;
			outputVideoCams<<join;
		} else {
			imshow(FIELD_W,field);
			imshow(VIDEO_W, join);
		}

		b = getTickCount();

		std::cout<<"Resto     : "<<(b-a)/getTickFrequency()<<std::endl;

		end_time = getTickCount();

		std::cout<<std::endl;
		std::cout<<"TOTAL     : "<<(end_time-init_time)/getTickFrequency()<<
			" ("<<0.04*FPS/((end_time-init_time)/getTickFrequency())<<")"<<std::endl;
		std::cout<<"PLAYERS   : "<<GlobalStats::totalPlayers()<<std::endl;
		std::cout<<"LAST ID   : "<<GlobalStats::playerV.at(GlobalStats::totalPlayers()-1).getPlayerId()<<std::endl;
		std::cout<<"TO DELETE : "<<GlobalStats::playersToDelete.size()<<std::endl;
		std::cout<<"--------------------------------------"<<std::endl;
	}

	destroyAllWindows();
	// Hacemos el cálculo de las estadísticas a partir de los datos recopilados
	StatsAnalyzer::calculateAllStats();

	int command = 0;
	/*
	* Interfaz de usuario en la que se tiene acceso a las estadísticas calculadas
	* anteriormente. Si se pulsa ENTER se mostrará el siguiente jugador, si se pulsa
	* ESC, se finalizará la ejecución.
	*/
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
