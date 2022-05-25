#include "figure.h"

void Figure::delete_adjacent(Figure *f) {
    removeOffset();
    f->removeOffset();
    // Finding min distance.
    int min = 4;
    int min_i = 0;
    for (size_t i = 0; i < points_xy.size(); i += 2) {
        int dist = std::abs(f->points_xy[0] - points_xy[i]);
        dist += std::abs(f->points_xy[1] - points_xy[i + 1]);
        //  for second part.
        i += 2;
        dist += std::abs(f->points_xy[0] - points_xy[i]);
        dist += std::abs(f->points_xy[1] - points_xy[i + 1]);

        if (dist < min) {
            min_i = i - 2;
            min = dist;
        }
    }
    points_xy.erase(points_xy.begin() + min_i, points_xy.begin() + min_i + 4);
    returnOffset();
}

void Figure::add_adjacent(Figure *f) {
    //  if domino vertical.
    if (points_xy[0] == points_xy[2]) {
        //  Inserting the same x.
        points_xy.push_back(points_xy[0]);
        //  Inserting new y.
        points_xy.push_back(points_xy[points_xy.size() - 2] + 1);
        // if domino is horizontal.
    } else {
        if (f->points_xy[0] <= points_xy[0]) {
            points_xy.insert(points_xy.begin(), points_xy[1]);
            points_xy.insert(points_xy.begin(), points_xy[1] - 1);
        } else {
            points_xy.push_back(points_xy[points_xy.size() - 2] + 1);
            points_xy.push_back(points_xy[1]);
        }
    }
}

Figure::Figure() {
    type = FigureTypes(rand() % options.size());
    points_xy = options[type];
    offsetx = 4;
    offsety = -2;
    color = colors[rand() % colors.size()];
}

Figure::Figure(std::vector<int> &&coordinates, QColor color) {
    points_xy = coordinates;
    this->color = color;
    offsetx = 0;
    offsety = 0;
}

void Figure::rotateClockWise() {
    for (size_t i = 2; i < points_xy.size(); i += 2) {
        int temp_x = points_xy[i];
        points_xy[i] = points_xy[i + 1] * (-1);
        points_xy[i + 1] = temp_x;
    }
}

void Figure::rotateCounterClock() {
    for (size_t i = 2; i < points_xy.size(); i += 2) {
        int temp_x = points_xy[i] * (-1);
        points_xy[i] = points_xy[i + 1];
        points_xy[i + 1] = temp_x;
    }
}

void Figure::moveDown() {
    ++offsety;
}

void Figure::collapse(std::vector<int> &&cubes_indexes, std::vector <Figure> *cubes) {
    if (cubes_indexes.size() == 2) {
        //  two dominoes merge together.
        if (type == FigureTypes::SMALL) {
            add_adjacent(&cubes->at(cubes_indexes[0]));
            add_adjacent(&cubes->at(cubes_indexes[1]));
            //  Tricky way to append in the front.
            offsety -= 2;
            type = FigureTypes::I;
        } else {
            points_xy.clear();
        }
        //  Cubes stop to exist like cubes.
        cubes->erase(cubes->begin() + cubes_indexes[1]);
        cubes->erase(cubes->begin() + cubes_indexes[0]);
        //  If we just ran into a cube.
    } else {
        if (type == FigureTypes::SMALL) {
            points_xy.clear();
            //  Cube stops to exist like cubes.
            cubes->erase(cubes->begin() + cubes_indexes[0]);
        } else if (type != FigureTypes::T) {
            delete_adjacent(&cubes->at(cubes_indexes[0]));
            //delete_adjacent(&cubes->at(cubes_indexes[0]));
            type = FigureTypes::SMALL;
            cubes->erase(cubes->begin() + cubes_indexes[0]);
        }
    }
}

void Figure::removeOffset() {
    if (offsetx || offsety) {
        for (size_t i = 0; i < points_xy.size(); i += 2) {
            points_xy[i] += offsetx;
            points_xy[i + 1] += offsety;
        }
        offsetx = offsety = 0;
    }
}

void Figure::returnOffset() {
    offsetx = points_xy[0];
    offsety = points_xy[1];
    for (size_t i = 0; i < points_xy.size(); i += 2) {
        points_xy[i] -= offsetx;
        points_xy[i + 1] -= offsety;
    }
}
