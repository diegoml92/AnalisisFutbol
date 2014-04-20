#include "config_file.h"

class BallDetector
{
private:

	static vector<Rect> balls;		// Elementos que pueden ser el balón
	static Rect foundBall;			// Balón encontrado

	/* DETERMINA SI ES SIMILAR A UN CUADRADO */
	static bool isSquare(Rect ball);

public:

	/* AÑADE UNA POSIBILIDAD A LA LISTA DE BALONES */
	static void addBall(Rect ball);

	/* DETERMINA SI CUMPLE EL TAMAÑO DEL BALÓN */
	static bool isBallSize(Rect ball);

	/* DETERMINA CUÁL ES EL BALÓN */
	static void selectBall();

	/* DIBUJA EL BALÓN */
	static void drawBall(Mat partido);
};

