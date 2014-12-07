#include "VideoManager.h"

VideoCapture VideoManager::video [N_VIDEOS];

/* INICILIZA LA CAPTURA DE VÍDEO */
bool VideoManager::init() {
	bool open = true;
	for(int i=0;i<N_VIDEOS && open;i++) {
		std::stringstream videoPath;
		videoPath << VIDEO_PATH << i << VIDEO_FORMAT;
		open = open && video[i].open(videoPath.str());
	}
    return open;
}

/* OBTIENE EL SIGUIENTE FRAME DE VÍDEO */
bool VideoManager::nextFrame(Mat partido[]) {
	return video[0].read(partido[0]) && video[1].read(partido[1]) && video[2].read(partido[2]) &&
            video[3].read(partido[3]) && video[4].read(partido[4]) && video[5].read(partido[5]);
}

/* UNE LAS SECUENCIAS DE VÍDEO EN UNA SOLA IMAGEN */
Mat VideoManager::joinSequences(Mat partido[]) {
    Mat dst = Mat::zeros(Size(VIDEO_WIDTH*3, VIDEO_HEIGHT*2+8), CV_8UC3);
    for(int i=0; i<N_VIDEOS; i++) {
        int x = VIDEO_WIDTH * (i%3);
        int y = VIDEO_HEIGHT * (i/3);
		partido[i].copyTo(dst(Rect(Point(x,y),Point(x+partido[i].cols,y+partido[i].rows))));
    }
    return dst;
}