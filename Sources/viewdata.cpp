#include "Headers/viewdata.h"


ViewData::ViewData(GameSessionModel *model) : _gameSessionModel(model), tableSize(model->GetTableSize()){}

void ViewData::RefreshPositions()
{
    BluePosition = _gameSessionModel->GetPosition(BluePlayer);
    RedPosition = _gameSessionModel->GetPosition(RedPlayer);
}
