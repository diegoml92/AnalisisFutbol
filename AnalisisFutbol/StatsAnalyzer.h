#include "config_file.h"

class StatsAnalyzer
{
private:

	/* ALMACENA TODAS LAS POSICIONES DE LA PELOTA */
	static Mat ballArea;

	/* ALMACENA LA DISTANCIA RECORRIDA POR LA PELOTA */
	static float ballDistance;

	/* GUARDA EL ÚLTIMO PUNTO ENCONTRADO */
	static Point3i lastPoint;

	/* NORMALIZA LOS VALORES DE LA MATRIZ ENTRE 0 Y 1 */
	static Mat normalizeBallAreaStats();

	/* CALCULA LA DISTANCIA ENTRE DOS PUNTOS */
	static float distance(Point3i p);

public:

	/* INCREMENTA EL VALOR EN LA POSICIÓN INDICADA */
	static void addBallPosition(int x, int y);

	/* INCREMENTA LA DISTANCIA RECORRIDA */
	static void addBallDistance(int x, int y, int z);

	/* DEVUELVE LAS ESTADÍSTICAS DEL BALÓN */
	static Mat getBallStats();

};

