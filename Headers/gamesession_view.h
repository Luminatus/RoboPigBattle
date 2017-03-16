#ifndef GAMESESSION_VIEW_H
#define GAMESESSION_VIEW_H
#include <QWidget>
#include "ui_gamesession.h"
#include "Headers/gamesession_model.h"
#include "Headers/viewdata.h"

class GameSessionView : public QWidget
{
    Q_OBJECT
public:
    GameSessionView(QWidget* mainWindowView,GameSessionModel* gameSessionModel);
    ~GameSessionView();
private:
    void updateWindow();
    void setupIcons();
    void setActiveWidget(Player p);

    QWidget* _mainWindowView;
    GameSessionModel* _gameSessionModel;
    Ui::GameSessionUI* ui;
    ViewData* _viewData;
    QTimer* _stepTimer;
    QFont* _activeFont;
    QFont* _inactiveFont;
    QVector<QIcon*> _iconVector;
    QPushButton* _activeButton;
    QWidget* _activeWidget;
    QLayout* _activeLayout;
    int _activeIndex;
    void setupCommandButtons();
    void setActiveButton(int index);

signals:
    void saveGame(int,int,int,Position,Position);

public slots:
    void backToMainMenu();
    void makeStep(Command,Command);
    void gameWon(int player);
    void buttonClicked();
    void clickedCommand();
    void changePlayer(Player);
    void finishRound();
};

#endif // GAMESESSION_VIEW_H
