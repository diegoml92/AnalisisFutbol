#include "PlayerClassifier.h"
#include "GUI.h"
#include "TrackingObj.h"
#include "From3DTo2D.h"
#include "GlobalStats.h"
#include "StatsAnalyzer.h"

vector<Player> PlayerClassifier::playerV;
std::list<Player> PlayerClassifier::playersToDelete;
std::unordered_map<string,int> PlayerClassifier::duplicatedPlayers;

/* DETERMINA SI EL PUNTO ESTÁ CERCA DEL BORDE DEL CAMPO */
bool PlayerClassifier::isFieldEdge(Point p) {
	return p.x < 20  || p.x > SOCCER_FIELD_WIDTH - 20;
}

/* AÑADE UN JUGADOR */
void PlayerClassifier::addPlayer(Mat frame, Mat filter, Point position) {
	vector<Rect> rects;
	// Descartamos si está demasiado cerca del borde del campo para evitar ruido
	if(!PlayerClassifier::isFieldEdge(position)) {
		for(int i=0; i<N_VIDEOS; i++) {
			Point realPos = From3DTo2D::getCameraPosition(position,i);
			if(TrackingObj::isInFocus(realPos)) {
				Rect playerBox = GlobalStats::getPlayerRect(realPos);
				if(TrackingObj::isInRange(&playerBox) && isPlayerSize(playerBox) && canBePlayer(filter(playerBox))) {
					rects.push_back(playerBox);
				}
			}
		}
		if(rects.size()>0) {
			vector<Mat> hist;
			calculateHistogram(frame, filter, rects, &hist);
			// No se ha asociado a ningún jugador perdido anteriormente
			if(!PlayerClassifier::recoverPlayer(position,hist)) {
				PlayerClassifier::playerV.push_back(Player(position, hist));
			}
		}
	}
}

/* DETERMINA SI UN ELEMENTO YA HA SIDO DETECTADO */
bool PlayerClassifier::alreadyDetected(Point p) {
	bool found = false;
	int i=0;
	vector<Player>::iterator it = playerV.begin();
	while(!found && it!=playerV.end()) {
		found = StatsAnalyzer::isSamePoint(p,it->getLastPosition());
		it++;
	}
	return found;
}

/* CALCULA EL HISTOGRAMA DEL JUGADOR */
void PlayerClassifier::calculateHistogram(Mat frame, Mat filter, vector<Rect> rects, vector<Mat>* hist_v) {

	int channel_B [] = {0};
	int channel_G [] = {1};
	int channel_R [] = {2};
	int nBins = N_BINS;
	float range [] = {0,RGB};
	const float *ranges = {range};
	Mat hist[N_CHANNELS];
	for(int i=0; i<rects.size(); i++) {
		Mat mask = filter(rects[i]);
		Mat src = frame(rects.at(i));
		calcHist(&src,1,channel_B,mask,hist[0],1,&nBins,&ranges,true,true);
		calcHist(&src,1,channel_G,mask,hist[1],1,&nBins,&ranges,true,true);
		calcHist(&src,1,channel_R,mask,hist[2],1,&nBins,&ranges,true,true);
	}
	if(hist_v->empty()) {
		for(int i=0 ; i<N_CHANNELS; i++) {
			hist_v->push_back(hist[i]);
		}
	}
}

/* CALCULA EL HISTOGRAMA DEL JUGADOR */
vector<Mat> PlayerClassifier::calculateHistogram(Point pos, int nCam) {
	vector<Rect> v;
	Rect* rect = &(GlobalStats::getPlayerRect(pos));
	TrackingObj::isInRange(rect);
	v.push_back(*rect);
	vector<Mat> hist;
	calculateHistogram(GlobalStats::frame[nCam],GlobalStats::filter[nCam],v,&hist);
	return hist;
}

/* COMPARA LOS HISTOGRAMAS DE LOS JUGADORES */
double PlayerClassifier::compareHistogram(vector<Mat> playerHist, vector<Mat> newHist) {
	double diff = 0;
	for(int i=0; i<N_CHANNELS; i++) {
		diff += compareHist(playerHist.at(i),newHist.at(i),CV_COMP_BHATTACHARYYA);
	}
	return diff;
}

/* DETERMINA SI CUMPLE EL TAMAÑO PROPIO DE UN JUGADOR */
bool PlayerClassifier::isPlayerSize(Rect player) {
	return player.height>MIN_HEIGH && player.height<MAX_HEIGH
			&& player.width>MIN_WIDTH && player.width<MAX_WIDTH;
}

