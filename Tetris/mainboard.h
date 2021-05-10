#ifndef MAINBOARD_H
#define MAINBOARD_H
#include <QFrame>
#include <QLabel>
#include <QPointer>
#include <QBasicTimer>
#include "mainpiece.h"
#include <QMediaPlayer>

//We can use QFrame to draw a frame within our widget
//We can use this .h file to handle the in-game logic
class TetrisBoard : public QFrame
{
    Q_OBJECT

protected:
    //timer implementations
    QBasicTimer timer; //used this library
    void timerEvent(QTimerEvent *event) override; //timers to make moving object
    int timeout() { return 1000 / (2 + level); } //shorter timeout when increasing level to increase difficulty

    //paint implementation (inspired from Qt Labs)
    void paintEvent(QPaintEvent *event) override; //event used to paint the blocks
    void drawBlock(QPainter &painter, int x, int y, shapes shape); //set blocks to specific RGB color
    void createPiece(); // sets piece randomly
    void showNextPiece(); //shows piece that follows from current piece
    void clearBoard(); //cleans blocks in the main board
    Piece currentPiece;
    Piece nextPiece;
    int currentX;
    int currentY;
    QPointer<QLabel> nextPieceLabel;


    //in-Game functions
    void keyPressEvent(QKeyEvent *event) override; //triggers movement of the blocks
    void dropDown(); //takes piece to the bottom
    void oneLineDown(); //drops piece one line
    void pieceDropped(int dropHeight); //current height of pieces in board
    void removeFullLines();
    bool tryMove(const Piece &newPiece, int newX, int newY); //when a key is pressed
    bool isStarted; // true when game starts
    bool isPaused;
    bool isGameOver;
    int score;
    int level;
    QMediaPlayer *backGroundMusic;
    int numLinesRemoved;
    int numPiecesDropped;
    bool isWaitingAfterLine;

    //game dimension
    enum { boardWidth = 10, boardHeight = 22 };
    shapes &shapePosition(int x, int y) { return board[(y * boardWidth) + x]; }
    int mainboardWidth() { return contentsRect().width() / boardWidth; }
    int mainboardHeight() { return contentsRect().height() / boardHeight; }
    shapes board[boardWidth * boardHeight];

public:
    TetrisBoard(QWidget *parent = nullptr);
    void setNextPieceLabel(QLabel *label);  //variable holding name of the next piece to draw

public slots:
    void startGame();
    void GameOver();
    void pauseGame();

signals:
    void changeScore(int currentScore);
    void changeLevel(int currentLevel);
    void linesRemovedChanged(int numLines);

};




#endif // MAINBOARD_H
