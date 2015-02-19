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

#define N_PLAYERS 11
#define N_TEAMS 2

#define DESP 335
#define LONG 265

#define ANALYZER_VIDEO_SIZE_RELATION 8

#define SOCCER_FIELD_WIDTH 1200
#define SOCCER_FIELD_HEIGHT 680

#define MAX_BALL_HEIGHT 15

#define THRESHOLD_W "Imagen filtrada"
#define VIDEO_W "Imagen original"
#define GUI_W "Panel de control"
#define SGUI_W "Panel de Estadísticas"
#define STATS_W "Estadísticas"
#define FIELD_FILTER_W "Filtro del campo"
#define PLAYER_SIZE_W "Player size"
#define BALL_SIZE_W "Ball size"

#define VIDEO_PATH "C:/Proyecto/Sequences/Seq"
#define VIDEO_FORMAT ".avi"
#define BG_FORMAT "_bg.jpg"

#define FIELD2D_PATH "C:/Proyecto/soccer_field.jpg"
#define FIELD2DSMALL_PATH "C:/Proyecto/soccer_field_small.jpg"

#define BHATTACHARYYA_THRES 0.45

#define R_BUTTON_RADIUS 5
#define R_BUTTON_ACTIVE_RADIUS 4
#define R_BUTTON_LEFT_MARGIN 75
#define R_BUTTON_TOP_MARGIN 40
#define R_BUTTON_GROUP_SHIFT 250
#define R_BUTTON_VERT_SHIFT 20
#define R_BUTTON_VERT_GROUP_SHIFT 80
#define N_RBUTTON_GROUPS 2
#define N_RBUTTONS 3
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

#define MIN_BALL_SIZE 15
#define MAX_BALL_SIZE 30
