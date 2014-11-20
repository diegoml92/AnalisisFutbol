#include "config_file.h"
#include "Player.h"

class Team
{
private:
	// IDENTIFICADORES
	static int id;
	int team_id;

	// ESTADÍSTICAS
	float distance;
	Mat area_stats;

    // ATRIBUTOS AUXILIARES
	Mat area;

	// JUGADORES
	vector<Player> players;

public:
	/* CONSTRUCTOR */
	Team(void);

	/* CALCULA LAS ESTADÍSTICAS */
	void calculateStats();

	/* DEVUELVE LA MATRIZ DE POSICIONES ACUMULADAS */
	Mat getArea();

	/* DEVUELVE LAS ESTADÍSITCAS DEL ÁREA RECORRIDA */
	Mat getAreaStats();

	/* DEVUELVE LA DISTANCIA RECORRIDA */
	float getDistance();
};

