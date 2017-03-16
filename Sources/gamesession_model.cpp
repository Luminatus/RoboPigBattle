#include "Headers/gamesession_model.h"

GameSessionModel::GameSessionModel(int tableSize) :
    _tableSize(tableSize),
    _RedPlayer(new RoboPig(RedPlayer,tableSize/2 -1,0,DownDirection,3)),
    _BluePlayer(new RoboPig(BluePlayer,tableSize/2, tableSize-1, UpDirection,3)),
    _activePlayer(BluePlayer),
    _currentCommand(0)
{

}

GameSessionModel::GameSessionModel(int tableSize, int redHP, int blueHP, Position redPos, Position bluePos) :
    _tableSize(tableSize),
    _RedPlayer(new RoboPig(RedPlayer,redPos.X,redPos.Y,redPos.Angle,redHP)),
    _BluePlayer(new RoboPig(BluePlayer,bluePos.X,bluePos.Y,bluePos.Angle,blueHP)),
    _activePlayer(BluePlayer),
    _currentCommand(0)
{

}

GameSessionModel::~GameSessionModel()
{

}

int GameSessionModel::GetTableSize()
{
    return _tableSize;
}
/*
int GameSessionModel::GetTileState(int posX, int posY)
{
    if (isValidPosition(posX,posY))
    {
        return _gameTable[posX][posY]->State;
    }
    else return ErrorState;
}
*/

bool GameSessionModel::IsGameWon()
{
    if(_BluePlayer->Health==0)
    {
        if(_RedPlayer->Health==0)
            emit GameWon(None);
        else
            emit GameWon(RedPlayer);
    }
    else if(_RedPlayer->Health==0)
        emit GameWon(BluePlayer);
    else
        return false;
    return true;
}

Position GameSessionModel::GetPosition(Player player)
{
    switch(player)
    {
        case RedPlayer: return _RedPlayer->CurrentPos; break;
        case BluePlayer: return _BluePlayer->CurrentPos; break;
        default: return Position(-1,-1,0);
    }
}

bool GameSessionModel::setCommand(unsigned int commandSlot, Icon commandIndex)
{
    if (commandSlot >= 5) return false;
    RoboPig* pig;
    switch(_activePlayer)
    {
    case RedPlayer: pig = _RedPlayer; break;
    case BluePlayer: pig = _BluePlayer; break;
    default: return false;
    }
    switch(commandIndex)
    {
    case MoveUpIcon:
    case MoveDownIcon:
    case MoveLeftIcon:
    case MoveRightIcon: pig->Memory[commandSlot].Type=Move; break;
    case TurnLeftIcon:
    case TurnRightIcon: pig->Memory[commandSlot].Type=Rotate; break;
    case LaserIcon:
    case FistIcon: pig->Memory[commandSlot].Type=Attack; break;
    case SkipIcon: pig->Memory[commandSlot].Type=Skip; break;
    }
    pig->Memory[commandSlot].Value=commandIndex;
    return true;
}

void GameSessionModel::processRound()
{
    if (_activePlayer == None) return;

    _activePlayer = static_cast<Player> ((_activePlayer + 1) %3);
    emit changePlayer(_activePlayer);

}

void GameSessionModel::GameStep()
{
    if(_activePlayer!=None) return;
    if(_currentCommand==5){ endRound(); return; }
    Command redCommand = _RedPlayer->Memory[_currentCommand];
    Command blueCommand = _BluePlayer->Memory[_currentCommand];
    if(redCommand.Type==blueCommand.Type)
        executeParallelCommands();
    else if(redCommand.Type>blueCommand.Type)
    {
        executeCommand(_RedPlayer);
        executeCommand(_BluePlayer);
    }
    else
    {
        executeCommand(_BluePlayer);
        executeCommand(_RedPlayer);
    }
    _currentCommand++;
    emit MakeStep(redCommand,blueCommand);

}

void GameSessionModel::executeCommand(RoboPig* pig)
{
    Command command = pig->Memory[_currentCommand];
    RoboPig* otherPig = pig->PigName==RedPlayer ? _BluePlayer : _RedPlayer;
    if(command.Type == Move)
    {
        Position newPos = calculateStep(pig);
        if (newPos.X != otherPig->CurrentPos.X || newPos.Y != otherPig->CurrentPos.Y)
            pig->CurrentPos=newPos;
    }
    else
    switch(command.Value)
    {
        case SkipIcon :
            break;
        case TurnLeftIcon:
            pig->CurrentPos.Angle=pig->CurrentPos.Angle == 0 ? 3 : pig->CurrentPos.Angle-1;
            break;
        case TurnRightIcon:
            pig->CurrentPos.Angle=(pig->CurrentPos.Angle +1) %4;
            break;
        case LaserIcon:
            shootLaser(pig);
            break;
        case FistIcon:
            hitWithFist(pig);
            break;
        default: break;
    }
}

