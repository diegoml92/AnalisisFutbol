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
	Point3i lastPoint;

public:

	// IDENTIFICADORES
	static vector<int> id;

	/* CONSTRUCTOR */
	Player(int team_id);

	/* CONSTRUCTOR AUXILIAR */
	Player();

	/* INCREMENTA EL VALOR EN LA POSICION INDICADA */
	void addPosition(Point3i p);

	/* CALCULA LAS ESTADÍSTICAS */
	void calculateStats();

	/* DEVUELVE LA MATRIZ DE POSICIONES ACUMULADAS */
	Mat getArea();

	/* DEVUELVE LAS ESTADÍSITCAS DEL ÁREA RECORRIDA */
	Mat getAreaStats();

	/* DEVUELVE LA DISTANCIA RECORRIDA */
	float getDistance();
};