/* DETERMINA CUÁL DE LOS ELEMENTOS ASIGNADOS A UN JUGADOR ES EL CORRECTO */
Point PlayerClassifier::findBestMatch(Player* player) {
	float min = N_CHANNELS; // La distancia bhattacharya es mámixo 1; Como trabajamos con
							// cada canal por separado, el máximo será 3.
	int cam = -1;
	for(int i=0; i<N_VIDEOS; i++) {
		if (player->getBPos(i)) {
			float res = PlayerClassifier::compareHistogram(player->getHistogram(),
				PlayerClassifier::calculateHistogram(player->getCamPos(i), i));
			if(res < min) {
				min = res;
				cam = i;
			}
		}
	}
	// Desactivamos todas las localizaciones excepto la de la mejor opcion
	for(int i=0; i<N_VIDEOS; i++) {
		if (player->getBPos(i) && i!=cam) {
			player->unSetCamPos(i);
		}
	}
	return From3DTo2D::get2DPosition(player->getCamPos(cam),cam);
}

/* DESCARTA FALSOS POSITIVOS (LÍNEAS, ETC) EN FUNCIÓN DE EL NÚMERO DE PÍXELES BLANCOS */
bool PlayerClassifier::canBePlayer(Mat roi, float val) {
	return countNonZero(roi) / (float)(roi.cols*roi.rows) >= val;
}

/* REALIZA EL SEGUIMIENTO DE LOS ELEMENTOS DEL PARTIDO */
void PlayerClassifier::objectDetection() {
	RNG rng(12345);
	for(int i=0; i<N_VIDEOS; i++) {
		Mat temp;		// Matriz auxiliar en la que buscaremos los contornos
		GlobalStats::filter[i].copyTo(temp);
		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE);
		if(hierarchy.size() > 0) {									// Si se encuentra algún contorno

			/*Mat drawing = Mat::zeros( temp.size(), CV_8UC3 );
			Mat drawing2 = Mat::zeros( temp.size(), CV_8UC3 );
			Mat boundingBox = Mat::zeros( temp.size(), CV_8UC3 );*/

			for(int j = 0; j < contours.size(); j++ ) {				// Recorremos los contornos
				Rect elem = boundingRect(Mat(contours[j]));			// Creamos el boundingBox
				if(PlayerClassifier::isPlayerSize(elem) &&
					PlayerClassifier::canBePlayer(GlobalStats::filter[i](elem))) {
					GlobalStats::locations[i].push_back(elem);		// Añadimos al vector de localizaciones

					/*Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
					drawContours (drawing, contours, j, color, 2, 8, hierarchy, 0, Point() );
					drawContours (drawing2, contours, j, color, 2, 8, hierarchy, 0, Point() );
					drawContours (boundingBox, contours, j, color, 2, 8, hierarchy, 0, Point() );
					rectangle(boundingBox, elem, color);*/
				} /*else {
					Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
					drawContours (drawing2, contours, j, color, 2, 8, hierarchy, 0, Point() );
				}*/
			}

			/*imshow("ORIGINAL",GlobalStats::frame[i]);
			imshow("FOREGROUND",GlobalStats::filter[i]);
			imshow("CONTORNOS1",drawing2);
			imshow("CONTORNOS2",drawing);
			imshow("BOUNDING_BOX",boundingBox);
			waitKey();*/
		}
	}
}

/* ELIMINA LAS DUPLICIDADES EN LAS DETECCIONES */
void PlayerClassifier::removeDuplications() {
	for(int i=0; i<N_VIDEOS; i++) {
		for(vector<Point2f>::iterator it1=GlobalStats::locations2D[i].begin(); it1!=GlobalStats::locations2D[i].end(); ++it1) {
			bool found = false;
			for(int j=i+1; j<N_VIDEOS;j++) {
				for(vector<Point2f>::iterator it2=GlobalStats::locations2D[j].begin(); it2!=GlobalStats::locations2D[j].end(); ++it2) {
					if(StatsAnalyzer::isSamePoint(*it1, *it2)) {
						found = true;
						break;
					}
				}
			}
			if(!found && From3DTo2D::isInRange(*it1) && !PlayerClassifier::alreadyDetected(*it1) && !GlobalStats::allPlayersDetected()) {
				PlayerClassifier::addPlayer(GlobalStats::frame[i],GlobalStats::filter[i],*it1);
			}
		}
	}
}

/*
* AÑADE UN JUGADOR A LA LISTA DE BORRADO 
* Y LO BORRA DE LA LISTA DE JUGADORES ACTUALES
*/
void PlayerClassifier::addPlayerToDelete(vector<Player>::iterator* it) {
	Player player(**it);
	player.startDeletionCounter();
	PlayerClassifier::playersToDelete.push_back(player);
	*it = PlayerClassifier::playerV.erase(*it);
}

/* COMPRUEBA LOS JUGADORES A BORRAR */
void PlayerClassifier::checkPlayersToDelete() {
	std::list<Player>::iterator it = playersToDelete.begin();
	while (it!=playersToDelete.end()) {
		it->increaseDeletionCounter();
		if(it->getDeletionCounter() > TIME_TO_DELETE * FPS) {
			it = PlayerClassifier::playersToDelete.erase(it);
		} else {
			it++;
		}
	}
}

