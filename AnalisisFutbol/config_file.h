// Se incluyen las librerías necesarias para el proyecto
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\video\tracking.hpp>
#include <iostream>

// Indicamos el namespace para no tener que escribirlo delante de cada operación que lo requiera
using namespace cv;

#define N_VIDEOS 6

#define VIDEO_WIDTH 1920
#define VIDEO_HEIGHT 1080

#define SAVE_RESULT_SEQ false

#define FPS 25

#define MS_TO_KMH 3.6

#define N_PLAYERS 11
#define N_TEAMS 2

#define SEARCH_WINDOW 15,15

#define ANALYZER_VIDEO_SIZE_RELATION 8

#define SOCCER_FIELD_WIDTH 1100
#define SOCCER_FIELD_HEIGHT 680

#define CIRCLE_RADIUS 91.5
#define AREA_HEIGHT 403.2
#define AREA_WIDTH 165
#define PENALTY_AREA_HEIGHT 183.2
#define PENALTY_AREA_WIDTH 55
#define PENALTY_POINT 110
#define GOAL_SIZE 73.2

#define THRESHOLD_W "Imagen filtrada"
#define VIDEO_W "Imagen original"
#define SGUI_W "Panel de Estadísticas"
#define STATS_W "Estadísticas"
#define FIELD_FILTER_W "Filtro del campo"
#define PLAYER_SIZE_W "Player size"

#define VIDEO_PATH "C:/Proyecto/Sequences/Seq"
#define VIDEO_FORMAT ".avi"
#define BG_FORMAT "_bg.jpg"

#define FIELD2D_PATH "C:/Proyecto/soccer_field.jpg"
#define FIELD2DSMALL_PATH "C:/Proyecto/soccer_field_small.jpg"

#define BHATTACHARYYA_THRES 0.45

#define R_BUTTON_RADIUS 5
#define R_BUTTON_ACTIVE_RADIUS 4
#define N_RBUTTON_SGROUPS 2

#define GUI_COLOR 250,175,0
#define GUI_AUX_COLOR 235,220,155
#define COLOR_WHITE 255,255,255
#define COLOR_BLACK 0,0,0

#define PLAYER_WIDTH 35
#define PLAYER_HEIGHT 90

#define MIN_WIDTH 20
#define MAX_WIDTH 70
#define MIN_HEIGH 65
#define MAX_HEIGH 130
