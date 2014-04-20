#include "config_file.h"

class BallDetector
{
private:

	static vector<Rect> balls;		// Elementos que pueden ser el bal�n
	static Rect foundBall;			// Bal�n encontrado

	/* DETERMINA SI ES SIMILAR A UN CUADRADO */
	static bool isSquare(Rect ball);

public:

	/* A�ADE UNA POSIBILIDAD A LA LISTA DE BALONES */
	static void addBall(Rect ball);

	/* DETERMINA SI CUMPLE EL TAMA�O DEL BAL�N */
	static bool isBallSize(Rect ball);

	/* DETERMINA CU�L ES EL BAL�N */
	static void selectBall();

	/* DIBUJA EL BAL�N */
	static void drawBall(Mat partido);
};

