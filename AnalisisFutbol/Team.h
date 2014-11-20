#include "config_file.h"
#include "Player.h"

class Team
{
private:
	// IDENTIFICADORES
	static int id;
	int team_id;

	// JUGADORES
	vector<Player> players;

public:
	Team(void);
};

