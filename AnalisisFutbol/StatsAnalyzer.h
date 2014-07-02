#include "config_file.h"

class StatsAnalyzer
{
private:

	/* ALMACENA TODAS LAS POSICIONES DE LA PELOTA */
	static Mat ballArea;

	/* NORMALIZA LOS VALORES DE LA MATRIZ ENTRE 0 Y 1 */
	static Mat normalizeBallAreaStats();

public:

	/* INCREMENTA EL VALOR EN LA POSICI�N INDICADA */
	static void addBallPosition(int x, int y);

	/* DEVUELVE LAS ESTAD�STICAS DEL BAL�N */
	static Mat getBallStats();

};

