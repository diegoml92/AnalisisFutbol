// Se incluyen las librerías necesarias para el proyecto
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>

// Indicamos el namespace para no tener que escribirlo delante de cada operación que lo requiera
using namespace cv;

#define N_VIDEOS 6
#define VIDEO_WIDTH 1920
#define VIDEO_HEIGHT 1080

#define TRACKING false

#define MAX_RGB 255

#define THRESHOLD_W "Imagen filtrada"
#define VIDEO_W "Imagen original"
#define GUI_W "Panel de control"
#define FIELD_FILTER_W "Filtro del campo"
#define PLAYER_SIZE_W "Player size"
#define BALL_SIZE_W "Ball size"

#define VIDEO_PATH_0 "C:/Proyecto/Sequences/Film Role-0 ID-0 T-2 m00s00-000-m00s00-185.avi"
#define VIDEO_PATH_1 "C:/Proyecto/Sequences/Film Role-0 ID-1 T-2 m00s00-000-m00s00-185.avi"
#define VIDEO_PATH_2 "C:/Proyecto/Sequences/Film Role-0 ID-2 T-2 m00s00-000-m00s00-185.avi"
#define VIDEO_PATH_3 "C:/Proyecto/Sequences/Film Role-0 ID-3 T-2 m00s00-000-m00s00-185.avi"
#define VIDEO_PATH_4 "C:/Proyecto/Sequences/Film Role-0 ID-4 T-2 m00s00-000-m00s00-185.avi"
#define VIDEO_PATH_5 "C:/Proyecto/Sequences/Film Role-0 ID-5 T-2 m00s00-000-m00s00-185.avi"

#define BHATTACHARYYA_THRES 0.45

#define BUTTON_WIDTH 60
#define BUTTON_HEIGHT 25
#define BUTTON_SIZE 60,25
#define BUTTON_LEFT_MARGIN 75
#define BUTTON_TOP_MARGIN 210
#define BUTTON_SHIFT 170
#define N_BUTTONS 3

#define R_BUTTON_RADIUS 5
#define R_BUTTON_ACTIVE_RADIUS 4
#define R_BUTTON_LEFT_MARGIN 75
#define R_BUTTON_TOP_MARGIN 40
#define R_BUTTON_GROUP_SHIFT 250
#define R_BUTTON_VERT_SHIFT 20
#define R_BUTTON_VERT_GROUP_SHIFT 80
#define N_RBUTTON_GROUPS 3
#define N_RBUTTONS 3







/*

// Variable booleana que nos permitirá decidir si utilizar trackbars o no. Cuando el filtro del césped sea
// lo suficientemente bueno podemos desactivarlo
const bool CALIB = false;
// Se indica si queremos hacer el seguimiento de los jugadores. Hasta que no tengamos preparado el filtro del
// campo no nos interesa activar esta opción
const bool TRACKING = false;


// Constante que indica el valor máximo que se puede dar a una trackbar
const int MAX_RGB = 255;

// Valores para el rango de tamaño de los jugadores
int MIN_WIDTH = 10, MIN_HEIGH = 25;
int MAX_WIDTH = 45, MAX_HEIGH = 80;

// Tamaño máximo para el balón
const int MAX_BALL_SIZE = 10;

// Nombres de las ventanas que serán utilizadas
const cv::string THRESHOLD_W = "Imagen filtrada";
const cv::string VIDEO_W = "Imagen original";
const cv::string GUI_W = "Barras de desplazamiento";

// Ruta del fichero donde se encuentra el video a analizar
const cv::string VIDEO_PATH = "C:/Proyecto/Barca_Madrid.mp4";

// Valor a utilizar cuando se comparen los histogramas
const float BHATTACHARYYA_THRES = 0.45f;
*/
