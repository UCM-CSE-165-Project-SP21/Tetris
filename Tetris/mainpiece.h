#ifndef MAINPIECE_H
#define MAINPIECE_H

/* block shapes names */
enum shapes {empty_shape, z_shape, s_shape, line_shape, t_shape, square_shape,
                   l_shape, alt_l_shape};

class Piece {

public:

Piece() { setShape(empty_shape); } //set first shape to empty shape
shapes pieceShape; //variable used to keep track of the random shaped
int coords[4][2]; //dimension of the pieces

//setters
void setShape(shapes shapes);
void setRandomShape();
void setX(int index, int x) { coords[index][0] = x; }
void setY(int index, int y) { coords[index][1] = y; }

//getters to call from mainboard
int getMinX() const;
int getMaxX() const;
int getMinY() const;
int getMaxY() const;
int getX(int index) const { return coords[index][0]; }
int getY(int index) const { return coords[index][1]; }

//control the direction of the blocks called from key event
Piece rotateLeft() const;
Piece rotateRight() const;
shapes shape() const { return pieceShape; }

};

#endif // MAINPIECE_H
