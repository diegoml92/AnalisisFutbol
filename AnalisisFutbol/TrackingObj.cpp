#include "GUI.h"
#include "TrackingObj.h"
#include "BallDetector.h"
#include "PlayerClassifier.h"
#include "From3DTo2D.h"
#include "GlobalStats.h"

/* LLEVA A CABO EL SEGUIMIENTO DE LOS JUGADORES  */
bool TrackingObj::tracking(Mat frame, Mat bg, Point* pos) {
	
	float rango[] = {0,255};
    const float* ranges [] = {rango};
    int channels [] = {0};
    int histSize [] = {128};

    Rect playerBox = Rect((*pos).x-PLAYER_WIDTH/2,(*pos).y-PLAYER_HEIGHT,PLAYER_WIDTH,PLAYER_HEIGHT);

	bool inRange = isInRange(&playerBox);

	if(inRange) {

		Mat hsv_roi, roi = frame(playerBox);
		cvtColor(roi,hsv_roi,COLOR_BGR2HSV);

		Mat thres, diff = abs(frame-bg);
		threshold(diff,thres,40,255,CV_THRESH_BINARY);
		vector<Mat> planes;
		split(thres,planes);
		Mat filter;
		filter = (planes[0] | planes[1] | planes[2]);

		Mat mask = filter(playerBox);
		Mat roi_hist;
		Mat images [] = {hsv_roi};

		calcHist(&hsv_roi,1,channels,mask,roi_hist,1,histSize,ranges);
		normalize(roi_hist,roi_hist,0,255,NORM_MINMAX);

		TermCriteria term_crit(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 5, 1);

		//int team_id = 0;
		//Player player(team_id);

		Mat hsv;
		cvtColor(frame, hsv, COLOR_BGR2HSV);

		Mat img2;
		frame.copyTo(img2);

		Mat dst;
		calcBackProject(&hsv,1,channels,roi_hist,dst,ranges,1);

		threshold(dst,dst,180,255,CV_THRESH_TOZERO);

		meanShift(dst,playerBox,term_crit);

		rectangle(img2,Rect(playerBox.tl(),playerBox.size()),Scalar(255,255,255),1);

		//if ((int)video.get(CV_CAP_PROP_POS_FRAMES)%5 == 0) {
		//Point bcenter = Point(playerBox.tl().x + playerBox.width/2, playerBox.br().y);
		//player.addPosition(bcenter);
		//From3DTo2D::paint2DPositions(playerBox,D_SQ_NUM,paint);
		//}

		*pos = Point(playerBox.tl().x + playerBox.width/2,playerBox.br().y);
		std::cout<<"CamPosReturn: "<<*pos<<std::endl;
	}
	return inRange;
}

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


/* TRACKING DE LOS JUGADORES */
void TrackingObj::trackPlayers(Mat frame[N_VIDEOS], Mat bg[N_VIDEOS], Point* detectedPlayer, int index) {
	bool detected = false;
	Point newPos;
	vector<Point> positions;
	for(int i=0; i<N_VIDEOS; i++) {
		Point* realPos = &From3DTo2D::getRealPosition(*detectedPlayer,i);
		if(isInFocus(*realPos)) {
			std::cout<<"CamPosBefore: "<<*realPos<<std::endl;
			detected = tracking(frame[i],bg[i],realPos);
			std::cout<<"CamPosAfter : "<<*realPos<<std::endl;
			positions.push_back(From3DTo2D::get2DPosition(*realPos,i));
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
	*detectedPlayer = newPos;
	std::cout<<"2dPosReturn : "<<*detectedPlayer<<std::endl;
}

/* REALIZA EL SEGUIMIENTO DE LOS ELEMENTOS DEL PARTIDO */
void TrackingObj::objectDetection(Mat filtro, Mat &partido, int nVideo, Mat paint) {
	Mat temp;		// Matriz auxiliar en la que buscaremos los contornos
	filtro.copyTo(temp);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE);
	if(hierarchy.size() > 0) {													// Si se encuentra algún contorno
		PlayerClassifier::clearVectors();										// Limpiamos los vectores de clasificación
		BallDetector::clear();													// Limpiamos el vector de balones

		for( int i = 0; i < contours.size(); i++ ) {							// Recorremos los contornos
			Rect elem = boundingRect(Mat(contours[i]));							// Creamos el boundingBox
			if(PlayerClassifier::isPlayerSize(elem)) {							// Si cumple las restricciones de tamaño...
				PlayerClassifier::addPlayer(partido, filtro, elem);				// ...añade el jugador y lo clasifica
				Point playerPos = Point(elem.br().x-elem.width/2, elem.br().y); // Calculamos el punto en el que está el jugador
				From3DTo2D::get2DPosition(playerPos, nVideo);					// Transformamos a posición 2D
				GlobalStats::locations[nVideo].push_back(playerPos);			// Añadimos al vector de elementos encontrados
				From3DTo2D::paint2DPositions(elem, nVideo, paint);				// Representamos las posiciones en el plano 2D
			} else if(BallDetector::isBallSize(elem)) {							// Si tiene tamaño y forma de balón...
				BallDetector::addBall(elem);									// ...se añade
			}
		}

		PlayerClassifier::sortVectors();										// Ordenamos los vectores de clasificación
		PlayerClassifier::drawTeams(partido);									// Dibujamos la etiqueta de cada elemento
		BallDetector::selectBall(partido, filtro);								// Elige el balón entre los candidatos
		BallDetector::drawBall(partido);										// Dibujamos el balón
	}
}
