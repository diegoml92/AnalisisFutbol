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

	// ATRIBUTOS AUXILIARES
	Mat area;
	Point2i lastPoint;

public:

	// IDENTIFICADORES
	static vector<int> id;

	/* CONSTRUCTOR */
	Player(int team_id);

	/* CONSTRUCTOR AUXILIAR */
	Player();

	/* INCREMENTA EL VALOR EN LA POSICION INDICADA */
	void addPosition(int x, int y);

	/* INCREMENTA LA DISTANCIA RECORRIDA */
	void addDistance(Point2i actualPoint);

	/* CALCULA LAS ESTAD�STICAS */
	void calculateStats();

	/* DEVUELVE LA MATRIZ DE POSICIONES ACUMULADAS */
	Mat getArea();

	/* DEVUELVE LAS ESTAD�SITCAS DEL �REA RECORRIDA */
	Mat getAreaStats();

	/* DEVUELVE LA DISTANCIA RECORRIDA */
	float getDistance();
};