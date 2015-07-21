#include "config_file.h"
#include "Player.h"

/* CLASE ENCARGADA DE LA GESTI�N DE LOS JUGADORES */
class PlayerClassifier {
private:

	/* CALCULA EL HISTOGRAMA DEL JUGADOR */
	static void calculateHistogram(Mat frame, Mat filter, vector<Rect> rects, vector<Mat>* hist_v);

	/* CALCULA EL HISTOGRAMA DEL JUGADOR */
	static vector<Mat> calculateHistogram(Point pos, int nCam);

	/* DETERMINA SI EL PUNTO EST� CERCA DEL BORDE DEL CAMPO */
	static bool isFieldEdge(Point p);

	/* ELIMINA LOS ELEMENTOS DUPLICADOS */
	static void PlayerClassifier::deleteDuplicatedPlayer
		(string key, Point position, Player p1, Player p2, vector<Player>* toDelete);

	// Jugadores duplicados
	static std::unordered_map<string,int> PlayerClassifier::duplicatedPlayers;

public:

	// Variables globales
	static vector<Player> playerV;
	static std::list<Player> playersToDelete;

	/* A�ADE UN JUGADOR */
	static void addPlayer(Mat partido, Mat filtro, Point player);

	/* DETERMINA SI CUMPLE EL TAMA�O PROPIO DE UN JUGADOR */
	static bool isPlayerSize(Rect player);

	/* DETERMINA CU�L DE LOS ELEMENTOS ASIGNADOS A UN JUGADOR ES EL CORRECTO */
	static Point findBestMatch(Player* player);

	/* COMPARA LOS HISTOGRAMAS DE LOS JUGADORES */
	static double compareHistogram(vector<Mat> playerHist, vector<Mat> newHist);

	/* DESCARTA FALSOS POSITIVOS (L�NEAS, ETC) EN FUNCI�N DE EL N�MERO DE P�XELES BLANCOS */
	static bool canBePlayer(Mat roi, float val = 0.35);

	/* DETECCI�N DE JUGADORES SOBRE EL CAMPO */
	static void objectDetection();

	/* ELIMINA LAS DUPLICIDADES EN LAS DETECCIONES */
	static void removeDuplications();

	/* DETERMINA SI UN ELEMENTO YA HA SIDO DETECTADO */
	static bool alreadyDetected(Point p);

	/*
	* A�ADE UN JUGADOR A LA LISTA DE BORRADO 
	* Y LO BORRA DE LA LISTA DE JUGADORES ACTUALES
	*/
	static void addPlayerToDelete(vector<Player>::iterator* itP);

	/* LOCALIZA LOS JUGADORES DUPLICADOS */
	static void checkDuplicatedPlayers();

	/* COMPRUEBA LOS JUGADORES A BORRAR */
	static void checkPlayersToDelete();

	/* INTENTA ASOCIAR UNA NUEVA DETECCI�N A ALG�N JUGADOR PERDIDO */
	static bool recoverPlayer(Point pos, vector<Mat> hist);

};

