#include "Headers/gamesession_view.h"
#include <QPainter>
#include <QPaintEngine>
#include <QPaintEvent>
#include <QMessageBox>
#include <QTimer>

GameSessionView::GameSessionView(QWidget* mainWindowView, GameSessionModel* gameSessionModel):
    _mainWindowView(mainWindowView),
    _gameSessionModel(gameSessionModel),
    ui(new Ui::GameSessionUI),
  _activeButton(NULL),
  _activeLayout(NULL),
  _activeWidget(NULL)
{
    GameStats stats = _gameSessionModel->GetGameStats();
    _viewData = new ViewData(_gameSessionModel);
    _viewData->tileSize = 60;
  //  _activeTile = new ActiveTile();
  //  _viewData->activeTile = _activeTile;
    _viewData->activePlayer = stats.ActivePlayer;
    _viewData->commandId = stats.CommandIndex;
    _inactiveFont = new QFont("Arial",10,0,false);
    _activeFont = new QFont("Arial",11,QFont::Bold,true);

    setupIcons();

    ui->setupUi(this);
    setupCommandButtons();
    ui->gameTable->setViewData(_viewData);
    ui->BluePlayerLabel->setFont(*_activeFont);
    ui->RedPlayerLabel->setFont(*_inactiveFont);
    ui->RedPlayerPoints->display(stats.RedHealth);
    ui->BluePlayerPoints->display(stats.BlueHealth);
    int minwidth = ui->UserUI->width() > ui->gameTable->width() ? ui->UserUI->width()+20 : ui->gameTable->width()+20;
    setMinimumSize(minwidth,ui->gameTable->height()+230);
    setMaximumSize(minwidth,ui->gameTable->height()+230);


    _stepTimer = new QTimer();
    _stepTimer->setSingleShot(true);
    _stepTimer->setInterval(1000);


    for(int i = 0; i<ui->RedMemoryLayout->count();i++)
    {

        QPushButton* butt = qobject_cast<QPushButton*>( ui->RedMemoryLayout->itemAt(i)->widget());
        butt->setIcon(*_iconVector[LockedIcon]);
    }

    setActiveWidget(_viewData->activePlayer);

    connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(backToMainMenu()));
    connect(ui->confirmButton,SIGNAL(clicked(bool)),_gameSessionModel,SLOT(processRound()));
    connect(_gameSessionModel,SIGNAL(changePlayer(Player)),this,SLOT(changePlayer(Player)));
//    connect(ui->gameTable,SIGNAL(TileClicked(int,int)),this,SLOT(processClick(int,int)));
    connect(_gameSessionModel,SIGNAL(GameWon(int)),this,SLOT(gameWon(int)));
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);
show();
}

GameSessionView::~GameSessionView()
{
    disconnect(ui->closeButton,SIGNAL(clicked()),this,SLOT(backToMainMenu()));
//    disconnect(ui->gameTable,SIGNAL(TileClicked(int,int)),this,SLOT(processClick(int,int)));
    disconnect(_gameSessionModel,SIGNAL(GameWon(int)),this,SLOT(gameWon(int)));

    delete _stepTimer;
 //   delete _activeTile;
    delete _activeFont;
    delete _inactiveFont;
    delete _viewData;
    delete _gameSessionModel;
    delete ui;
}


void GameSessionView::backToMainMenu()
{
    if(!_gameSessionModel->IsGameWon())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this,"Kilépés","El akarod menteni a játékot? (A jelenlegi kör adatai elvésznek)",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            GameStats stat = _gameSessionModel->GetGameStats();
            emit saveGame(_viewData->tableSize,stat.RedHealth,stat.BlueHealth,_viewData->RedPosition,_viewData->BluePosition);
        }
    }
    close();
    delete this;
}

void GameSessionView::setupIcons()
{
    _iconVector.resize(10);
    /*
    QPixmap emptyPixmap(30,30);
    emptyPixmap.fill(Qt::transparent);
    */
    _iconVector[SkipIcon] = new QIcon();
    _iconVector[MoveUpIcon] = new QIcon(":/commands/uparrow_s.png");
    _iconVector[MoveDownIcon] = new QIcon(":/commands/downarrow_s.png");
    _iconVector[MoveRightIcon] = new QIcon(":/commands/rightarrow_s.png");
    _iconVector[MoveLeftIcon] = new QIcon(":/commands/leftarrow_s.png");
    _iconVector[TurnRightIcon] = new QIcon(":/commands/turnright_s.png");
    _iconVector[TurnLeftIcon] = new QIcon(":/commands/turnleft_s.png");
    _iconVector[LaserIcon] = new QIcon(":/commands/laser3_s.png");
    _iconVector[FistIcon] = new QIcon(":/commands/fist_s.png");
    _iconVector[LockedIcon] = new QIcon(":/commands/skip_s.png");
}