void GameSessionModel::executeParallelCommands()
{
    if (_RedPlayer->Memory[_currentCommand].Type != Move)
    {
        executeCommand(_RedPlayer);
        executeCommand(_BluePlayer);
    }
    else
    {
        Position newRedPos = calculateStep(_RedPlayer);
        Position newBluePos = calculateStep(_BluePlayer);
        if(newRedPos.X!=newBluePos.X || newRedPos.Y != newBluePos.Y)
        {
            executeCommand(_RedPlayer);
            executeCommand(_BluePlayer);
        }

    }

}

Position GameSessionModel::calculateStep(RoboPig *pig)
{
    Position newPos = pig->CurrentPos;
    switch(pig->Memory[_currentCommand].Value)
    {
    case MoveUpIcon:
        if (newPos.Y > 0) newPos.Y-=1;
        break;
    case MoveDownIcon:
        if (newPos.Y < _tableSize-1) newPos.Y+=1;
        break;
    case MoveRightIcon:
        if (newPos.X < _tableSize-1) newPos.X+=1;
        break;
    case MoveLeftIcon:
        if (newPos.X > 0) newPos.X-=1;
        break;
    }

    return newPos;

}

void GameSessionModel::endRound()
{
    _activePlayer=BluePlayer;
    _currentCommand=0;
    Command cleanCommand;
    cleanCommand.Type=Skip;
    cleanCommand.Value=SkipIcon;
    for(int i=0; i<5; i++)
    {
        _RedPlayer->Memory[i]=cleanCommand;
        _BluePlayer->Memory[i]=cleanCommand;
    }
    emit newRound();

}

void GameSessionModel::shootLaser(RoboPig* pig)
{
    RoboPig* otherPig = pig->PigName==RedPlayer ? _BluePlayer : _RedPlayer;
    bool isHit = false;
    if (pig->CurrentPos.X==otherPig->CurrentPos.X || pig->CurrentPos.Y==otherPig->CurrentPos.Y )
        switch(pig->CurrentPos.Angle)
        {
            case UpDirection:
                isHit = otherPig->CurrentPos.X == pig->CurrentPos.X && otherPig->CurrentPos.Y < pig->CurrentPos.Y; break;
            case DownDirection:
                isHit = otherPig->CurrentPos.X == pig->CurrentPos.X && otherPig->CurrentPos.Y > pig->CurrentPos.Y; break;
            case LeftDirection:
                isHit = otherPig->CurrentPos.Y == pig->CurrentPos.Y && otherPig->CurrentPos.X < pig->CurrentPos.X; break;
            case RightDirection:
                isHit = otherPig->CurrentPos.Y == pig->CurrentPos.Y && otherPig->CurrentPos.X > pig->CurrentPos.X; break;
        }
    if(isHit)
    {
        otherPig->Health--;
    }
    emit laserShot(pig->PigName,isHit);
}

void GameSessionModel::hitWithFist(RoboPig* pig)
{
    RoboPig* otherPig = pig->PigName==RedPlayer ? _BluePlayer : _RedPlayer;
    int hDiff = abs(pig->CurrentPos.X - otherPig->CurrentPos.X);
    int vDiff = abs(pig->CurrentPos.Y - otherPig->CurrentPos.Y);
    if (hDiff>1 || vDiff>1)
        return;
    else
    {
        otherPig->Health--;
    }


}

GameStats GameSessionModel::GetGameStats()
{
    return GameStats(_activePlayer,_RedPlayer->Health,_BluePlayer->Health,_currentCommand);
}

RoboPig::RoboPig(Player name, int PosX, int PosY, int Angle, int Health) :PigName(name)
{
    Position pos(PosX,PosY,Angle);
    this->CurrentPos = pos;
    this->Health = Health;
    for (int i=0; i<5; i++)
    {
        Memory[i].Type=Skip;
        Memory[i].Value=SkipIcon;
    }
}
