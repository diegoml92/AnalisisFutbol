#include "config_file.h"
#include "Player.h"

/* CLASE ENCARGADA DEL SEGUIMIENTO DE LOS ELEMENTOS DEL CAMPO */
class TrackingObj {
private:
	/* LLEVA A CABO EL SEGUIMIENTO DE LOS JUGADORES  */
	static bool tracking(Point* pos, Player player, int nCam);

	/* VENTANA DE BÚSQUEDA DEL JUGADOR */
	static void searchWindow(Rect playerBox, Rect* searchWindow, Rect* relative);

	/* CALCULA LA DISTANCIA ENTRE DOS PUNTOS */
	static float distance(Point actualPoint, Point lastPoint);

	/*
	* DETERMINA SI LOS ELEMENTOS ASOCIADOS POR CADA SECUENCIA
	* EN LA QUE HAYA SIDO DETECTADO UN JUGADOR NO SE CORRESPONDEN
	*/
	static bool isDifferentPosition(vector<Point> positions);


public:

	/* COMPRUEBA SI EL PUNTO ESTÁ EN EL FOCO DE LA CÁMARA */
	static bool isInFocus(Point p);

	/* COMPRUEBA SI EL JUGADOR ESTÁN EN EL RANGO DENTRO DE LA IMAGEN */
	static bool isInRange(Rect* r);

	/* TRACKING DE LOS JUGADORES */
	static void trackPlayers(vector<Player>::iterator* player);

};
