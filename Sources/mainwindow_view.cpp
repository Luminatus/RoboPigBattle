#include "Headers/mainwindow_view.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>

MainWindowView::MainWindowView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow),
    mainWindowModel(new MainWindowModel)
{
    ui->setupUi(this);
    ui->SizePicker->addItem("4x4");
    ui->SizePicker->addItem("6x6");
    ui->SizePicker->addItem("8x8");
    ui->SizePicker->setFocus();

    connect(ui->quitButton,SIGNAL(clicked()),QApplication::instance(),SLOT(quit()));
    connect(ui->InfoButton,SIGNAL(clicked()),this,SLOT(OpenFile()));
    connect(ui->NewGameButton,SIGNAL(clicked()),this,SLOT(newGameButtonClicked()));
}

void MainWindowView::exitGame()
{
    delete ui;
    delete mainWindowModel;
    QApplication::instance()->quit();
}

void MainWindowView::newGameButtonClicked()
{

    connect(mainWindowModel,SIGNAL(InitFailed()),this,SLOT(gameInitFailed()));
    connect(mainWindowModel,SIGNAL(GameReady(GameSessionModel*)),this,SLOT(initializeGameView(GameSessionModel*)));
    int index = ui->SizePicker->currentIndex();
    ui->NewGameButton->setEnabled(false);
    ui->NewGameButton->setStyleSheet(ui->NewGameButton->styleSheet());

    switch(index)
    {
     case 0 : mainWindowModel->InitGame(Four); break;
     case 1 : mainWindowModel->InitGame(Six); break;
     case 2 : mainWindowModel->InitGame(Eight); break;
    }
}

void MainWindowView::gameInitFailed()
{
    QMessageBox* message = new QMessageBox;
    message->setText("Init failed!");
    message->show();
    ui->NewGameButton->setEnabled(true);
    ui->NewGameButton->setStyleSheet(ui->NewGameButton->styleSheet());
    disconnect(mainWindowModel,SIGNAL(InitFailed()),this,SLOT(gameInitFailed()));
    disconnect(mainWindowModel,SIGNAL(GameReady(GameSessionModel*)),this,SLOT(initializeGameView(GameSessionModel*)));
}

void MainWindowView::initializeGameView(GameSessionModel* gameSession)
{
    disconnect(mainWindowModel,SIGNAL(InitFailed()),this,SLOT(gameInitFailed()));
    disconnect(mainWindowModel,SIGNAL(GameReady(GameSessionModel*)),this,SLOT(initializeGameView(GameSessionModel*)));
    gameSessionView=new GameSessionView(this,gameSession);
    connect(gameSessionView,SIGNAL(destroyed()),this,SLOT(returnToMain()));
    connect(gameSessionView,SIGNAL(saveGame(int,int,int,Position,Position)),this,SLOT(saveGame(int,int,int,Position,Position)));
    close();
}

void MainWindowView::returnToMain()
{
    disconnect(gameSessionView,SIGNAL(destroyed()),this,SLOT(returnToMain()));
    ui->NewGameButton->setEnabled(true);
    ui->NewGameButton->setStyleSheet(ui->NewGameButton->styleSheet());
    ui->SizePicker->setCurrentIndex(0);
    ui->SizePicker->setFocus();
    show();
}

void MainWindowView::OpenFile()
{
  /*  QFileDialog dialog;
    dialog.setFileMode(QFileDialog::ExistingFile);

    if(!dialog.exec())
        return;
*/
    QFile file("savegame.data");
    bool isValid = true;
    if (file.exists())
    {
        file.open(QIODevice::ReadOnly|QIODevice::Text);
        QTextStream stream(&file);
        int tableSize = 0;
        int RedHP = 0;
        int BlueHP = 0;
        Position BluePos;
        Position RedPos;
        QString temp;
        temp = stream.readLine();
        tableSize = temp.toInt();
        temp = stream.readLine();
        RedHP = temp.toInt();
        temp = stream.readLine();
        BlueHP = temp.toInt();
        temp = stream.readLine();
        QStringList arr = temp.split(",");
        if (arr.length()==3)
        {
            BluePos.X=arr[0].toInt();
            BluePos.Y=arr[1].toInt();
            BluePos.Angle=(Direction)arr[2].toInt();
        }
        else
            isValid=false;

        temp = stream.readLine();
        arr = temp.split(",");
        if (arr.length()==3)
        {
            RedPos.X=arr[0].toInt();
            RedPos.Y=arr[1].toInt();
            RedPos.Angle=(Direction)arr[2].toInt();
        }
            else isValid = false;

        connect(mainWindowModel,SIGNAL(GameReady(GameSessionModel*)),this,SLOT(initializeGameView(GameSessionModel*)));


       if(!isValid || !mainWindowModel->loadGame(tableSize,RedHP,BlueHP,BluePos,RedPos))
       {
           QMessageBox* box = new QMessageBox();
           box->setText("Hibás mentés!");
           box->show();
           disconnect(mainWindowModel,SIGNAL(GameReady(GameSessionModel*)),this,SLOT(initializeGameView(GameSessionModel*)));
       }
    }
    else
    {
        QMessageBox* box = new QMessageBox();
        box->setText("Nincs elmentett játék!");
        box->show();
    }
}


void MainWindowView::saveGame(int tableSize, int redHP, int blueHP, Position redPos, Position bluePos)
{
    QFile file("savegame.data");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(&file);
    stream<<tableSize<<"\n";
    stream<<redHP<<"\n";
    stream<<blueHP<<"\n";
    stream<<redPos.X<<","<<redPos.Y<<","<<redPos.Angle<<"\n";
    stream<<bluePos.X<<","<<bluePos.Y<<","<<bluePos.Angle<<"\n";
    file.close();
}
