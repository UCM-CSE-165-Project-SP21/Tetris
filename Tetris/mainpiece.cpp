#include "mainpiece.h"
#include <QtCore>

void Piece::setShape(shapes shape){
    /*describes the shapes of the blocks using coordinates */
    static constexpr int table[8][4][2]{
        { { 0, 0 },   { 0, 0 },   { 0, 0 },   { 0, 0 } },
        { { 0, -1 },  { 0, 0 },   { -1, 0 },  { -1, 1 } },
        { { 0, -1 },  { 0, 0 },   { 1, 0 },   { 1, 1 } },
        { { 0, -1 },  { 0, 0 },   { 0, 1 },   { 0, 2 } },
        { { -1, 0 },  { 0, 0 },   { 1, 0 },   { 0, 1 } },
        { { 0, 0 },   { 1, 0 },   { 0, 1 },   { 1, 1 } },
        { { -1, -1 }, { 0, -1 },  { 0, 0 },   { 0, 1 } },
        { { 1, -1 },  { 0, -1 },  { 0, 0 },   { 0, 1 } }
    };

    for (int i = 0; i < 4 ; i++) {
            for (int j = 0; j < 2; ++j)
                coords[i][j] = table[shape][i][j];
        }

    pieceShape = shape;

}

void Piece::setRandomShape(){
    setShape(shapes(QRandomGenerator::global()->bounded(7) + 1)); //return random number from 1-7
}

int Piece::getMinX() const
{
    //returns the coordinate to the lower block in a piece
    int min = coords[0][0];
    for (int i = 1; i < 4; ++i)
        min = qMin(min, coords[i][0]);
    return min;
}

int Piece::getMaxX() const
{
    //retunr the coordinate to the highest block in a piece
    int max = coords[0][0];
    for (int i = 1; i < 4; ++i)
        max = qMax(max, coords[i][0]);
    return max;
}

int Piece::getMinY() const
{
    int min = coords[0][1];
    for (int i = 1; i < 4; ++i)
        min = qMin(min, coords[i][1]);
    return min;
}

int Piece::getMaxY() const
{
    int max = coords[0][1];
    for (int i = 1; i < 4; ++i)
        max = qMax(max, coords[i][1]);
    return max;
}

Piece Piece::rotateLeft() const
{
    if (pieceShape == square_shape)
        return *this;

    Piece result;
    result.pieceShape = pieceShape;
    for (int i = 0; i < 4; ++i) {
        result.setX(i, getY(i));
        result.setY(i, -getX(i));
    }

    return result;
}

Piece Piece::rotateRight() const
{
    if (pieceShape == square_shape)
        return *this;

    Piece result;
    result.pieceShape = pieceShape;
    for (int i = 0; i < 4; ++i) {
        result.setX(i, -getY(i));
        result.setY(i, getX(i));
    }

    return result;
}
