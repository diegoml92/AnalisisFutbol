#include "config_file.h"

class PlayerClassifier {

private:

	static vector<vector<Rect>> clasif;
	static vector<vector<Rect>>::iterator it;

	static vector<vector<Mat>> clasifHists;
	static vector<vector<Mat>>::iterator itHist;

public:

	/*	LIMPIA LOS VECTORES */
	static void clearVectors();

	/*	
	*	ORDENA LOS VECTORES:
	*	Este orden se lleva a cabo en función del número de elementos contenidos,
	*	de esta manera, lo 2 primeros serán ambos equipos y el resto serán otros
	*	elementos (porteros, árbitro...)
	*/
	static void sortVectors();

	/*	
	*	CLASIFICA LOS ELEMENTOS DETECTADOS:
	*	Compara el histograma de los jugadores y los clasifica por equipos
	*/
	static void comparePlayer(Mat partido, Mat umbral, Rect rect);

	/*
	*	DIBUJA LOS JUGADORES EN EL VÍDEO
	*/
	static void drawTeams(Mat partido);

	/*
	*	ENCUENTRA EL EQUIPO EN EL QUE ESTÁ UN JUGADOR
	*/
	static void findAndDraw(Rect rect, Mat partido);

};

