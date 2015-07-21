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
	From3DTo2D::initProjectionMatrices();	// Inicializamos las matrices de proyección
	FieldFilter::initFilter();				// Inicializamos imágenes de bg y máscaras

	Mat paint;                              // Utilizada para el pintado de los resultados

	// DEBUG!!!
	// Guardamos los resultados de la ejecución
	VideoWriter outputVideo2D,outputVideoCams;
	if(SAVE_RESULT_SEQ) {
		Size size1 = Size(SOCCER_FIELD_WIDTH,SOCCER_FIELD_HEIGHT);
		Size size2 = Size(VIDEO_WIDTH*3/2,(VIDEO_HEIGHT*2+4)/2);
		outputVideo2D.open("video/field2D_out.avi", -1, 25, size1, true);
		outputVideoCams.open("video/sequences_out.avi", -1, 25, size2, true);

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
	while(VideoManager::nextFrame(GlobalStats::frame) && waitKey(1)!=27) {

		// DEBUG!!!
		int64 init_time,end_time,a,b;
		init_time = getTickCount();

		// Cargamos el campo 2D en field
		GlobalStats::field2D.copyTo(paint);
		
		a = getTickCount();

		// Filtramos el campo en el frame
		FieldFilter::discardField();

		b = getTickCount();

		std::cout<<"Filtro    : "<<(b-a)/getTickFrequency()<<std::endl;

		a = getTickCount();

		// Hacemos tracking de cada jugador
		TrackingObj::trackPlayers();

		b = getTickCount();

		std::cout<<"Tracking  : "<<(b-a)/getTickFrequency()<<std::endl;

		// Comprobamos el estado de la lista de los jugadores a borrar
		PlayerClassifier::checkPlayersToDelete();
		PlayerClassifier::checkDuplicatedPlayers();
		
		a = getTickCount();
		// Eliminamos del filtro los jugadores ya trackeados para que no sean detectados de nuevo
		for(vector<Player>::iterator itP = PlayerClassifier::playerV.begin(); itP!=PlayerClassifier::playerV.end(); itP++) {
			for(int i=0; i<N_VIDEOS; i++) {
				if(itP->getBPos(i)) {
					Rect rectP = GlobalStats::getPlayerRect(itP->getCamPos(i));
					if(TrackingObj::isInRange(&rectP)) {
						GlobalStats::filter[i](rectP).setTo(0);
					}
				}
			}
		}
		b = getTickCount();

		std::cout<<"Cleaning:   "<<(b-a)/getTickFrequency()<<std::endl;
		

		a = getTickCount();

        if(!GlobalStats::allPlayersDetected()) {
			// Se lleva a cabo la detección de los elementos del campo
			PlayerClassifier::objectDetection();

			b = getTickCount();

			std::cout<<"Detection : "<<(b-a)/getTickFrequency()<<std::endl;

			a = getTickCount();

			// Obtención de posiciones 2D de localizaciones obtenidas
			From3DTo2D::calculateLocations2D();

			b = getTickCount();

			std::cout<<"Pos2D     : "<<(b-a)/getTickFrequency()<<std::endl;

			a = getTickCount();

			// Eliminación de duplicidades (elementos localizados en varias cámaras)
			PlayerClassifier::removeDuplications();
		}

		b = getTickCount();

		std::cout<<"Duplicados: "<<(b-a)/getTickFrequency()<<std::endl;

		a = getTickCount();

		//Pintar jugadores
		for(vector<Player>::iterator it = PlayerClassifier::playerV.begin(); it!=PlayerClassifier::playerV.end(); it++) {
			Point p = it->getLastPosition();
			Scalar colour = GlobalStats::calculateColor(it->getHistogram());
			bool exists = false;
			for(int k=0; k<N_VIDEOS; k++) {
				Point realP;
				if(it->getBPos(k)) {
					realP = it->getCamPos(k);
				} else {
					realP = From3DTo2D::getCameraPosition(p,k);
				}
				if (TrackingObj::isInFocus(realP)) {
					Rect paintR = GlobalStats::getPlayerRect(realP);
					if(TrackingObj::isInRange(&paintR)) {
						exists = true;
						rectangle(GlobalStats::frame[k],paintR,colour,2);
					}
				}
			}
			if(exists) {
				circle(paint,p,3,colour,2);
			}
		}
		
		b = getTickCount();

		std::cout<<"Painting  : "<<(b-a)/getTickFrequency()<<std::endl;

		a = getTickCount();

		// Limpiamos el vector de posiciones
		GlobalStats::clearLocations();
		// Unimos las secuencias en una sola para mostrarlas
		Mat join = VideoManager::joinSequences(GlobalStats::frame);
		pyrDown(join, join, Size(join.cols/2, join.rows/2));

		// DEBUG!!!
		if(SAVE_RESULT_SEQ) {
			outputVideo2D<<paint;
			outputVideoCams<<join;
		} else {
			imshow(FIELD_W,paint);
			imshow(VIDEO_W, join);
		}

		b = getTickCount();

		std::cout<<"Resto     : "<<(b-a)/getTickFrequency()<<std::endl;

		end_time = getTickCount();

		std::cout<<std::endl;
		std::cout<<"TOTAL     : "<<(end_time-init_time)/getTickFrequency()<<
			" ("<<0.04*FPS/((end_time-init_time)/getTickFrequency())<<")"<<std::endl;
		std::cout<<"PLAYERS   : "<<GlobalStats::totalPlayers()<<std::endl;
		std::cout<<"LAST ID   : "<<Player::id<<std::endl;
		std::cout<<"TO DELETE : "<<PlayerClassifier::playersToDelete.size()<<std::endl;
		std::cout<<"--------------------------------------"<<std::endl;

	}

	destroyAllWindows();
	// Hacemos el cálculo de las estadísticas a partir de los datos recopilados
	StatsAnalyzer::calculateAllStats();

	/*
	* Interfaz de usuario en la que se tiene acceso a las estadísticas calculadas
	* anteriormente. Si se pulsa ENTER se mostrará el siguiente jugador, si se pulsa
	* ESC, se finalizará la ejecución.
	*/
	int command = 0;
	while(command != 27) {
		GUI::showStatsWindow();
		if(command == 13) {
			if(GUI::nPlayer < GlobalStats::totalPlayers()-1) {
				GUI::nPlayer++;
			} else {
				GUI::nPlayer = 0;
			}
		}
		command = waitKey(1);
	}

	return 0;

}
