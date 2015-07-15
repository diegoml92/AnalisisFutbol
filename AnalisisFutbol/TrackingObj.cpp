#include "TrackingObj.h"
#include "PlayerClassifier.h"
#include "From3DTo2D.h"
#include "GlobalStats.h"

/* COMPRUEBA SI EL JUGADOR ESTÁN EN EL RANGO DENTRO DE LA IMAGEN */
bool TrackingObj::isInRange(Rect* r) {
	Rect rect = *r;
	bool inRange = rect.tl().x >= 0 && rect.tl().y >= 0 &&
		rect.br().x < VIDEO_WIDTH && rect.br().y < VIDEO_HEIGHT;
	// Si no está en rango intentamos reajustarlo
	if(!inRange) {
		int tl_x = rect.tl().x;
		int tl_y = rect.tl().y;
		int br_x = rect.br().x;
		int br_y = rect.br().y;

		if(tl_x < 0) {
			tl_x = 0;
		}
		if(tl_y < 0) {
			tl_y = 0;
		}
		if(br_x >= VIDEO_WIDTH) {
			br_x = VIDEO_WIDTH-1;
		}
		if(br_y >= VIDEO_HEIGHT) {
			br_y = VIDEO_HEIGHT-1;
		}

		Rect tmp_rect = Rect(Point(tl_x,tl_y),Point(br_x,br_y));
		if(tmp_rect.width >= MIN_WIDTH && tmp_rect.height >= MIN_HEIGH) {
			inRange = true;
			*r = tmp_rect;
		}
	}
	return inRange;
}

/* COMPRUEBA SI EL PUNTO ESTÁ EN EL FOCO DE LA CÁMARA */
bool TrackingObj::isInFocus(Point p) {
	return p.x >= 0 && p.y >= 0 && p.x < VIDEO_WIDTH && p.y < VIDEO_HEIGHT;
}

/* VENTANA DE BÚSQUEDA DEL JUGADOR */
void TrackingObj::searchWindow(Rect playerBox, Rect* searchWindow, Rect* relative) {
	Point windowP = Point(SEARCH_WINDOW);
	*searchWindow =  Rect(playerBox.tl()-windowP,playerBox.br()+windowP);
	*relative = Rect(windowP, playerBox.size());
	bool inRange = searchWindow->tl().x >= 0 && searchWindow->tl().y >= 0 &&
		searchWindow->br().x < VIDEO_WIDTH && searchWindow->br().y < VIDEO_HEIGHT;
	// Si la ventana no está en rango la reajustamos
	if(!inRange) {
		int tl_x = searchWindow->tl().x;
		int tl_y = searchWindow->tl().y;
		int br_x = searchWindow->br().x;
		int br_y = searchWindow->br().y;

		bool relativeChanged = false;
		Point relP = windowP;

		if(tl_x < 0) {
			relativeChanged = true;
			relP.x = playerBox.x;
			tl_x = 0;
		}
		if(tl_y < 0) {
			relativeChanged = true;
			relP.y = playerBox.y;
			tl_y = 0;
		}
		if(br_x >= VIDEO_WIDTH) {
			br_x = VIDEO_WIDTH-1;
		}
		if(br_y >= VIDEO_HEIGHT) {
			br_y = VIDEO_HEIGHT-1;
		}

		if(relativeChanged) {
			*relative = Rect(Point(relP), playerBox.size());
		}
		*searchWindow = Rect(Point(tl_x,tl_y),Point(br_x,br_y));
	}
}

/* LLEVA A CABO EL SEGUIMIENTO DE LOS JUGADORES  */
bool TrackingObj::tracking(Point* pos, Player player, int nCam) {
	
	int channelB [] = {0};
	int channelG [] = {1};
	int channelR [] = {2};
	float range [] = {0,RGB};
	const float* ranges [] = {range};

	Rect playerBox = GlobalStats::getPlayerRect(*pos);
	bool inRange = isInRange(&playerBox);

	if(inRange) {
		// Obtenemos la ventana de búsqueda
		Rect searchWindowRect, relativePlayerBox;
		TrackingObj::searchWindow(playerBox, &searchWindowRect, &relativePlayerBox);
		Mat window = GlobalStats::frame[nCam](searchWindowRect);
		Mat windowMask = GlobalStats::filter[nCam](searchWindowRect);
		// Calculamos las matrices de back projection
		Mat dst[N_CHANNELS];
		vector<Mat> hist = player.getHistogram();
		calcBackProject(&window,1,channelB,hist[0],dst[0],ranges);
		calcBackProject(&window,1,channelG,hist[1],dst[1],ranges);
		calcBackProject(&window,1,channelR,hist[2],dst[2],ranges);
		// Aplicamos la máscara a lo obtenido para filtrar
		dst[0]&=windowMask;
		dst[1]&=windowMask;
		dst[2]&=windowMask;

		Mat backProj = (dst[0]+dst[1]+dst[2])/N_CHANNELS;

		threshold(backProj,backProj,20,RGB,THRESH_TOZERO);

		TermCriteria term_crit(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 10, 1);

		Rect tmp = relativePlayerBox;
		Rect aux = relativePlayerBox + (Point)player.getShift(nCam);
		Point desp;
		// Aplicamos meanshift para obtener la nueva localización
		if(isInRange(&aux)) {
			meanShift(backProj,aux,term_crit);
			desp = aux.tl() - tmp.tl();
		} else {
			meanShift(backProj,relativePlayerBox,term_crit);
			desp = relativePlayerBox.tl() - tmp.tl();
		}
		*pos = Point(playerBox.tl().x + desp.x + playerBox.width/2,playerBox.br().y + desp.y);
		// Comprobamos si la nueva posición cumple las restricciones
		playerBox = GlobalStats::getPlayerRect(*pos);
		inRange = isInRange(&playerBox);
	}

	return inRange && PlayerClassifier::canBePlayer(GlobalStats::filter[nCam](playerBox), 0.2);
}

