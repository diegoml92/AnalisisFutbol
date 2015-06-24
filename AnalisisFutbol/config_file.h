// Se incluyen las librerías necesarias para el proyecto
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\video\tracking.hpp>
#include <list>
#include <iostream>

// Indicamos el namespace para no tener que escribirlo delante de cada operación que lo requiera
using namespace cv;

// SE DEFINEN UNA SERIE DE CONSTANTES DE CONFIGURACIÓN

/* VENTANAS */
#define VIDEO_W "SECUENCIAS DE VIDEO"
#define FIELD_W "POSICIONES 2D"
#define STATS_W "ESTADÍSTICAS"

/* PATHS */
#define VIDEO_PATH "./Sequences/Seq"
#define VIDEO_FORMAT ".avi"
#define BG_FORMAT "_bg.jpg"
#define FIELD2D_PATH "./img/soccer_field.jpg"
#define FIELD2DSMALL_PATH "./img/soccer_field_small.jpg"

/* VIDEO */
#define N_VIDEOS 6
#define VIDEO_WIDTH 960
#define VIDEO_HEIGHT 540
#define FPS 25

/* MEDIDAS DEL TERRENO DE JUEGO */
#define SOCCER_FIELD_WIDTH 1080
#define SOCCER_FIELD_HEIGHT 680
#define CIRCLE_RADIUS 91.5
#define AREA_HEIGHT 403.2
#define AREA_WIDTH 165
#define PENALTY_AREA_HEIGHT 183.2
#define PENALTY_AREA_WIDTH 55

/* INTERFAZ GRÁFICA */
#define STATS_W_WIDTH 280
#define STATS_W_HEIGHT 335
#define GUI_COLOR 250,175,0
#define GUI_AUX_COLOR 235,220,155
#define COLOR_WHITE 255,255,255
#define COLOR_BLACK 0,0,0

/* RANGOS DE TAMAÑO */
#define PLAYER_WIDTH 17
#define PLAYER_HEIGHT 45
#define MIN_WIDTH 10
#define MAX_WIDTH 35
#define MIN_HEIGH 32
#define MAX_HEIGH 65

/* OTRAS */
#define RGB 256
#define N_BINS 16
#define N_CHANNELS 3
#define MS_TO_KMH 3.6
#define MAX_PLAYERS 25
#define FIELD_MARGIN 10
#define SAMPLING_RATE 5	
#define TIME_TO_DELETE 3
#define DIFF_POINTS_DIST 5
#define SEARCH_WINDOW 15,10
#define ANALYZER_VIDEO_SIZE_RELATION 4

// DEBUG!!!
#define SAVE_RESULT_SEQ false
