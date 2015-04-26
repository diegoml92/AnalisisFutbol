#ifndef PLAYER_H
#define PLAYER_H

#include "config_file.h"

/* JUGADORES */
class Player
{
private:

	// IDENTIFICADORES
	int player_id;
	int team_id;

	// ESTADISTICAS
	float distance;
	Mat area_stats;
	float avgSpeed;
	float maxSpeed;

	// ATRIBUTOS AUXILIARES
	Mat area;
	Point lastPoint;
	int nSpeed;

	/* DETERMINA SI EL PUNTO EST� EN UN RANGO V�LIDO */
	bool isInRange(Point pos);

public:

	// IDENTIFICADORES
	static vector<int> id;

	/* CONSTRUCTOR */
	Player(int team_id, Point pos);

	/* INCREMENTA EL VALOR EN LA POSICION INDICADA */
	void addPosition(Point p);

	/* CALCULA LAS ESTAD�STICAS */
	void calculateStats();

	/* DEVUELVE LA MATRIZ DE POSICIONES ACUMULADAS */
	Mat getArea();

	/* DEVUELVE LAS ESTAD�SITCAS DEL �REA RECORRIDA */
	Mat getAreaStats();

	/* DEVUELVE LA DISTANCIA RECORRIDA */
	float getDistance();

	/* DEVUELVE LA VELOCIDAD M�XIMA */
	float getMaxSpeed();

	/* DEVUELVE LA VELOCIDAD MEDIA */
	float getAvgSpeed();

	/* DEVUELVE EL PUNTO ACUTAL */
	Point getPosition();
};

#endif //PLAYER_H