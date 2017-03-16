#ifndef GAMESESSIONMODEL_H
#define GAMESESSIONMODEL_H
#include <QObject>
#include <QVector>
#include "Headers/datastructures.h"


struct RoboPig
{
    const Player PigName;
    Position CurrentPos;
    int Health;
    Command Memory [5];
    RoboPig(Player name, int PosX, int PosY, int Angle, int Health);
};

class GameSessionModel : public QObject
{
    Q_OBJECT
public:
    GameSessionModel(int tableSize);
    GameSessionModel(int tableSize,int redHP, int blueHP, Position redPos, Position bluePos);
    ~GameSessionModel();
    bool setCommand(unsigned int commandSlot,Icon commandIndex);

   // int GetTileState(int posX, int posY);
    int GetTableSize();
  //  void GetDirections(int posX, int posY);
    bool IsGameWon();
    GameStats GetGameStats();
    Position GetPosition(Player);
private:

  //  QVector<QVector<GameTile*> > _gameTable;
    void executeCommand(RoboPig* pig );
    void executeParallelCommands();
    void endRound();
    void shootLaser(RoboPig* pig);
    void hitWithFist(RoboPig* pig);
    Position calculateStep(RoboPig* pig);
    const int _tableSize;
    RoboPig* _RedPlayer;
    RoboPig* _BluePlayer;
    Player _activePlayer;
    int _currentCommand;
  //  bool isValidPosition(int posX, int posY);
  //  bool canStep(int posX, int posY);

signals:
    void GameWon(int);
    void MakeStep(Command, Command);
    void changePlayer(Player);
    void laserShot(Player, bool);
    void newRound();

public slots:
    void processRound();
    void GameStep();

};

#endif // GAMESESSIONMODEL_H
