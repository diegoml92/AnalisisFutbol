#include "TrackingObj.h"

// Valores que marca en rango de filtro del césped
extern int MIN_B;
extern int MAX_B;
extern int MIN_G;
extern int MAX_G;
extern int MIN_R;
extern int MAX_R;

// Valores para el rango de tamaño de los jugadores
extern int MIN_WIDTH, MIN_HEIGH;
extern int MAX_WIDTH, MAX_HEIGH;

void TrackingObj::trackObject(Mat filtro, Mat &partido) {
	Mat temp;
	filtro.copyTo(temp);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE);
	double refArea=0;
	if(hierarchy.size() > 0) {
		int numObjects = hierarchy.size();

		PlayerClassifier::clearVectors();

		rectangle(filtro, Point(0,0), Point(MAX_BALL_SIZE,MAX_BALL_SIZE), Scalar(0), 0);
		rectangle(filtro, Point(0,0), Point(7,7), Scalar(0), 0);

		for(int index = 0; index >= 0; index = hierarchy[index][0]) {

			vector<Rect> minRect( contours.size() );
			for( int i = 0; i < contours.size(); i++ ) {
				minRect[i] = boundingRect( Mat(contours[i]) );
			}
			if( (minRect[index].width>MIN_WIDTH && minRect[index].width<MAX_WIDTH) &&
				(minRect[index].height>MIN_HEIGH && minRect[index].height<MAX_HEIGH) ) {

				PlayerClassifier::comparePlayer(partido,filtro,minRect[index]);

				PlayerClassifier::findAndDraw(minRect[index], partido);

			} else if(	minRect[index].width < MAX_BALL_SIZE &&
						minRect[index].height < MAX_BALL_SIZE) {
				rectangle(partido,minRect[index],Scalar(0,0,255),-1);
			}
		}

		PlayerClassifier::sortVectors();

		PlayerClassifier::drawTeams(partido);
	}
}