void GameSessionView::setActiveWidget(Player p)
{
    if (_activeLayout!=Q_NULLPTR)
    {
        for(int i = 0; i<_activeLayout->count();i++)
        {

            QPushButton* butt = qobject_cast<QPushButton*>( _activeLayout->itemAt(i)->widget());
            butt->setIcon(*_iconVector[LockedIcon]);
            disconnect(butt,SIGNAL(clicked(bool)),this,SLOT(buttonClicked()));
        }
    }

    switch(p)
    {
    case RedPlayer:
        _activeWidget=ui->RedMemory;
        break;
    case BluePlayer:
        _activeWidget=ui->BlueMemory;
        break;
    default:
        _activeWidget = NULL; break;

}
    if(_activeWidget!=NULL)
    {
    _activeLayout = qobject_cast<QWidget*>( _activeWidget->children().first())->layout();
    for(int i = 0; i<_activeLayout->count();i++)
    {

        QPushButton* butt = qobject_cast<QPushButton*>( _activeLayout->itemAt(i)->widget());
        butt->setIcon(*_iconVector[SkipIcon]);
        connect(butt,SIGNAL(clicked(bool)),this,SLOT(buttonClicked()));
    }
    }
    setActiveButton(0);
}

void GameSessionView::setupCommandButtons()
{
    for (int i=0; i< ui->ButtonLayout->count(); i++)
    {
        QPushButton* butt = qobject_cast<QPushButton*>(ui->ButtonLayout->itemAt(i)->widget());
        connect(butt,SIGNAL(clicked(bool)),this,SLOT(clickedCommand()));
    }
}

void GameSessionView::buttonClicked()
{
    QPushButton* butt = qobject_cast<QPushButton*> (sender());
    int buttonIndex = _activeLayout->indexOf(butt);
    setActiveButton(buttonIndex);

}

void GameSessionView::setActiveButton(int index)
{
    if(_activeButton!=Q_NULLPTR && _activeButton!=NULL)
        _activeButton->setStyleSheet("border:1px solid black;");
    _activeButton = qobject_cast<QPushButton*> (_activeLayout->itemAt(index)->widget());
    _activeButton->setStyleSheet("border:2px solid yellow;");
    _activeIndex = _activeLayout->indexOf(_activeButton);
}

void GameSessionView::clickedCommand()
{

    QPushButton* senderButton = qobject_cast<QPushButton*>(sender());
    Icon commandIndex =static_cast<Icon>( ui->ButtonLayout->indexOf(senderButton));
    if(_gameSessionModel->setCommand(_activeIndex,commandIndex))
    {
        _activeButton->setIcon(*(_iconVector[commandIndex]));
        setActiveButton((_activeIndex+1) % 5);

    };
}

void GameSessionView::changePlayer(Player player)
{
    _viewData->activePlayer = player;
    setActiveWidget(_viewData->activePlayer);
    bool isPlayerNone = _viewData->activePlayer==None;
    ui->ButtonFrame->setEnabled(!isPlayerNone);

    if(isPlayerNone)
    {
        connect(_gameSessionModel,SIGNAL(MakeStep(Command,Command)),this,SLOT(makeStep(Command,Command)));
        connect(_stepTimer,SIGNAL(timeout()),_gameSessionModel,SLOT(GameStep()));
        connect(_gameSessionModel,SIGNAL(newRound()),this,SLOT(finishRound()));
        _stepTimer->start();
    }
}

void GameSessionView::makeStep(Command redCommand, Command blueCommand)
{
 QPushButton* butt;
 butt= qobject_cast<QPushButton*>(ui->BlueMemoryLayout->itemAt(_viewData->commandId)->widget());
 butt->setIcon(*_iconVector[blueCommand.Value]);
 butt = qobject_cast<QPushButton*>(ui->RedMemoryLayout->itemAt(_viewData->commandId)->widget());
 butt->setIcon(*_iconVector[redCommand.Value]);
 _viewData->RedPosition = _gameSessionModel->GetPosition(RedPlayer);
 _viewData->RedPosition = _gameSessionModel->GetPosition(BluePlayer);

updateWindow();

if(!_gameSessionModel->IsGameWon())
_stepTimer->start();

}

void GameSessionView::updateWindow()
{
    GameStats stat = _gameSessionModel->GetGameStats();
    _viewData->commandId = stat.CommandIndex;
    ui->BluePlayerPoints->display(stat.BlueHealth);
    ui->RedPlayerPoints->display(stat.RedHealth);
    if(stat.ActivePlayer==BluePlayer)
    {
        ui->BluePlayerLabel->setFont(*_activeFont);
        ui->RedPlayerLabel->setFont(*_inactiveFont);
    }
    else if(stat.ActivePlayer==RedPlayer)
    {
        ui->RedPlayerLabel->setFont(*_activeFont);
        ui->BluePlayerLabel->setFont(*_inactiveFont);

    }
    update();
}

void GameSessionView::gameWon(int player)
{
    disconnect(_gameSessionModel,SIGNAL(GameWon(int)),this,SLOT(gameWon(int)));
    updateWindow();
    QMessageBox* message = new QMessageBox();
    if(player==None)
        message->setText("Döntetlen!");
    else{
        QString winnerColour= player==BluePlayer? "kék":"vörös";
        message->setText("A "+winnerColour+" játékos nyert!");
    }
    message->show();
    connect(message,SIGNAL(finished(int)),SLOT(backToMainMenu()));
}



void GameSessionView::finishRound()
{
disconnect(_gameSessionModel,SIGNAL(MakeStep(Command,Command)),this,SLOT(makeStep(Command,Command)));
disconnect(_stepTimer,SIGNAL(timeout()),_gameSessionModel,SLOT(GameStep()));
disconnect(_gameSessionModel,SIGNAL(newRound()),this,SLOT(finishRound()));

GameStats stat = _gameSessionModel->GetGameStats();
_viewData->activePlayer=stat.ActivePlayer;
_viewData->commandId = stat.CommandIndex;
changePlayer(_viewData->activePlayer);
}
