#ifndef VIEWDATA_H
#define VIEWDATA_H

#include "Headers/datastructures.h"
#include "Headers/gamesession_model.h"

struct ViewData
{
    int commandId;
    ViewData(GameSessionModel* model);
    const int tableSize;
    int tileSize;
    int windowWidth;
    int windowHeight;
    Player activePlayer;
    Position RedPosition;
    Position BluePosition;
    void RefreshPositions();
private:
    GameSessionModel* _gameSessionModel;
};

#endif // VIEWDATA_H
