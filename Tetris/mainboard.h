#ifndef MAINBOARD_H
#define MAINBOARD_H
#include <QFrame>
#include <QLabel>
#include <QPointer>
#include <QBasicTimer>
#include "mainpiece.h"

//We can use QFrame to draw a frame within our widget
//We can use this .h file to handle the in-game logic
class TetrisBoard : public QFrame
{
    Q_OBJECT

public:
    TetrisBoard(QWidget *parent = nullptr);
//    void setNextPieceLabel(QLabel *label);
//    QSize sizeHint() const override;
//    QSize minimumSizeHint() const override;

public slots:
    void start();
    void pause();

signals:
//    void scoreChanged(int score);
//    void levelChanged(int level);
//    void linesRemovedChanged(int numLines);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
//    void timerEvent(QTimerEvent *event) override;

private:
    enum { BoardWidth = 10, BoardHeight = 22 };

    shapes &shapeAt(int x, int y) { return board[(y * BoardWidth) + x]; }
//    int timeoutTime() { return 1000 / (1 + level); }
    int squareWidth() { return contentsRect().width() / BoardWidth; }
    int squareHeight() { return contentsRect().height() / BoardHeight; }
    void clearBoard();
    void dropDown();
    void oneLineDown();
    void pieceDropped(int dropHeight);
    void removeFullLines();
    void newPiece();
    void showNextPiece();
    bool tryMove(const Piece &newPiece, int newX, int newY);
    void drawSquare(QPainter &painter, int x, int y, shapes shape);

    QBasicTimer timer;
    QPointer<QLabel> nextPieceLabel;
    bool isWaitingAfterLine;
    Piece curPiece;
    Piece nextPiece;
    int curX;
    int curY;
    int numLinesRemoved;
    int numPiecesDropped;
    shapes board[BoardWidth * BoardHeight];
    bool isStarted;
    bool isPaused;
//    QBasicTimer timer;
    int score;
    int level;
};




#endif // MAINBOARD_H
