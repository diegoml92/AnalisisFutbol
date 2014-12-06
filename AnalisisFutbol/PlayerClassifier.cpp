#include "PlayerClassifier.h"
#include "GUI.h"

vector<Rect> PlayerClassifier::players;

vector<vector<Rect>> PlayerClassifier::clasif;
vector<vector<Rect>>::iterator PlayerClassifier::it;

vector<vector<Mat>> PlayerClassifier::clasifHists;
vector<vector<Mat>>::iterator PlayerClassifier::itHist;

/* AÑADE UN JUGADOR */
void PlayerClassifier::addPlayer(Mat partido, Mat filtro, Rect player) {
	players.push_back(player);
	comparePlayer(partido,filtro,player);	// Hacemos la comparación
	findAndDraw(player, partido);			// Dibujamos al jugador
}

/*	
*	CLASIFICA LOS ELEMENTOS DETECTADOS:
*	Compara el histograma de los jugadores y los clasifica por equipos
*/
void PlayerClassifier::comparePlayer(Mat partido, Mat umbral, Rect rect) {

	int channels [] = {0,1,2};
	int nBins = 256;
	float range [] = {0,256};
	const float *ranges = {range};
	Mat hist_B, hist_G, hist_R;
	vector<Mat> planes, hist_v;
	split(partido(rect),planes);
	calcHist(&planes[0],1,channels,umbral(rect),hist_B,1,&nBins,&ranges);
	calcHist(&planes[1],1,channels,umbral(rect),hist_G,1,&nBins,&ranges);
	calcHist(&planes[2],1,channels,umbral(rect),hist_R,1,&nBins,&ranges);
	hist_v.push_back(hist_B);
	hist_v.push_back(hist_G);
	hist_v.push_back(hist_R);
	if(clasifHists.empty()) {
		clasifHists.push_back(hist_v);
		clasif.push_back(vector<Rect>());
		clasif.back().push_back(rect);
	} else {
		bool found = false;
		itHist = clasifHists.begin();
		int k = 0;
		vector<Mat> temp;
		while(itHist!=clasifHists.end() && !found) {
			temp = *itHist;
			found = compareHist(temp[0],hist_B,CV_COMP_BHATTACHARYYA) +
					compareHist(temp[1],hist_G,CV_COMP_BHATTACHARYYA) +
					compareHist(temp[2],hist_R,CV_COMP_BHATTACHARYYA) < BHATTACHARYYA_THRES*3;
			itHist++;
			k++;
		}
		if(found) {
			clasif[k-1].push_back(rect);
		} else {
			clasifHists.push_back(hist_v);
			clasif.push_back(vector<Rect>());
			clasif.back().push_back(rect);
		}
	}
}

/* ENCUENTRA EL EQUIPO EN EL QUE ESTÁ UN JUGADOR */
void PlayerClassifier::findAndDraw(Rect rect, Mat partido) {
	vector<Rect> s;
	it = clasif.begin();
	bool found = false;
	while(it!=clasif.end() && !found) {
		s = *it;
		found = std::find(s.begin(),s.end(),rect) != s.end();
		it++;
	}
	switch(GUI::playerBox()) {
		case 1 : {
			rectangle(partido,rect,mean(partido(*s.begin())),2,8);
			break;
		}
		case 2 : {
			rectangle(partido,rect,Scalar(COLOR_BLACK),2,8);
			break;
		}
	}
	
}

/*	LIMPIA LOS VECTORES */
void PlayerClassifier::clearVectors() {
	clasif.clear();
	clasifHists.clear();
}

/*	
*	ORDENA LOS VECTORES:
*	Este orden se lleva a cabo en función del número de elementos contenidos,
*	de esta manera, lo 2 primeros serán ambos equipos y el resto serán otros
*	elementos (porteros, árbitro...)
*/
void PlayerClassifier::sortVectors() {
	bool swapped = false;
	if(clasif.size()>1) {
		vector<Rect> aux;
		vector<Mat> auxHist;
		for(int i=0;i<clasif.size()-1;i++) {
			for(int j=0;j<clasif.size()-i-1;j++) {
				if(clasif[j].size() < clasif[j+1].size()) {
					aux = clasif[j];
					clasif[j] = clasif[j+1];
					clasif[j+1] = aux;

					auxHist = clasifHists[j];
					clasifHists[j] = clasifHists[j+1];
					clasifHists[j+1] = auxHist;
				}
			}
		}
	}
}

/* DETERMINA SI CUMPLE EL TAMAÑO PROPIO DE UN JUGADOR */
bool PlayerClassifier::isPlayerSize(Rect player) {
	return player.height>MIN_HEIGH && player.height<MAX_HEIGH
			&& player.width>MIN_WIDTH && player.width<MAX_WIDTH;
}

/* DIBUJA LOS JUGADORES EN EL VÍDEO */
void PlayerClassifier::drawTeams(Mat partido) {
	for(int i=0;i<clasif.size();i++) {
		for(int j=0;j<clasif[i].size();j++) {
			if(i==0) {
				putText(partido,"Equipo1",Point(clasif[i][j].x,clasif[i][j].y),1,2,Scalar(COLOR_WHITE));
			} else if(i==1) {
				putText(partido,"Equipo2",Point(clasif[i][j].x,clasif[i][j].y),1,2,Scalar(COLOR_WHITE));
			} else {
				putText(partido,"Otros",Point(clasif[i][j].x,clasif[i][j].y),1,2,Scalar(COLOR_WHITE));
			}
		}
	}
}