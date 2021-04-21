#ifndef MAINBOARD_H
#define MAINBOARD_H
#include <QFrame>
#include <QBasicTimer>

//We can use QFrame to draw a frame within our widget
//We can use this .h file to handle the in-game logic
class TetrisBoard : public QFrame
{
    Q_OBJECT

public:
    TetrisBoard(QWidget *parent = nullptr);

public slots:
    void start();
    void pause();

private:
    bool isStarted;
    bool isPaused;
    QBasicTimer timer;
    int score;
    int level;
};




#endif // MAINBOARD_H
