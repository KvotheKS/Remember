#include "GameData.h"

bool GameData::playerVictory = false;
int GameData::spawn_ID = 0; //define index da spawnList
std::vector<bool> GameData::isAlive(3,true);