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
	std::cout<<static_cast<int>(video[0].get(CV_CAP_PROP_FOURCC))<<std::endl;
	std::cout<<video[0].get(CV_CAP_PROP_FPS)<<std::endl;
    return open;
}

/* OBTIENE EL SIGUIENTE FRAME DE VÍDEO */
bool VideoManager::nextFrame(Mat frame[]) {
	bool hasNext = true;
	for(int i=0;i<N_VIDEOS && hasNext;i++) {
		hasNext &= video[i].read(frame[i]);
	}
	return hasNext;
}

/* UNE LAS SECUENCIAS DE VÍDEO EN UNA SOLA IMAGEN */
Mat VideoManager::joinSequences(Mat frame[]) {
    Mat dst = Mat::zeros(Size(VIDEO_WIDTH*3, VIDEO_HEIGHT*2+8), CV_8UC3);
    for(int i=0; i<N_VIDEOS; i++) {
        int x = VIDEO_WIDTH * (i%3);
        int y = VIDEO_HEIGHT * (i/3);
		frame[i].copyTo(dst(Rect(Point(x,y),Point(x+frame[i].cols,y+frame[i].rows))));
    }
    return dst;
}