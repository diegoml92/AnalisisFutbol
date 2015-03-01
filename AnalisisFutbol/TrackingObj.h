#include "config_file.h"

/* CLASE ENCARGADA DEL SEGUIMIENTO DE LOS ELEMENTOS DEL CAMPO */
class TrackingObj {
private:
	/* LLEVA A CABO EL SEGUIMIENTO DE LOS JUGADORES  */
	static bool tracking(Mat frame, Mat filter, Mat* paint, Point* pos);

	/* COMPRUEBA SI EL PUNTO EST� EN EL FOCO DE LA C�MARA */
	static bool isInFocus(Point p);

	/* COMPRUEBA SI EL JUGADOR EST�N EN EL RANGO DENTRO DE LA IMAGEN */
	static bool isInRange(Rect* r);

public:

	/* TRACKING DE LOS JUGADORES */
	static void trackPlayers(Mat frame[N_VIDEOS], Mat filter[N_VIDEOS], Point* detectedPlayers, int index);

	/* DETECCI�N DE JUGADORES SOBRE EL CAMPO */
	static void objectDetection(Mat filtro, Mat &partido, int nVideo, Mat paint);

};
