#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mainboard.h"

#include <QCoreApplication>
#include <QLabel>
#include <QLCDNumber>
#include <QPushButton>
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent), board(new TetrisBoard){
    //ui->setupUi(this);

    scoreLCD = new QLCDNumber(5);
    scoreLCD->setSegmentStyle(QLCDNumber::Filled);

    levelLCD = new QLCDNumber(2);
    levelLCD->setSegmentStyle(QLCDNumber::Filled);

     linesLCD = new QLCDNumber(5);
     linesLCD->setSegmentStyle(QLCDNumber::Filled);

     nextPieceLabel = new QLabel;
         nextPieceLabel->setFrameStyle(QFrame::Box | QFrame::Raised);
         nextPieceLabel->setAlignment(Qt::AlignCenter);
         board->setNextPieceLabel(nextPieceLabel);

    startButton = new QPushButton(tr("&Start"));
    startButton->setFocusPolicy(Qt::NoFocus);

    quitButton = new QPushButton(tr("&Quit"));
    quitButton->setFocusPolicy(Qt::NoFocus);

    pauseButton = new QPushButton(tr("&Pause"));
    pauseButton->setFocusPolicy(Qt::NoFocus);

    connect(startButton, &QPushButton::clicked, board, &TetrisBoard::start);
    connect(pauseButton, &QPushButton::clicked, board, &TetrisBoard::pause);
    connect(quitButton , &QPushButton::clicked, qApp, &QCoreApplication::quit);

//    connect(board, &TetrisBoard::scoreChanged, scoreLCD, qOverload<int>(&QLCDNumber::display));
    connect(board, &TetrisBoard::scoreChanged, scoreLCD, QOverload<int>::of(&QLCDNumber::display));
    connect(board, &TetrisBoard::levelChanged, levelLCD, QOverload<int>::of(&QLCDNumber::display));
        connect(board, &TetrisBoard::linesRemovedChanged, linesLCD, QOverload<int>::of(&QLCDNumber::display));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(board, 0, 1, 6, 1);
//    scoreLCD->setStyleSheet("background-image: url(:/background/tetris-image.jpg);");
//    board->setStyleSheet("background-image: url(:/board-background/tetris_mainboard.jpg) no-repeat center center fixed;");

    this->setStyleSheet("background: rgb(14, 207, 207)");
    board->setStyleSheet("background-color: rgb(140, 225, 225)");
    startButton->setStyleSheet("background-color: light gray");
    pauseButton->setStyleSheet("background-color: light gray");
    quitButton->setStyleSheet("background-color: light gray");

    layout->addWidget(new QLabel(tr("NEXT")), 0, 0);
    layout->addWidget(nextPieceLabel, 1, 0);
    layout->addWidget(new QLabel(tr("LEVEL")), 2, 0);
    layout->addWidget(levelLCD, 3, 0);
    layout->addWidget(new QLabel(tr("SCORE")), 0, 2);
    layout->addWidget(scoreLCD, 1, 2);
    layout->addWidget(new QLabel(tr("LINES")), 2, 2);
    layout->addWidget(linesLCD, 3, 2);
    layout->addWidget(startButton, 4, 0);
    layout->addWidget(quitButton, 4, 2);
    layout->addWidget(pauseButton, 5, 2);

    scoreLCD->setStyleSheet("background-color: rgb(140, 225, 225); color: white;");
    levelLCD->setStyleSheet("background-color: rgb(140, 225, 225); color: white;");
    linesLCD->setStyleSheet("background-color: rgb(140, 225, 225); color: white;");
    nextPieceLabel->setStyleSheet("background-color: rgb(140, 225, 225); color: white;");

    setLayout(layout);

    setWindowTitle(tr("TETRIS"));
    resize(550, 370);
}

//MainWindow::~MainWindow()
//{
//    delete ui;
//}

