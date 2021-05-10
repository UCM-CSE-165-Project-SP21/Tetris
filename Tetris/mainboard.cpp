#include "mainboard.h"
#include <QKeyEvent>
#include <QLabel>
#include <QPainter>
#include <QMediaPlayer>

TetrisBoard::TetrisBoard(QWidget *parent)
    : QFrame(parent), isStarted(false), isPaused(false), isGameOver(false)
{

    /* initialied the music player and imported song*/
    backGroundMusic = new QMediaPlayer();
    backGroundMusic->setMedia(QUrl("qrc:/music/Through The Fire and Flames [8 Bit Cover Tribute to Dragonforce] - 8 Bit Universe.mp3"));
    backGroundMusic->setVolume(50);

    /* Frame style for the shape of the pieces */
    setFrameStyle(QFrame::Panel | QFrame::Sunken);
    setFocusPolicy(Qt::StrongFocus);

    clearBoard();

    nextPiece.setRandomShape();
}

void TetrisBoard::clearBoard()
{
    /* removing blocks in the main board */
    for (int i = 0; i < boardHeight * boardWidth; ++i)
        board[i] = empty_shape;
}

void TetrisBoard::setNextPieceLabel(QLabel *label)
{
    nextPieceLabel = label;
}

void TetrisBoard::startGame()
{
    if (isPaused){
        return;
    }
    isStarted = true;
    backGroundMusic->play();

    isWaitingAfterLine = false;
    numLinesRemoved = 0;
    numPiecesDropped = 0;

    score = 0;
    level = 1;
    clearBoard();

    emit linesRemovedChanged(numLinesRemoved);
    emit changeScore(score);
    emit changeLevel(level);

    /* create first piece and start timer */
    createPiece();
    timer.start(timeout(), this);
}

void TetrisBoard::pauseGame()
{
    if (!isStarted){
        return;
    }

    isPaused = !isPaused;

    if(isPaused){
        timer.stop();
        backGroundMusic->pause();
    }
    else{
        timer.start(timeout(), this);
        backGroundMusic->play();
    }
    update();
}

void TetrisBoard::GameOver()
{
    isGameOver = true;

    if(isGameOver){
        timer.stop();
        backGroundMusic->stop();

        //isGameOver = false;

    }

    update();
}


void TetrisBoard::createPiece()
{
    currentPiece = nextPiece;
    nextPiece.setRandomShape();
    showNextPiece();
    currentX = boardWidth / 2 + 1;
    currentY = boardHeight - 1 + currentPiece.getMinY();

     /* When piece reaches the top */
    if (!tryMove(currentPiece, currentX, currentY)) {
        currentPiece.setShape(empty_shape);
        timer.stop();
        isStarted = false;
        GameOver();
    }
}

void TetrisBoard::showNextPiece()
{
    if (!nextPieceLabel)
        return;

    int dx = nextPiece.getMaxX() - nextPiece.getMinX() + 1;
    int dy = nextPiece.getMaxY() - nextPiece.getMinY() + 1;

    /* triggers paint event at coordinates x and y*/
    QPixmap pixmap(dx * mainboardWidth(), dy * mainboardHeight());
    QPainter painter(&pixmap);
    painter.fillRect(pixmap.rect(), nextPieceLabel->palette().window());

    for (int i = 0; i < 4; ++i) {
        int x = nextPiece.getX(i) - nextPiece.getMinX();
        int y = nextPiece.getY(i) - nextPiece.getMinY();
        drawBlock(painter, x * mainboardWidth(), y * mainboardHeight(),
                   nextPiece.shape());
    }
    nextPieceLabel->setPixmap(pixmap);
}

void TetrisBoard::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    QPainter painter(this);
    QRect rect = contentsRect();

    if (isPaused) {
        painter.drawText(rect, Qt::AlignCenter, tr("Pause"));
        return;
    }
    if(isGameOver){
        clearBoard();
        painter.drawText(rect, Qt::AlignCenter, tr("GAME OVER"));
        //clearBoard();
        isGameOver = false;
        return;
    }

    int boardTop = rect.bottom() - boardHeight*mainboardHeight();

    /* draws the main board */
    for (int i = 0; i < boardHeight; ++i) {
        for (int j = 0; j < boardWidth; ++j) {
            shapes shape = shapePosition(j, boardHeight - i - 1);
            if (shape != empty_shape)
                drawBlock(painter, rect.left() + j * mainboardWidth(), boardTop + i * mainboardHeight(), shape);
        }

    }

    /* draws blocks */
    if (currentPiece.shape() != empty_shape) {
        for (int i = 0; i < 4; ++i) {
            int x = currentX + currentPiece.getX(i);
            int y = currentY - currentPiece.getY(i);
            drawBlock(painter, rect.left() + x * mainboardWidth(), boardTop + (boardHeight - y - 1) * mainboardHeight(), currentPiece.shape());
        }
    }
}

