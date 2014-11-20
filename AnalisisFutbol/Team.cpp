#include "Team.h"

int Team::id = 0;

/* CONSTRUCTOR */
Team::Team(void) {
	Team::team_id = id++;
	Player::id.push_back(0);
}
