#include "VideoManager.h"

VideoCapture VideoManager::video [N_VIDEOS];

/* INICILIZA LA CAPTURA DE VÍDEO */
bool VideoManager::init() {
	return video[0].open(VIDEO_PATH_0) && video[1].open(VIDEO_PATH_1) && video[2].open(VIDEO_PATH_2) &&
            video[3].open(VIDEO_PATH_3) && video[4].open(VIDEO_PATH_4) && video[5].open(VIDEO_PATH_5);
}

/* OBTIENE EL SIGUIENTE FRAME DE VÍDEO */
bool VideoManager::nextFrame(Mat partido[]) {
	return video[0].read(partido[0]) && video[1].read(partido[1]) && video[2].read(partido[2]) &&
            video[3].read(partido[3]) && video[4].read(partido[4]) && video[5].read(partido[5]);
}

