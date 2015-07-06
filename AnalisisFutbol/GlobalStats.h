#include "config_file.h"
#include "Player.h"

/*
   CONTIENE LAS INSTANCIAS GLOBALES DE LOS JUGADORES
   PARA EL CORRECTO MANEJO DE LAS ESTADISITCAS EN EL
   RESTO DE CLASES
*/
class GlobalStats {

public:

	// Irá almacenando cada fotograma del vídeo de entrada
	static Mat frame[N_VIDEOS];
	// Almacenará el umbral actualizado según los valores del filtro
	static Mat filter[N_VIDEOS];

	// Representacion del terreno de juego 2D
	static Mat field2D;
	// Terreno de juego para visualizar las estadísticas
	static Mat soccer_field;

	/*
	* Vector que almacenará los elementos detectados
	* en cada iteración por cada una de las cámaras
	*/	
	static vector<Rect> locations [N_VIDEOS];
	/* Equivalente al anterior, pero en posiciones 2D*/
	static vector<Point2f> locations2D [N_VIDEOS];


	/* VACÍA LOS VECTORES DE POSICIONES */
	static void clearLocations();

	/* CALCULA EL CENTRO DE UN RECTÁNGULO */
	static Point getCenter(Rect r);

	/* CALCULA EL RECTÁNGULO A PARTIR DEL CENTRO */
	static Rect getPlayerRect(Point pos);

	/* INDICA SE ESTÁN TODOS LOS JUGADORES DETECTADOS */
	static bool allPlayersDetected();

	/* DEVUELVE EL NÚMERO DE JUGADORES DETECTADOS */
	static int totalPlayers();

	/* DETERMINA EL "COLOR" DE UN JUGADOR A PARTIR DE SU HISTOGRAMA */
	static Scalar calculateColor(vector<Mat> hist);
};

