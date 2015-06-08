#include "VideoManager.h"

VideoCapture VideoManager::video [N_VIDEOS];
int VideoManager::actualFrame;

/* INICIALIZA LA CAPTURA DE VÍDEO */
bool VideoManager::init() {
	bool open = true;
	VideoManager::actualFrame = 0;
	for(int i=0;i<N_VIDEOS && open;i++) {
		std::stringstream videoPath;
		videoPath << VIDEO_PATH << i << VIDEO_FORMAT;
		open = open && video[i].open(videoPath.str());
	}
    return open;
}

/* OBTIENE EL SIGUIENTE FRAME DE VÍDEO */
bool VideoManager::nextFrame(Mat frame[]) {
	bool hasNext = true;
	VideoManager::actualFrame++;
	for(int i=0;i<N_VIDEOS && hasNext;i++) {
		hasNext &= video[i].read(frame[i]);
	}
	return hasNext;
}

/* UNE LAS SECUENCIAS DE VÍDEO EN UNA SOLA IMAGEN */
Mat VideoManager::joinSequences(Mat frame[]) {
    Mat dst = Mat::zeros(Size(VIDEO_WIDTH*3, VIDEO_HEIGHT*2+4), CV_8UC3);
    for(int i=0; i<N_VIDEOS; i++) {
        int x = VIDEO_WIDTH * (i%3);
        int y = VIDEO_HEIGHT * (i/3);
		frame[i].copyTo(dst(Rect(Point(x,y),Point(x+frame[i].cols,y+frame[i].rows))));
    }
    return dst;
}

/* DEVUELVE EL NÚMERO DE FRAME ACTUAL */
int VideoManager::getActualFrame() {
	return VideoManager::actualFrame;
}