/* ELIMINA LOS ELEMENTOS DUPLICADOS */
void PlayerClassifier::deleteDuplicatedPlayer(string key, Point position, Player p1, Player p2, vector<Player>* toDelete) {
	vector<Rect> rects;
	vector<Mat> hist;
	for(int i=0; i<N_VIDEOS; i++) {
		Point realPos = From3DTo2D::getCameraPosition(position,i);
		if(TrackingObj::isInFocus(realPos)) {
			Rect playerBox = GlobalStats::getPlayerRect(realPos);
			if(TrackingObj::isInRange(&playerBox)) {
				rects.push_back(playerBox);
			}
		}
		if(rects.size()>0) {
			calculateHistogram(GlobalStats::frame[i], GlobalStats::filter[i], rects, &hist);
		}
	}
	float diff1, diff2;
	diff1 = compareHistogram(hist,p1.getHistogram());
	diff2 = compareHistogram(hist,p2.getHistogram());
	if(diff1 <= BHATTACHARYYA_DISTANCE*N_CHANNELS) {
		if(diff2 <= BHATTACHARYYA_DISTANCE*N_CHANNELS) {
			//Ambos válidos, borrar el menos antiguo
			if(p1.getPlayerId() < p2.getPlayerId()) {
				(*toDelete).push_back(p2);
			} else {
				(*toDelete).push_back(p1);
			}
		} else {
			(*toDelete).push_back(p2);
		}
	} else if(diff2 <= BHATTACHARYYA_DISTANCE*N_CHANNELS) {
		(*toDelete).push_back(p1);
	} else {
		//Ninguno válido, borrar el menos antiguo
		if(p1.getPlayerId() < p2.getPlayerId()) {
			(*toDelete).push_back(p2);
		} else {
			(*toDelete).push_back(p1);
		}
	}
}

/* LOCALIZA LOS JUGADORES DUPLICADOS */
void PlayerClassifier::checkDuplicatedPlayers() {
	std::unordered_map<string,int>::iterator entry;
	vector<Player>toDelete;
	// Comprobamos si existen duplicidades
	for (std::vector<Player>::iterator it1 = playerV.begin(); it1!=playerV.end(); it1++) {
		for(std::vector<Player>::iterator it2 = it1 + 1; it2!=playerV.end(); it2++) {
			std::stringstream key;
			key<<it1->getPlayerId()<<"-"<<it2->getPlayerId();
			entry = duplicatedPlayers.find(key.str());
			// Si se encuentra un posible duplicado
			if(StatsAnalyzer::distance(it1->getPosition(), it2->getPosition()) < MIN_DISTANCE) {
				// Si ya existe, incrementamos el contador
				if(entry != duplicatedPlayers.end()) {
					int value = ++duplicatedPlayers.at(key.str());
					// Si supera en límite lo borramos
					if(value >= TIME_DUPLICATES*FPS) {
						Point p1 = it1->getPosition();
						Point p2 = it2->getPosition();
						Point p = p1 + p2;
						p = Point(p.x/2,p.y/2);
						PlayerClassifier::deleteDuplicatedPlayer(key.str(),p,*it1,*it2,&toDelete);
						duplicatedPlayers.erase(entry);
					}
				// Si no, añadimos una nueva entrada
				} else {
					std::pair<string,int> elem (key.str(), 1);
					duplicatedPlayers.insert(elem);
				}
			} else {
				// Si no es duplicado lo borramos de la lista (si está)
				if(entry != duplicatedPlayers.end()) {
					duplicatedPlayers.erase(entry);
				}
			}
		}
	}
	for(int i=0; i<toDelete.size(); i++) {
		vector<Player>::iterator it = std::find(playerV.begin(), playerV.end(), toDelete[i]);
		if(it!=playerV.end()) {
			playerV.erase(it);
		}
	}
}

/* INTENTA ASOCIAR UNA NUEVA DETECCIÓN A ALGÚN JUGADOR PERDIDO */
bool PlayerClassifier::recoverPlayer(Point pos, vector<Mat> hist) {
	float min_dist = 5+2.5*TIME_TO_DELETE;
	Player* found = NULL;
	for(std::list<Player>::iterator it = PlayerClassifier::playersToDelete.begin();
			it != PlayerClassifier::playersToDelete.end(); it++) {
		float dist;
		if(StatsAnalyzer::isInRecoverRange(pos,it->getLastPosition(),it->getDeletionCounter(),&dist) &&
				dist < min_dist &&
				PlayerClassifier::compareHistogram(hist,it->getHistogram()) <=
					BHATTACHARYYA_DISTANCE*N_CHANNELS) {
			found = &(*it);
			min_dist = dist;
		}
	}
	if(found!=NULL) {
		found->updateStats(pos);
		PlayerClassifier::playerV.push_back(*found);
		std::list<Player>::iterator it =
			std::find(PlayerClassifier::playersToDelete.begin(),PlayerClassifier::playersToDelete.end(),*found);
		if(it != PlayerClassifier::playersToDelete.end()) {
			PlayerClassifier::playersToDelete.erase(it);
		}
	}
	return found != NULL;
}
