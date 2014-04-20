#include "VideoManager.h"

VideoCapture VideoManager::video;

/* INICILIZA LA CAPTURA DE VÍDEO */
bool VideoManager::init() {
	/*bool result = video.open(VIDEO_PATH);

	if (result) {
		// Vamos a saltar los primeros minutos de video, ya que contienen información que no nos interesa
		// (himnos y protocolos antes de comenzar el partido) para ello, lo que hacemos es utilizar una propiedad
		// de la captura que nos permite averiguar el número total de frames de un vídeo, y posteriormente calculamos:
		// Ms / Mt * CV_CAP_PROP_FRAME_COUNT, y ese será el número de frames a saltar. Para saltarlos vamos a
		// utilizar la función set, cambiando la propiedad adecuada (0.0505)
		//video.set(CV_CAP_PROP_POS_FRAMES, 0.001 * video.get(CV_CAP_PROP_FRAME_COUNT));
	}*/

	return video.open(VIDEO_PATH);
}

/* OBTIENE EL SIGUIENTE FRAME DE VÍDEO */
bool VideoManager::nextFrame(Mat* partido) {
	return video.read(*partido);
}

