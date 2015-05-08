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
bool TrackingObj::tracking(Mat hsv, Mat filter, Mat* paint, Point* pos) {
	
	float range[] = {0,256};
	const float* ranges [] = {range};
    int channel_H [] = {0};
	int channel_S [] = {1};
	int channel_V [] = {2};
    int histSize [] = {64};

    Rect playerBox = Rect((*pos).x-PLAYER_WIDTH/2,(*pos).y-PLAYER_HEIGHT,PLAYER_WIDTH,PLAYER_HEIGHT);

	bool inRange = isInRange(&playerBox);

	if(inRange) {

		Mat hsv_roi = hsv(playerBox);

		Mat mask = filter(playerBox);
		Mat hist[3];
		Mat images [] = {hsv_roi};

		calcHist(&hsv_roi,1,channel_H,mask,hist[0],1,histSize,ranges);
		calcHist(&hsv_roi,1,channel_S,mask,hist[1],1,histSize,ranges);
		calcHist(&hsv_roi,1,channel_V,mask,hist[2],1,histSize,ranges);
		//normalize(hist[i],hist[i],0,1,NORM_MINMAX);

		TermCriteria term_crit(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 5, 1);

		Mat dst[3];
		Rect searchWindowRect, relativePlayerBox;
		searchWindow(playerBox, &searchWindowRect, &relativePlayerBox);
		Mat window = hsv(searchWindowRect);
		Mat windowMask = filter(searchWindowRect);
		calcBackProject(&window,1,channel_H,hist[0],dst[0],ranges,1);
		calcBackProject(&window,1,channel_H,hist[1],dst[1],ranges,1);
		calcBackProject(&window,1,channel_H,hist[2],dst[2],ranges,1);
		dst[0]&=windowMask;
		dst[1]&=windowMask;
		dst[2]&=windowMask;
		Mat backproj = dst[0]|dst[1]|dst[2];
		Rect tmp = relativePlayerBox;
		meanShift(backproj,relativePlayerBox,term_crit);

		Point desp = relativePlayerBox.tl() - tmp.tl();

		*pos = Point(playerBox.tl().x + desp.x + playerBox.width/2,playerBox.br().y + desp.y);
	}
	return inRange;
}

/* TRACKING DE LOS JUGADORES */
void TrackingObj::trackPlayers(Mat frame[N_VIDEOS], Mat filter[N_VIDEOS], Player* player, int index) {
	bool detected = false;
	Point newPos;
	vector<Point> positions;
	Mat paint[N_VIDEOS];
	for(int i=0; i<N_VIDEOS; i++) {
		Point* realPos = &From3DTo2D::getRealPosition(player->getPosition(),i);
		if(isInFocus(*realPos)) {
			if(tracking(frame[i],filter[i],&paint[i],realPos)) {
				positions.push_back(From3DTo2D::get2DPosition(*realPos,i));
				detected = true;
			}
		}
	}
	if(detected) {
		for(int i=0; i<positions.size(); i++) {
			newPos+=positions.at(i);
		}
		newPos = Point(newPos.x/positions.size(), newPos.y/positions.size());
	} else {
		GlobalStats::playersToDelete.push_back(index);
	}
	player->addPosition(newPos);
}

/* REALIZA EL SEGUIMIENTO DE LOS ELEMENTOS DEL PARTIDO */
void TrackingObj::objectDetection(Mat filtro, Mat &partido, int nVideo, Mat paint) {
	Mat temp;		// Matriz auxiliar en la que buscaremos los contornos
	filtro.copyTo(temp);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE);
	if(hierarchy.size() > 0) {													// Si se encuentra algún contorno
		for( int i = 0; i < contours.size(); i++ ) {							// Recorremos los contornos
			Rect elem = boundingRect(Mat(contours[i]));							// Creamos el boundingBox
			if(PlayerClassifier::isPlayerSize(elem) && PlayerClassifier::canBePlayer(filtro(elem))) {
				GlobalStats::locations[nVideo].push_back(elem);			// Añadimos al vector de elementos encontrados
			}
		}
	}
}
