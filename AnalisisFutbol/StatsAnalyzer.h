#include "config_file.h"

class StatsAnalyzer
{
private:

	/* ALMACENA TODAS LAS POSICIONES DE LA PELOTA */
	static Mat ballArea;

	/* NORMALIZA LOS VALORES DE LA MATRIZ ENTRE 0 Y 1 */
	static Mat normalizeBallAreaStats();

public:

	/* INCREMENTA EL VALOR EN LA POSICIÓN INDICADA */
	static void addBallPosition(int x, int y);

	/* DEVUELVE LAS ESTADÍSTICAS DEL BALÓN */
	static Mat getBallStats();

};

