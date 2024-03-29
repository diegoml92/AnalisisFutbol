#ifndef PLAYER_H
#define PLAYER_H

#include "config_file.h"

/* JUGADORES */
class Player
{
private:

	// IDENTIFICADORES
	int player_id;

	// ESTADISTICAS
	float distance;
	Mat area_stats;
	float avgSpeed;
	float maxSpeed;

	// ATRIBUTOS AUXILIARES
	Mat area;
	Point lastPoint;
	int nSpeed;
	bool bPos[N_VIDEOS];
	Point camPos[N_VIDEOS];
	Vec2i shift[N_VIDEOS];
	vector<Mat> histogram;
	int deletionCounter;
	int retries;

public:

	// IDENTIFICADORES
	static int id;

	/* CONSTRUCTOR */
	Player(Point pos, vector<Mat> histogram);

	/* CONSTRUCTOR AUXILIAR */
	Player(Player* p);

	/* INCREMENTA EL VALOR EN LA POSICION INDICADA */
	void addPosition(Point p);

	/* ACTUALIZA ESTAD�STICAS SI SE RECUPERA EL JUGADOR */
	void updateStats(Point pos);

	/* CALCULA LAS ESTAD�STICAS */
	void calculateStats();

	/* DEVUELVE LA �LTIMA POSICI�N TRACKEADA (SI LA HAY) */
	Point getPosition();

	/* DEVUELVE LAS ESTAD�SITCAS DEL �REA RECORRIDA */
	Mat getAreaStats();

	/* DEVUELVE LA DISTANCIA RECORRIDA */
	float getDistance();

	/* DEVUELVE LA VELOCIDAD M�XIMA */
	float getMaxSpeed();

	/* DEVUELVE LA VELOCIDAD MEDIA */
	float getAvgSpeed();

	/* DEVUELVE EL PUNTO ACUTAL */
	Point getLastPosition();

	/* DEVUELVE EL HISTOGRAMA DEL JUGADOR */
	vector<Mat> getHistogram();

	/* DEVUELVE EL VALOR i */
	bool getBPos(int i);

	/* DEVUELVE EL VALOR i */
	Point getCamPos(int i);

	/* ACTIVA LA C�MARA i */
	void setCamPos(int i, Point p);

	/* DESACTIVA LA C�MARA i */
	void unSetCamPos(int i);

	/* DEVUELVE EL �LTIMO DESPLAZAMIENTO EN LA C�MARA i */
	Vec2i getShift(int i);

	/* ESTABLECE EL DESPLAZAMIENTO */
	void setShift(Point shift, int i);

	/* DEVUELVE EL PLAYER ID */
	int getPlayerId();

	/* INICIA EL CONTADOR DE BORRADO A 0 */
	void startDeletionCounter();

	/* INCREMENTA EL CONTADOR DE BORRADO */
	void increaseDeletionCounter();

	/* DEVUELVE EL VALOR DEL CONTADOR DE BORRADO */
	int getDeletionCounter();

	/* SE IMPLEMENTA ESTE M�TODO PARA PERMITIR LA COMPARACI�N*/
	bool operator==(const Player other) const;
};

#endif //PLAYER_H