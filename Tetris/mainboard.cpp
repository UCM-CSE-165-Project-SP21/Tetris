#include "mainboard.h"

TetrisBoard::TetrisBoard(QWidget *parent)
    : QFrame(parent), isStarted(false), isPaused(false)
{
    setFrameStyle(QFrame::Panel | QFrame::Sunken);
    setFocusPolicy(Qt::StrongFocus);
}

void TetrisBoard::start()
{
    if (isPaused){
        return;
    }

    isStarted = true;
}

void TetrisBoard::pause()
{
    if (!isStarted){
        return;
    }

    isPaused = !isPaused;
}
