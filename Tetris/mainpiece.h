#ifndef MAINPIECE_H
#define MAINPIECE_H


enum shapes {empty_shape, z_shape, s_shape, line_shape, t_shape, square_shape,
                   l_shape, alt_l_shape};

class Piece {

private:
shapes pieceShape;
int coords[4][2];

void setX(int index, int x) { coords[index][0] = x; }
void setY(int index, int y) { coords[index][1] = y; }

public:
Piece() { setShape(empty_shape); }
void setShape(shapes shapes);
void setRandomShape();


int minX() const;
int maxX() const;
int minY() const;
int maxY() const;
int x(int index) const { return coords[index][0]; }
int y(int index) const { return coords[index][1]; }

Piece rotatedLeft() const;
Piece rotatedRight() const;
shapes shape() const { return pieceShape; }

};

#endif // MAINPIECE_H
