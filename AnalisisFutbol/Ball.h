#include "config_file.h"

class Ball
{
private:
	// ESTADISTICAS
	float distance;
	float height[MAX_BALL_HEIGHT];
	Mat area_stats;

	// ATRIBUTOS AUXILIARES
	Mat area;
	Point3i lastPoint;

public:
	/* CONSTRUCTOR */
	Ball(void);

	/* INCREMENTA EL VALOR EN LA POSICION INDICADA */
	void addPosition(int x, int y);

	/* INCREMENTA LA DISTANCIA RECORRIDA */
	void addDistance(Point3i actualPoint);

	/* ANADE EL VALOR DE LA ALTURA */
	void addHeight(float h);

	/* CALCULA LAS ESTAD�STICAS */
	void calculateStats();
};