#include "mainboard.h"
#include <QKeyEvent>
#include <QLabel>
#include <QPainter>

TetrisBoard::TetrisBoard(QWidget *parent)
    : QFrame(parent), isStarted(false), isPaused(false)
{
    setFrameStyle(QFrame::Panel | QFrame::Sunken);
    setFocusPolicy(Qt::StrongFocus);

    clearBoard();

    nextPiece.setRandomShape();
}

void TetrisBoard::clearBoard()
{
    for (int i = 0; i < BoardHeight * BoardWidth; ++i)
        board[i] = empty_shape;
}

void TetrisBoard::start()
{
    if (isPaused){
        return;
    }

    isStarted = true;
    newPiece();
}

void TetrisBoard::pause()
{
    if (!isStarted){
        return;
    }

    isPaused = !isPaused;
}

void TetrisBoard::newPiece()
{
    curPiece = nextPiece;
    nextPiece.setRandomShape();
//    showNextPiece();
    curX = BoardWidth / 2 + 1;
    curY = BoardHeight - 1 + curPiece.minY();

    if (!tryMove(curPiece, curX, curY)) {
        curPiece.setShape(empty_shape);
//        timer.stop();
        isStarted = false;
    }
}

//void TetrisBoard::showNextPiece()
//{
//    if (!nextPieceLabel)
//        return;

//    int dx = nextPiece.maxX() - nextPiece.minX() + 1;
//    int dy = nextPiece.maxY() - nextPiece.minY() + 1;

//    QPixmap pixmap(dx * squareWidth(), dy * squareHeight());
//    QPainter painter(&pixmap);
//    painter.fillRect(pixmap.rect(), nextPieceLabel->palette().window());

//    for (int i = 0; i < 4; ++i) {
//        int x = nextPiece.x(i) - nextPiece.minX();
//        int y = nextPiece.y(i) - nextPiece.minY();
//        drawSquare(painter, x * squareWidth(), y * squareHeight(),
//                   nextPiece.shape());
//    }
//    nextPieceLabel->setPixmap(pixmap);
//}

void TetrisBoard::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    QPainter painter(this);
    QRect rect = contentsRect();

    if (isPaused) {
        painter.drawText(rect, Qt::AlignCenter, tr("Pause"));
        return;
    }

    int boardTop = rect.bottom() - BoardHeight*squareHeight();

    for (int i = 0; i < BoardHeight; ++i) {
        for (int j = 0; j < BoardWidth; ++j) {
            shapes shape = shapeAt(j, BoardHeight - i - 1);
            if (shape != empty_shape)
                drawSquare(painter, rect.left() + j * squareWidth(),
                           boardTop + i * squareHeight(), shape);
        }

    }

    if (curPiece.shape() != empty_shape) {
        for (int i = 0; i < 4; ++i) {
            int x = curX + curPiece.x(i);
            int y = curY - curPiece.y(i);
            drawSquare(painter, rect.left() + x * squareWidth(),
                       boardTop + (BoardHeight - y - 1) * squareHeight(),
                       curPiece.shape());
        }
    }
}

void TetrisBoard::keyPressEvent(QKeyEvent *event)
{
    if (!isStarted || isPaused || curPiece.shape() == empty_shape) {
        QFrame::keyPressEvent(event);
        return;
    }
    switch (event->key()) {
    case Qt::Key_Left:
        tryMove(curPiece, curX - 1, curY);
        break;
    case Qt::Key_Right:
        tryMove(curPiece, curX + 1, curY);
        break;
    case Qt::Key_Down:
        tryMove(curPiece.rotatedRight(), curX, curY);
        break;
    case Qt::Key_Up:
        tryMove(curPiece.rotatedLeft(), curX, curY);
        break;
//    case Qt::Key_Space:
//        dropDown();
//        break;
//    case Qt::Key_D:
//        oneLineDown();
//        break;
    default:
        QFrame::keyPressEvent(event);
    }
}

bool TetrisBoard::tryMove(const Piece &newPiece, int newX, int newY)
{
    for (int i = 0; i < 4; ++i) {
        int x = newX + newPiece.x(i);
        int y = newY - newPiece.y(i);
        if (x < 0 || x >= BoardWidth || y < 0 || y >= BoardHeight)
            return false;
        if (shapeAt(x, y) != empty_shape)
            return false;
    }
    curPiece = newPiece;
    curX = newX;
    curY = newY;
    update();
    return true;
}

void TetrisBoard::drawSquare(QPainter &painter, int x, int y, shapes shape)
{
    static constexpr QRgb colorTable[8] = {
        0x000000, 0xCC6666, 0x66CC66, 0x6666CC,
        0xCCCC66, 0xCC66CC, 0x66CCCC, 0xDAAA00
    };

    QColor color = colorTable[int(shape)];
    painter.fillRect(x + 1, y + 1, squareWidth() - 2, squareHeight() - 2,
                     color);

    painter.setPen(color.lighter());
    painter.drawLine(x, y + squareHeight() - 1, x, y);
    painter.drawLine(x, y, x + squareWidth() - 1, y);

    painter.setPen(color.darker());
    painter.drawLine(x + 1, y + squareHeight() - 1,
                     x + squareWidth() - 1, y + squareHeight() - 1);
    painter.drawLine(x + squareWidth() - 1, y + squareHeight() - 1,
                     x + squareWidth() - 1, y + 1);
}

