#include "config_file.h"
#include "VideoManager.h"
#include "TrackingObj.h"
#include "GUI.h"
#include "EventManager.h"
#include "FieldFilter.h"
#include "From3DTo2D.h"

/* FUNCIÓN DE ENTRADA AL PROGRAMA */
int main(int argc, char* argv[]) {

	VideoManager::init();					// Cargamos el vídeo que vamos a utilizar

	Mat partido[N_VIDEOS];					// Irá almacenando cada fotograma del vídeo de entrada
	Mat umbral[N_VIDEOS];					// Almacenará el umbral actualizado según los valores del filtro

	Mat bg[N_VIDEOS];						// Almacenará los backgrounds obtenidos de cada secuencia
	for(int i=0; i<N_VIDEOS; i++) {
		std::stringstream path;
		path << "C:/Proyecto/Sequences/Seq" << i << "_bg.jpg";
		bg[i] = imread(path.str());
	}

	namedWindow(GUI_W);						// Creamos la ventana para la interfaz
	GUI::initGUI();							// Inicializamos la interfaz gráfica
	EventManager::initMouseListener();		// Inicializamos el controlador de eventos de ratón

	From3DTo2D::initProjectionMatrices();	// Inicializamos las matrices de proyección

	/*	
	*	Bucle en el que vamos pasando los frames del video con la función nextFrame,
    *   que coge el frame actual del video y lo guarda en la matriz partido. Cuando nextFrame
	*	devuelva false, el vídeo abrá acabado.
	*/
	while(VideoManager::nextFrame(partido)) {

		Mat paint;
		From3DTo2D::field2D.copyTo(paint);
        
        for(int i=0; i<N_VIDEOS; i++) {

			long a = getTickCount();

            // Filtramos el campo en el partido
            umbral[i] = FieldFilter::discardField(partido[i].clone(), bg[i]);

			long b = getTickCount();

            TrackingObj::trackObject(umbral[i],partido[i], i, paint);	// Hacemos el tracking de los elementos del campo

			long c = getTickCount();

            GUI::showGUI();                     // Mostramos la interfaz
            if(GUI::isActiveBallSize()) {       // Creamos las trackbars si corresponde
                namedWindow(BALL_SIZE_W);
                GUI::ballSizeTrackbars();
            } else {
                destroyWindow(BALL_SIZE_W);
            }
            if(GUI::isActiveFieldThres()) {     // Creamos las trackbars si corresponde
                namedWindow(FIELD_FILTER_W);
                GUI::fieldThresTrackbars();
            } else {
                destroyWindow(FIELD_FILTER_W);
            }
            if(GUI::isActivePlayerSize()) {     // Creamos las trackbars si corresponde
                namedWindow(PLAYER_SIZE_W);
                GUI::playerSizeTrackbars();
            } else {
                destroyWindow(PLAYER_SIZE_W);
            }

			long d = getTickCount();
            
            //pyrDown(partido[i], partido[i], Size(partido[i].cols/2, partido[i].rows/2));
            //imshow(VIDEO_W, partido[i]);					// Mostramos la imagen original
            //pyrDown(umbral, umbral, Size(umbral.cols/2, umbral.rows/2));
            //imshow(THRESHOLD_W, umbral);				// Mostramos también el threshold

            //while(waitKey()!=13);
            //waitKey(1);								// No aparecerá la imagen si no utlizamos este waitKey

			double b_a, c_b, d_c;
			b_a = (b-a) / getTickFrequency();
			c_b = (c-b) / getTickFrequency();
			d_c = (d-c) / getTickFrequency();

			std::cout << "Threshold " << i << ": " << b_a << std::endl;
			std::cout << "Detection " << i << ": " << c_b << std::endl;
			std::cout << "GUI       " << i << ": " << d_c << std::endl;

        }
        
		long a = getTickCount();

        Mat join = VideoManager::joinSequences(partido);

		long b = getTickCount();

		//pyrDown(join, join, Size(join.cols/2, join.rows/2));
		pyrDown(join, join, Size(join.cols/2, join.rows/2));
		pyrDown(join, join, Size(join.cols/2, join.rows/2));

		long c = getTickCount();

		double b_a, c_b;
		b_a = (b-a) / getTickFrequency();
		c_b = (c-b) / getTickFrequency();
		std::cout << "Join " << ": " << b_a << std::endl;
		std::cout << "Pyr  " << ": " << c_b << std::endl;

        imshow(VIDEO_W, join);
        waitKey(1);

	}
}