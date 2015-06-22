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
	vector<Mat> histogram;
	int deletionCounter;

public:

	// IDENTIFICADORES
	static int id;

	/* CONSTRUCTOR */
	Player(Point pos, vector<Mat> histogram);

	/* INCREMENTA EL VALOR EN LA POSICION INDICADA */
	void addPosition(Point p);

	/* CALCULA LAS ESTADÍSTICAS */
	void calculateStats();

	/* DEVUELVE LAS ESTADÍSITCAS DEL ÁREA RECORRIDA */
	Mat getAreaStats();

	/* DEVUELVE LA DISTANCIA RECORRIDA */
	float getDistance();

	/* DEVUELVE LA VELOCIDAD MÁXIMA */
	float getMaxSpeed();

	/* DEVUELVE LA VELOCIDAD MEDIA */
	float getAvgSpeed();

	/* DEVUELVE EL PUNTO ACUTAL */
	Point getPosition();

	/* DEVUELVE EL HISTOGRAMA DEL JUGADOR */
	vector<Mat> getHistogram();

	/* DEVUELVE EL VALOR i */
	bool getBPos(int i);

	/* DEVUELVE EL VALOR i */
	Point getCamPos(int i);

	/* ACTIVA LA CÁMARA i */
	void setCamPos(int i, Point p);

	/* DESACTIVA LA CÁMARA i */
	void unSetCamPos(int i);

	/* DEVUELVE EL PLAYER ID */
	int getPlayerId();

	/* INICIA EL CONTADOR DE BORRADO A 0 */
	void startDeletionCounter();

	/* INCREMENTA EL CONTADOR DE BORRADO */
	void increaseDeletionCounter();

	/* DEVUELVE EL VALOR DEL CONTADOR DE BORRADO */
	int getDeletionCounter();

	/* SE IMPLEMENTA ESTE MÉTODO PARA PERMITIR LA COMPARACIÓN*/
	bool operator==(const Player &other) const;
};

#endif //PLAYER_H