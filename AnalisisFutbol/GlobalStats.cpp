#include "GlobalStats.h"

// Iniciamos las variables
Team GlobalStats::teams[N_TEAMS] = {Team(), Team()};
Ball GlobalStats::ball = Ball();
vector<Point2f> GlobalStats::locations [N_VIDEOS];

void GlobalStats::clearLocations() {
	for(int i=0; i<N_VIDEOS; i++) {
		GlobalStats::locations[i].clear();
	}
}
