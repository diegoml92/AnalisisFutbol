#include "config_file.h"

/* SE ENCARGA DE LAS OPERACIONES DE PROYECCION */
class From3DTo2D
{

private :
	static Mat projectionMatrix[N_VIDEOS];	// Matrices de proteccion

public :

	
	static Mat field2D;		// Representacion 2D del campo

	/* INICIAMOS LAS MATRICES DE PROYECCION */
	static void initProjectionMatrices();

	/* CONVERTIMOS COORDENADAS DEL MUNDO REAL A POSICIONES 2D */
	static vector<Point2f> get2DPosition(vector<Point2f> p, int nVideo);

	/* PINTAMOS LAS POSICIONES EN EL PLANO 2D */
	static void paint2DPositions(Rect player, int nVideo, Mat paint);

};

