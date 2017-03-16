#ifndef MAINWINDOWMODEL_H
#define MAINWINDOWMODEL_H

#include <QObject>
#include "Headers/gamesession_model.h"

class MainWindowModel : public QObject
{
    Q_OBJECT
public:
    MainWindowModel();
    ~MainWindowModel();
    void InitGame(int n);
    bool loadGame(int,int,int,Position,Position);
private:
    GameSessionModel* _gameSessionModel;
    bool _InitializeGameSession(int n);

signals:
void InitFailed();
void GameReady(GameSessionModel*);
};

#endif // MAINWINDOWMODEL_H
