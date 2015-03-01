#include "config_file.h"

/* CLASE ENCARGADA DE LA CLASIFICACIÓN DE LOS JUGADORES EN DIFERENTES EQUIPOS */
class PlayerClassifier {

private:

	static vector<vector<Rect>> clasif;				// Almacena los elementos en sus respectivos grupos
	static vector<vector<Rect>>::iterator it;		// Iterator para recorrer el vector clasif

	static vector<vector<Mat>> clasifHists;			// Alamacena el histograma que representa a cada equipo
	static vector<vector<Mat>>::iterator itHist;	// Iterator para recorrer el vector clasifHist

	/*	
	*	CLASIFICA LOS ELEMENTOS DETECTADOS:
	*	Compara el histograma de los jugadores y los clasifica por equipos
	*/
	static void comparePlayer(Mat partido, Mat umbral, Rect rect);

	/* ENCUENTRA EL EQUIPO EN EL QUE ESTÁ UN JUGADOR */
	static void findAndDraw(Rect rect, Mat partido);

public:

	/* AÑADE UN JUGADOR */
	static void addPlayer(Mat partido, Mat filtro, Rect player);

	/*	LIMPIA LOS VECTORES */
	static void clearVectors();

	/*	
	*	ORDENA LOS VECTORES:
	*	Este orden se lleva a cabo en función del número de elementos contenidos,
	*	de esta manera, lo 2 primeros serán ambos equipos y el resto serán otros
	*	elementos (porteros, árbitro...)
	*/
	static void sortVectors();

	/* DETERMINA SI CUMPLE EL TAMAÑO PROPIO DE UN JUGADOR */
	static bool isPlayerSize(Rect player);

		/* DIBUJA LOS JUGADORES EN EL VÍDEO */
	static void drawTeams(Mat partido);

};

