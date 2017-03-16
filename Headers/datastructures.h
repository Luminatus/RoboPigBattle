#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

enum GameSize{
    Four = 4,
    Six = 6,
    Eight = 8
};

enum Player
{
    BluePlayer = 0,
    RedPlayer = 1,
    None = 2
};

enum Commands{
    Skip=0,
    Rotate=1,
    Move=2,
    Attack=3
};

enum Direction
{
    UpDirection = 0,
    RightDirection = 1,
    DownDirection = 2,
    LeftDirection = 3
};

enum Icon
{
    SkipIcon = 8,
    MoveUpIcon = 4,
    MoveDownIcon = 7,
    MoveRightIcon = 0,
    MoveLeftIcon = 3,
    TurnRightIcon = 2,
    TurnLeftIcon = 1,
    LaserIcon = 5,
    FistIcon = 6,
    LockedIcon=9
};

struct Position
{
    int X;
    int Y;
    int Angle;
    Position(int x, int y, int alpha): X(x), Y(y), Angle(alpha){}
    Position(): X(-1), Y(-1), Angle(0){}
};

struct Command
{
    Commands Type;
    int Value;
};

struct GameStats{
    const Player ActivePlayer;
    const int RedHealth;
    const int BlueHealth;
    const int CommandIndex;
    GameStats(Player player, int red, int blue, int commind) : ActivePlayer(player),BlueHealth(blue),RedHealth(red),CommandIndex(commind){}
};



#endif // DATASTRUCTURES_H
