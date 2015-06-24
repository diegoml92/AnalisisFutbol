#include "config_file.h"

class StatsAnalyzer
{
private:

	// Terreno de juego para visualizar las estad�sticas
	static Mat soccer_field;

	/* NORMALIZA LOS VALORES DE LA MATRIZ ENTRE 0 Y 1 */
	static Mat normalizeAreaStats(Mat m);

	/* CALCULA LA DISTANCIA ENTRE DOS PUNTOS */
	static float distance(Point actualPoint, Point lastPoint);

public:

	/* INCREMENTA EL VALOR EN LA POSICI�N INDICADA */
	static void addPosition(Mat m, Point p);

	/* INCREMENTA LA DISTANCIA RECORRIDA Y ACTUALIZA LA VELOCIDAD */
	static void addDistanceAndSpeed(float* distance, Point actualPoint, Point lastPoint,
		                            float* speed, int* nSpeed, float* maxSpeed);

	/* DEVUELVE LAS ESTAD�STICAS DE POSICIONES */
	static Mat getAreaStats(Mat m);

	/* CALCULA TODAS LAS ESTAD�STICAS */
	static void calculateAllStats();

	/* DETERMINA SI DOS PUNTOS SE CORRESPONDEN CON EL MISMO ELEMENTO */
	static bool isSamePoint(Point p1, Point p2);

	/* DETERMINA SI EL PUNTO EST� EN EL RANGO PARA SER RECUPERADO */
	static bool isInRecoverRange(Point p1, Point p2, int time, float* dist);

	/* ACTUALIZA LAS ESTAD�STICAS PARA UN JUGADOR RECUPERADO */
	static void updateStats(float* dist, Point actualPoint, Point lastPoint,
							float* speed, int* nSpeed, float* maxSpeed);

};