void TetrisBoard::keyPressEvent(QKeyEvent *event)
{
    if (!isStarted || isPaused || currentPiece.shape() == empty_shape) {
        QFrame::keyPressEvent(event);
        return;
    }
    switch (event->key()) {
    case Qt::Key_Left:
        tryMove(currentPiece, currentX - 1, currentY);
        break;
    case Qt::Key_Right:
        tryMove(currentPiece, currentX + 1, currentY);
        break;
    case Qt::Key_Down:
        tryMove(currentPiece.rotateRight(), currentX, currentY);
        break;
    case Qt::Key_Up:
        tryMove(currentPiece.rotateLeft(), currentX, currentY);
        break;
    case Qt::Key_Space:
        dropDown();
        break;
    case Qt::Key_D:
        oneLineDown();
        break;
    default:
        QFrame::keyPressEvent(event);
    }
}

void TetrisBoard::timerEvent(QTimerEvent *event)
{
    /* when pieces collide new piece is created else move one line */
    if (event->timerId() == timer.timerId()) {
        if (isWaitingAfterLine) {
            isWaitingAfterLine = false;
            createPiece();
            timer.start(timeout(), this);
        } else {
            oneLineDown();
        }
    } else {
        QFrame::timerEvent(event);
    }
}

bool TetrisBoard::tryMove(const Piece &createPiece, int newX, int newY)
{
    /* collision logic using piece coordinates and board dimensions */
    for (int i = 0; i < 4; ++i) {
        int x = newX + createPiece.getX(i);
        int y = newY - createPiece.getY(i);
        if (x < 0 || x >= boardWidth || y < 0 || y >= boardHeight)
            return false;
        if (shapePosition(x, y) != empty_shape)
            return false;
    }
    currentPiece = createPiece;
    currentX = newX;
    currentY = newY;
    update();
    return true;
}

void TetrisBoard::pieceDropped(int dropHeight)
{
    /* pieces dropped logic */
    for (int i = 0; i < 4; ++i) {
        int x = currentX + currentPiece.getX(i);
        int y = currentY - currentPiece.getY(i);
        shapePosition(x, y) = currentPiece.shape();
    }

    ++numPiecesDropped;
    if (numPiecesDropped % 25 == 0) {
        ++level;
        timer.start(timeout(), this);
        emit changeLevel(level);
    }

    score += dropHeight + 7;
    emit changeScore(score);
    removeFullLines();

    if (!isWaitingAfterLine)
        createPiece();
}

void TetrisBoard::oneLineDown()
{
    if (!tryMove(currentPiece, currentX, currentY - 1))
        pieceDropped(0);
}

void TetrisBoard::dropDown()
{
    int dropHeight = 0;
    int newY = currentY;
    while (newY > 0) {
        if (!tryMove(currentPiece, currentX, newY - 1))
            break;
        --newY;
        ++dropHeight;
    }
    pieceDropped(dropHeight);

}

void TetrisBoard::removeFullLines()
{
    int numFullLines = 0;

    /* iterate through the height of the board to check if any lines have been filled */
    for (int i = boardHeight - 1; i >= 0; --i) {
        bool lineIsFull = true;

        for (int j = 0; j < boardWidth; ++j) {
            if (shapePosition(j, i) == empty_shape) {
                lineIsFull = false;
                break;
            }
        }

        /* remove lines when they are full */

        if (lineIsFull) {
            ++numFullLines;
            for (int k = i; k < boardHeight - 1; ++k) {
                for (int j = 0; j < boardWidth; ++j)
                    shapePosition(j, k) = shapePosition(j, k + 1);
            }
            for (int j = 0; j < boardWidth; ++j)
                shapePosition(j, boardHeight - 1) = empty_shape;
        }
    }
    /* update score and lines LCD screens */
    if (numFullLines > 0) {
        numLinesRemoved += numFullLines;
        score += 10 * numFullLines;
        emit linesRemovedChanged(numLinesRemoved);
        emit changeScore(score);

        timer.start(500, this);
        isWaitingAfterLine = true;
        currentPiece.setShape(empty_shape);
        update();
    }
}

void TetrisBoard::drawBlock(QPainter &painter, int x, int y, shapes shape)
{ 
    /* set color to the blocks */
    static constexpr QRgb colorTable[8] = {
            0x000000, 0xff0000, 0x00ff00, 0x00ffff,
            0x800080, 0xffff00, 0x0000ff, 0xff7f00
        };

    /* apply color to the shape */
    QColor color = colorTable[int(shape)];
    painter.fillRect(x + 1, y + 1, mainboardWidth() - 2, mainboardHeight() - 2, color);

    painter.setPen(color.lighter());
    painter.drawLine(x, y + mainboardHeight() - 1, x, y);
    painter.drawLine(x, y, x + mainboardWidth() - 1, y);

    painter.setPen(color.darker());
    painter.drawLine(x + 1, y + mainboardHeight() - 1, x + mainboardWidth() - 1, y + mainboardHeight() - 1);
    painter.drawLine(x + mainboardWidth() - 1, y + mainboardHeight() - 1, x + mainboardWidth() - 1, y + 1);
}

