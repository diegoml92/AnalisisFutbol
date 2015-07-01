#include "config_file.h"

/* SE ENCARGA DE LAS OPERACIONES DE PROYECCION */
class From3DTo2D
{

private :
	static Mat projectionMatrix[N_VIDEOS];	// Matrices de proyección

	static Mat inverseProjectionMatrix[N_VIDEOS]; // Matrices de proyección inversa

public :

	/* DETERMINA SI EL PUNTO PERTENECE A UNA POSICIÓN VÁLIDA */
	static bool isInRange(Point p);

	/* INICIAMOS LAS MATRICES DE PROYECCION */
	static void initProjectionMatrices();

	/* CONVERTIMOS COORDENADAS DE LA CÁMARA A POSICIONES 2D */
	static Point2f get2DPosition(Point2f p, int nVideo);

	/* CONVERTIMOS COORDENADAS DE LA CÁMARA A POSICIONES 2D */
	static vector<Point2f> get2DPositionVector(vector<Point2f> p, int nVideo);

	/* CONVERTIMOS COORDENADAS 2D A COORDENADAS DE LA CÁMARA */
	static Point getCameraPosition(Point modelPos, int nVideo);

};