/* CALCULA LA DISTANCIA ENTRE DOS PUNTOS */
float TrackingObj::distance(Point p1, Point p2) {
	float dist = 0;
	if(p2.x >= 0) {
		dist = norm(p1-p2)/10.0;
	}
	return dist;
}

/*
* DETERMINA SI LOS ELEMENTOS ASOCIADOS POR CADA SECUENCIA
* EN LA QUE HAYA SIDO DETECTADO UN JUGADOR NO SE CORRESPONDEN
*/
bool TrackingObj::isDifferentPosition(vector<Point> positions) {
	bool different = false;
	for(int i=0; i<positions.size() && !different; i++) {
		for(int j=i+1; j<positions.size() && !different; j++) {
			different = TrackingObj::distance(positions[i],positions[j]) > DIFF_POINTS_DIST;
		}
	}
	return different;
}

/* VERIFICA QUE LA NUEVA POSICIÓN ESTE DENTRO DEL RANGO DE DISTANCIA */
void TrackingObj::validatePosition(Point lastPoint, Point* actualPoint, int nCam) {
	Point ori = From3DTo2D::get2DPosition(lastPoint, nCam);
	Point des = From3DTo2D::get2DPosition(*actualPoint, nCam);
	float dist = TrackingObj::distance(ori,des);
	if (dist > MAX_DIST) {
		float rel = MAX_DIST / dist;
		Vec2i v = *actualPoint - lastPoint;
		v *= rel;
		actualPoint->x = lastPoint.x + v[0];
		actualPoint->y = lastPoint.y + v[1];
	}
}

/* TRACKING DE LOS JUGADORES */
void TrackingObj::trackPlayers() {
	vector<Player>::iterator itP = PlayerClassifier::playerV.begin();
	while(itP != PlayerClassifier::playerV.end()) {
		Player* player = &(*itP);
		bool detected = false;
		vector<Point> positions;
		for(int i=0; i<N_VIDEOS; i++) {
			Point* realPos;
			if (player->getBPos(i)) {
				// Ya estaba detectado en la secuencia i
				realPos = &(player->getCamPos(i));
			} else {
				realPos = &From3DTo2D::getCameraPosition(player->getPosition(),i);
			}
			if(player->getBPos(i) || isInFocus(*realPos)) {
				// Se aplica el tracking
				if(tracking(realPos,*player,i)) {
					if(player->getBPos(i)) {
						Point camPos = player->getCamPos(i);
						TrackingObj::validatePosition(camPos,realPos,i);
						Point shift = *(realPos) - camPos;
						player->setShift(shift,i);
					}
					player->setCamPos(i,*realPos);
					positions.push_back(From3DTo2D::get2DPosition(*realPos,i));
					detected = true;
				} else {
					player->unSetCamPos(i);
				}
			}
		}
		if(detected) {
			Point newPos;
			if(!TrackingObj::isDifferentPosition(positions)) {
				// Jugador trackeado correctamente
				for(int i=0; i<positions.size(); i++) {
					newPos+=positions.at(i);
				}
				newPos = Point(newPos.x/positions.size(), newPos.y/positions.size());
				player->addPosition(newPos);
			} else {
				// El jugador ha sido detectado en posiciones muy distantes
				newPos = PlayerClassifier::findBestMatch(player);
				player->addPosition(newPos);
			}
			// Incrementamos el iterador del bucle de tracking
			itP++;
		} else {
			// Añadimos el jugador a la lista de borrado
			PlayerClassifier::addPlayerToDelete(&itP);
		}
	}
}
