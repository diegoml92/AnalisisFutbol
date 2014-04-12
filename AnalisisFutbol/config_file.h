// Se incluyen las librer�as necesarias para el proyecto
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

#define CALIB false
#define TRACKING false

#define MAX_RGB 255

#define MAX_BALL_SIZE 10

#define THRESHOLD_W "Imagen filtrada"
#define VIDEO_W "Imagen original"
#define GUI_W "Barras de desplazamiento"

#define VIDEO_PATH "C:/Proyecto/Barca_Madrid.mp4"

#define BHATTACHARYYA_THRES 0.45







/*

// Variable booleana que nos permitir� decidir si utilizar trackbars o no. Cuando el filtro del c�sped sea
// lo suficientemente bueno podemos desactivarlo
const bool CALIB = false;
// Se indica si queremos hacer el seguimiento de los jugadores. Hasta que no tengamos preparado el filtro del
// campo no nos interesa activar esta opci�n
const bool TRACKING = false;


// Constante que indica el valor m�ximo que se puede dar a una trackbar
const int MAX_RGB = 255;

// Valores para el rango de tama�o de los jugadores
int MIN_WIDTH = 10, MIN_HEIGH = 25;
int MAX_WIDTH = 45, MAX_HEIGH = 80;

// Tama�o m�ximo para el bal�n
const int MAX_BALL_SIZE = 10;

// Nombres de las ventanas que ser�n utilizadas
const cv::string THRESHOLD_W = "Imagen filtrada";
const cv::string VIDEO_W = "Imagen original";
const cv::string GUI_W = "Barras de desplazamiento";

// Ruta del fichero donde se encuentra el video a analizar
const cv::string VIDEO_PATH = "C:/Proyecto/Barca_Madrid.mp4";

// Valor a utilizar cuando se comparen los histogramas
const float BHATTACHARYYA_THRES = 0.45f;
*/
