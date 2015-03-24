#ifndef TEAM_H
#define TEAM_H

#include "config_file.h"
#include "Player.h"

class Team
{
private:
	// IDENTIFICADORES
	static int id;
	int team_id;

	// HISTOGRAMA
	Mat histogram;

	// ESTADÍSTICAS
	float distance;
	Mat area_stats;

    // ATRIBUTOS AUXILIARES
	Mat area;

	// JUGADORES
	vector<Player> players;

public:
	/* CONSTRUCTOR */
	Team();

	/* CALCULA LAS ESTADÍSTICAS */
	void calculateStats();

	/* DEVUELVE EL IDENTIFICADOR DEL EQUIPO */
	int getTeamId();

	/* INICIALIZA EL HISTOGRAMA */
	void setHistogram(Mat hist);
	
	/* DEVUELVE EL HISTOGRAMA */
	Mat getHistogram();

	/* DEVUELVE LA LISTA DE JUGADORES */
	vector<Player> getPlayers();

	/* DEVUELVE LA LISTA DE JUGADORES */
	vector<Player>* getPlayersP();

	/* DEVUELVE LA MATRIZ DE POSICIONES ACUMULADAS */
	Mat getArea();

	/* DEVUELVE LAS ESTADÍSITCAS DEL ÁREA RECORRIDA */
	Mat getAreaStats();

	/* DEVUELVE LA DISTANCIA RECORRIDA */
	float getDistance();

	/* AÑADE UN NUEVO JUGADOR */
	void createPlayer(Point init);
};

#endif