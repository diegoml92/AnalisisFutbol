#include "config_file.h"
#include "Player.h"

/* CLASE ENCARGADA DEL SEGUIMIENTO DE LOS ELEMENTOS DEL CAMPO */
class TrackingObj {
private:
	/* LLEVA A CABO EL SEGUIMIENTO DE LOS JUGADORES  */
	static bool tracking(Mat frame, Mat filter, Mat* paint, Point* pos);

	/* VENTANA DE BÚSQUEDA DEL JUGADOR */
	static void searchWindow(Rect playerBox, Rect* searchWindow, Rect* relative);

public:

	/* COMPRUEBA SI EL PUNTO ESTÁ EN EL FOCO DE LA CÁMARA */
	static bool isInFocus(Point p);

	/* COMPRUEBA SI EL JUGADOR ESTÁN EN EL RANGO DENTRO DE LA IMAGEN */
	static bool isInRange(Rect* r);

	/* TRACKING DE LOS JUGADORES */
	static void trackPlayers(Mat frame[N_VIDEOS], Mat filter[N_VIDEOS], Player* player, int index);

	/* DETECCIÓN DE JUGADORES SOBRE EL CAMPO */
	static void objectDetection(Mat filtro, Mat &partido, int nVideo, Mat paint);

};
