#include "VideoManager.h"

VideoCapture [] VideoManager::video = VideoCapture[N_VIDEOS];

/* INICILIZA LA CAPTURA DE V�DEO */
bool VideoManager::init() {
	/*bool result = video.open(VIDEO_PATH);

	if (result) {
		// Vamos a saltar los primeros minutos de video, ya que contienen informaci�n que no nos interesa
		// (himnos y protocolos antes de comenzar el partido) para ello, lo que hacemos es utilizar una propiedad
		// de la captura que nos permite averiguar el n�mero total de frames de un v�deo, y posteriormente calculamos:
		// Ms / Mt * CV_CAP_PROP_FRAME_COUNT, y ese ser� el n�mero de frames a saltar. Para saltarlos vamos a
		// utilizar la funci�n set, cambiando la propiedad adecuada (0.0505)
		//video.set(CV_CAP_PROP_POS_FRAMES, 0.001 * video.get(CV_CAP_PROP_FRAME_COUNT));
	}*/

	return video[0].open(VIDEO_PATH_0) && video[1].open(VIDEO_PATH_1) && video[2].open(VIDEO_PATH_2) &&
            video[3].open(VIDEO_PATH_3) && video[4].open(VIDEO_PATH_4) && video[5].open(VIDEO_PATH_5);
}

/* OBTIENE EL SIGUIENTE FRAME DE V�DEO */
bool VideoManager::nextFrame(Mat[] partido) {
	return video[0].read(partido[0]) && video[1].read(partido[1]) && video[2].read(partido[2])
            video[3].read(partido[3]) && video[4].read(partido[4]) && video[5].read(partido[5]);
}

