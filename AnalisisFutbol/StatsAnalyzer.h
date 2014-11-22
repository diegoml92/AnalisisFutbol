#include "config_file.h"

class StatsAnalyzer
{
private:

	// ESTAD�STICAS
	static Mat ballAreaStats;
	static vector<vector<Mat>> playerAreaStats;
	static Mat teamAreaStats;

	/* NORMALIZA LOS VALORES DE LA MATRIZ ENTRE 0 Y 1 */
	static Mat normalizeAreaStats(Mat m);

	/* CALCULA LA DISTANCIA ENTRE DOS PUNTOS */
	static float distance(Point3i actualPoint, Point3i lastPoint);

public:

	/* INCREMENTA EL VALOR EN LA POSICI�N INDICADA */
	static void addPosition(Mat m, int x, int y);

	/* INCREMENTA LA DISTANCIA RECORRIDA */
	static void addDistance(float distance, Point2i actualPoint, Point2i lastPoint);
	static void addDistance(float distance, Point3i actualPoint, Point3i lastPoint);

	/* A�ADE EL VALOR DE LA ALTURA */
	static void addHeight(float h, float height[]);

	/* DEVUELVE LAS ESTAD�STICAS DE POSICIONES */
	static Mat getAreaStats(Mat m);

	/* CALCULA TODAS LAS ESTAD�STICAS */
	static void calculateAllStats();

};

