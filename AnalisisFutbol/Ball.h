#include "config_file.h"

class Ball
{
private:
	// ESTADÍSTICAS
	float distance;
	float height[MAX_BALL_HEIGHT];
	Mat area_stats;

	// ATRIBUTOS AUXILIARES
	Mat area;
	Point3i lastPoint;

public:
	/* CONSTRUCTOR */
	Ball(void);

	/* INCREMENTA EL VALOR EN LA POSICIÓN INDICADA */
	void addPosition(Point3i p);

	/* AÑADE EL VALOR DE LA ALTURA */
	void addHeight(float h);

	/* CALCULA LAS ESTADÍSTICAS */
	void calculateStats();

	/* DEVUELVE LA MATRIZ DE POSICIONES ACUMULADAS */
	Mat getArea();

	/* DEVUELVE LAS ESTADÍSITCAS DEL ÁREA RECORRIDA */
	Mat getAreaStats();

	/* DEVUELVE LA DISTANCIA RECORRIDA */
	float getDistance();
};