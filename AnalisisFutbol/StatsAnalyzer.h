#include "config_file.h"

class StatsAnalyzer
{
private:

	/* NORMALIZA LOS VALORES DE LA MATRIZ ENTRE 0 Y 1 */
	static Mat normalizeAreaStats(Mat m);

public:
	
	/* CALCULA LA DISTANCIA ENTRE DOS PUNTOS */
	static float distance(Point actualPoint, Point lastPoint);

	/* INCREMENTA EL VALOR EN LA POSICIÓN INDICADA */
	static void addPosition(Mat m, Point p);

	/* INCREMENTA LA DISTANCIA RECORRIDA Y ACTUALIZA LA VELOCIDAD */
	static void addDistanceAndSpeed(float* dist, Point actualPoint, Point* lastPoint,
									float* speed, int* nSpeed, float* maxSpeed, int* retries);

	/* DEVUELVE LAS ESTADÍSTICAS DE POSICIONES */
	static Mat getAreaStats(Mat m);

	/* CALCULA TODAS LAS ESTADÍSTICAS */
	static void calculateAllStats();

	/* DETERMINA SI DOS PUNTOS SE CORRESPONDEN CON EL MISMO ELEMENTO */
	static bool isSamePoint(Point p1, Point p2);

	/* DETERMINA SI EL PUNTO ESTÁ EN EL RANGO PARA SER RECUPERADO */
	static bool isInRecoverRange(Point p1, Point p2, int time, float* dist);

	/* ACTUALIZA LAS ESTADÍSTICAS PARA UN JUGADOR RECUPERADO */
	static void updateStats(float* dist, Point actualPoint, Point* lastPoint,
					float* speed, int* nSpeed, float* maxSpeed, int* retries, int nFrames);

};

