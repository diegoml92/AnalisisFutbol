#include "config_file.h"

class Ball
{
private:
	// ESTAD�STICAS
	float distance;
	float height[MAX_BALL_HEIGHT];
	Mat area_stats;

	// ATRIBUTOS AUXILIARES
	Mat area;
	Point3i lastPoint;

public:
	/* CONSTRUCTOR */
	Ball(void);

	/* INCREMENTA EL VALOR EN LA POSICI�N INDICADA */
	void addPosition(Point3i p);

	/* A�ADE EL VALOR DE LA ALTURA */
	void addHeight(float h);

	/* CALCULA LAS ESTAD�STICAS */
	void calculateStats();

	/* DEVUELVE LA MATRIZ DE POSICIONES ACUMULADAS */
	Mat getArea();

	/* DEVUELVE LAS ESTAD�SITCAS DEL �REA RECORRIDA */
	Mat getAreaStats();

	/* DEVUELVE LA DISTANCIA RECORRIDA */
	float getDistance();
};