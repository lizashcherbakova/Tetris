#ifndef FIGURE_H
#define FIGURE_H

#include <QColor>
#include <vector>

static std::vector<std::vector<int>> options = { {0,0,1,0,2,0,3,0},  // I
                                                 {0,0,1,0,0,1,0,2},  // L
                                                 {0,0, 1,0, 1,1,2,0}, // T
                                                 {0,0,0,1,1,0,1,1},   // O
                                                 {0,0,0,1,1,1,1,2},   // S
                                               };
static std::vector < QColor > colors = {
  Qt::GlobalColor::blue,
  Qt::GlobalColor::cyan,
  Qt::GlobalColor::darkBlue,
  Qt::GlobalColor::darkCyan,
  Qt::GlobalColor::darkMagenta,
  Qt::GlobalColor::darkRed,
  Qt::GlobalColor::darkGreen,
  Qt::GlobalColor::red,
  QColor(255, 165, 0),
  QColor(255, 219, 88),
  QColor(51, 204, 55),
  QColor(0, 191, 255)
};

enum FigureTypes {
    I = 0,
    L,
    T,
    O,
    S,
    SMALL
};

class Figure {

private:
    std::vector<int> points_xy;
    void delete_adjacent(Figure *f);
    void add_adjacent(Figure *f);

public:
    FigureTypes type;
    QColor color;
    int offsetx, offsety;
    Figure();
    Figure(std::vector<int> &&coordinates, QColor color);
    inline std::vector<int> *getCoordinates() {
        return &points_xy;
    }
    void rotateClockWise();
    void rotateCounterClock();
    //  Move down after checking.
    void moveDown();
    void collapse(std::vector<int> &&cubes_indexes, std::vector <Figure> *cubes);
    void removeOffset();
    void returnOffset();
};

#endif // FIGURE_H
