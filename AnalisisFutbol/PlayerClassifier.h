#include "config_file.h"
/* CLASE ENCARGADA DE LA CLASIFICACIÓN DE LOS JUGADORES EN DIFERENTES EQUIPOS */
class PlayerClassifier {

private:

	// Almacena los elementos en sus respectivos grupos
	static vector<vector<Rect>> clasif;
	// Iterator para recorrer el vector clasif
	static vector<vector<Rect>>::iterator it;

	// Alamacena el histograma que representa a cada equipo
	static vector<vector<Mat>> clasifHists;
	// Iterator para recorrer el vector clasifHist
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

