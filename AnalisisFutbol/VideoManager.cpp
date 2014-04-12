#include "VideoManager.h"

cv::VideoCapture VideoManager::video;

bool VideoManager::init() {
	bool result = video.open(VIDEO_PATH);

	if (result) {
		// Vamos a saltar los primeros minutos de video, ya que contienen informaci�n que no nos interesa
		// (himnos y protocolos antes de comenzar el partido) para ello, lo que hacemos es utilizar una propiedad
		// de la captura que nos permite averiguar el n�mero total de frames de un v�deo, y posteriormente calculamos:
		// Ms / Mt * CV_CAP_PROP_FRAME_COUNT, y ese ser� el n�mero de frames a saltar. Para saltarlos vamos a
		// utilizar la funci�n set, cambiando la propiedad adecuada (0.0505)
		video.set(CV_CAP_PROP_POS_FRAMES, 0.001 * video.get(CV_CAP_PROP_FRAME_COUNT));
	}

	return result;
}

bool VideoManager::nextFrame(cv::Mat* partido) {
	return video.read(*partido);
}

