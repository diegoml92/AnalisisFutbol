#include "TrackingObj.h"
#include "PlayerClassifier.h"
#include "From3DTo2D.h"
#include "GlobalStats.h"

/* COMPRUEBA SI EL JUGADOR ESTÁN EN EL RANGO DENTRO DE LA IMAGEN */
bool TrackingObj::isInRange(Rect* r) {
	Rect rect = *r;
	bool inRange = false;
	inRange = rect.tl().x >= 0 && rect.tl().y >= 0 && rect.br().x < VIDEO_WIDTH && rect.br().y < VIDEO_HEIGHT;
	if(!inRange) {
		int tl_x = rect.tl().x,tl_y = rect.tl().y,br_x = rect.br().x,br_y = rect.br().y;
		if(rect.tl().x < 0) {
			tl_x = 0;
		}
		if(rect.tl().y < 0) {
			tl_y = 0;
		}
		if(rect.br().x >= VIDEO_WIDTH) {
			br_x = VIDEO_WIDTH-1;
		}
		if(rect.br().y >= VIDEO_HEIGHT) {
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
	return p.x >= 0 && p.y >= 0 && p.x <= VIDEO_WIDTH && p.y <= VIDEO_HEIGHT;
}

/* VENTANA DE BÚSQUEDA DEL JUGADOR */
void TrackingObj::searchWindow(Rect playerBox, Rect* searchWindow, Rect* relative) {
	Point windowP = Point(SEARCH_WINDOW);
	*searchWindow =  Rect(playerBox.tl()-windowP,playerBox.br()+windowP);
	*relative = Rect(windowP, playerBox.size());
	bool inRange = searchWindow->tl().x >= 0 && searchWindow->tl().y >= 0 &&
		searchWindow->br().x < VIDEO_WIDTH && searchWindow->br().y < VIDEO_HEIGHT;
	if(!inRange) {
		int tl_x = searchWindow->tl().x,tl_y = searchWindow->tl().y;
		int br_x = searchWindow->br().x,br_y = searchWindow->br().y;
		bool relativeChanged = false;
		Point relP = windowP;
		if(searchWindow->tl().x < 0) {
			relativeChanged = true;
			relP.x = playerBox.x;
			tl_x = 0;
		}
		if(searchWindow->tl().y < 0) {
			relativeChanged = true;
			relP.y = playerBox.y;
			tl_y = 0;
		}
		if(searchWindow->br().x >= VIDEO_WIDTH) {
			br_x = VIDEO_WIDTH-1;
		}
		if(searchWindow->br().y >= VIDEO_HEIGHT) {
			br_y = VIDEO_HEIGHT-1;
		}

		if(relativeChanged) {
			*relative = Rect(Point(relP), playerBox.size());
		}
		*searchWindow = Rect(Point(tl_x,tl_y),Point(br_x,br_y));
	}
}

/* LLEVA A CABO EL SEGUIMIENTO DE LOS JUGADORES  */
bool TrackingObj::tracking(Mat frame, Mat filter, Point* pos, Player player) {
	
	float range[] = {0,256};
	const float* ranges [] = {range};
    int channel_B [] = {0};
	int channel_G [] = {1};
	int channel_R [] = {2};

    Rect playerBox = GlobalStats::getPlayerRect(*pos);

	bool inRange = isInRange(&playerBox);

	if(inRange) {

		Mat roi = frame(playerBox);

		vector<Mat> hist = player.getHistogram();

		Rect searchWindowRect, relativePlayerBox;
		searchWindow(playerBox, &searchWindowRect, &relativePlayerBox);
		Mat window = frame(searchWindowRect);
		Mat windowMask = filter(searchWindowRect);

		Mat dst[3];
		calcBackProject(&window,1,channel_B,hist[0],dst[0],ranges);
		calcBackProject(&window,1,channel_B,hist[1],dst[1],ranges);
		calcBackProject(&window,1,channel_B,hist[2],dst[2],ranges);

		dst[0]&=windowMask;
		dst[1]&=windowMask;
		dst[2]&=windowMask;

		Mat backProj = dst[0]|dst[1]|dst[2];
		Rect tmp = relativePlayerBox;

		TermCriteria term_crit(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 3, 1);
		meanShift(backProj,relativePlayerBox,term_crit);

		Point desp = relativePlayerBox.tl() - tmp.tl();
		*pos = Point(playerBox.tl().x + desp.x + playerBox.width/2,playerBox.br().y + desp.y);
	}
	playerBox = GlobalStats::getPlayerRect(*pos);
	return inRange && isInRange(&playerBox) && PlayerClassifier::canBePlayer2(filter(playerBox));
}

/* TRACKING DE LOS JUGADORES */
void TrackingObj::trackPlayers(Mat frame[N_VIDEOS], Mat filter[N_VIDEOS], Player* player) {
	bool detected = false;
	vector<Point> positions;
	for(int i=0; i<N_VIDEOS; i++) {
		Point* realPos;
		if (player->getBPos(i)) {
			realPos = &player->getCamPos(i);
		} else {
			realPos = &From3DTo2D::getRealPosition(player->getPosition(),i);
		}
		if(player->getBPos(i) || isInFocus(*realPos)) {
			if(tracking(frame[i],filter[i],realPos,*player)) {
				player->setCamPos(i,*realPos);
				positions.push_back(From3DTo2D::get2DPosition(*realPos,i));
				detected = true;
				filter[i](Rect()).setTo(0);
			} else {
				player->unSetCamPos(i);
			}
		}
	}
	if(detected) {
		Point newPos;
		for(unsigned i=0; i<positions.size(); i++) {
			newPos+=positions.at(i);
		}
		newPos = Point(newPos.x/positions.size(), newPos.y/positions.size());
		player->addPosition(newPos);
	} else {
		GlobalStats::playersToDelete.push_back(player);
	}
}

/* REALIZA EL SEGUIMIENTO DE LOS ELEMENTOS DEL PARTIDO */
void TrackingObj::objectDetection(Mat filtro, Mat &partido, int nVideo, Mat paint) {
	Mat temp;		// Matriz auxiliar en la que buscaremos los contornos
	filtro.copyTo(temp);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE);
	if(hierarchy.size() > 0) {													// Si se encuentra algún contorno
		for(unsigned i = 0; i < contours.size(); i++ ) {							// Recorremos los contornos
			Rect elem = boundingRect(Mat(contours[i]));							// Creamos el boundingBox
			if(PlayerClassifier::isPlayerSize(elem) && PlayerClassifier::canBePlayer(filtro(elem))) {
				GlobalStats::locations[nVideo].push_back(elem);			// Añadimos al vector de elementos encontrados
			}
		}
	}
}
