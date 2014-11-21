#include "config_file.h"
#include "Player.h"

class Team
{
private:
	// IDENTIFICADORES
	static int id;
	int team_id;

	// ESTAD�STICAS
	float distance;
	Mat area_stats;

    // ATRIBUTOS AUXILIARES
	Mat area;

	// JUGADORES
	vector<Player> players;

public:
	/* CONSTRUCTOR */
	Team(void);

	/* CALCULA LAS ESTAD�STICAS */
	void calculateStats();

	/* DEVUELVE LA MATRIZ DE POSICIONES ACUMULADAS */
	Mat getArea();

	/* DEVUELVE LAS ESTAD�SITCAS DEL �REA RECORRIDA */
	Mat getAreaStats();

	/* DEVUELVE LA DISTANCIA RECORRIDA */
	float getDistance();
};

