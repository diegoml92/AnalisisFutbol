#include "GUI.h"
#include "GlobalStats.h"

int GUI::nPlayer = 0;

/* MUESTRA LAS ESTADÍSTICAS SELECCIONADAS */
void GUI::showStatsWindow() {
	Mat stats;
	stats = Mat(Size(STATS_W_WIDTH,STATS_W_HEIGHT), CV_8UC3);
	stats.setTo(Scalar(GUI_COLOR));
	line(stats,Point(0,25),Point(STATS_W_WIDTH,25),Scalar(GUI_AUX_COLOR),2);

	if(GlobalStats::playerV.size()>GUI::nPlayer) {

		int player = GUI::nPlayer;

		std::stringstream ssplayer;
		ssplayer << "Jugador " << player;

		putText(stats,"Nombre:",Point(10,20),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);
		putText(stats,ssplayer.str(),Point(100,20),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);

		std::stringstream ssdistance;
		ssdistance << GlobalStats::playerV[player].getDistance() << " m";

		std::stringstream ssavgspeed;
		ssavgspeed << GlobalStats::playerV[player].getAvgSpeed() << " km/h";

		std::stringstream ssmaxspeed;
		ssmaxspeed << GlobalStats::playerV[player].getMaxSpeed() << " km/h";

		putText(stats,"Distancia :",Point(10,75),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);
		putText(stats,ssdistance.str(),Point(100,75),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);

		putText(stats,"Vel. media:",Point(10,100),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);
		putText(stats,ssavgspeed.str(),Point(100,100),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);

		putText(stats,"Vel. max. :",Point(10,125),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);
		putText(stats,ssmaxspeed.str(),Point(100,125),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);

		Mat aux = stats(Rect(5,160,
			SOCCER_FIELD_WIDTH/ANALYZER_VIDEO_SIZE_RELATION,SOCCER_FIELD_HEIGHT/ANALYZER_VIDEO_SIZE_RELATION));
		GlobalStats::playerV[player].getAreaStats().copyTo(aux);
	} else {
		putText(stats,"NO HAY NINGÚN JUGADOR",Point(10+STATS_W_WIDTH,100),CV_FONT_HERSHEY_PLAIN,1,Scalar(COLOR_WHITE),1);
	}
	imshow(STATS_W,stats);
}
