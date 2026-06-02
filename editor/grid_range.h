#ifndef GRID_RANGE_H
#define GRID_RANGE_H

#include <QPoint>
#include <iterator>

class GridRange {
private:
    QPoint origin;
    int width;
    int height;


public:
    GridRange(QPoint origin, int width, int height);

    class Iterator {
    private:
        QPoint origin;
        int curr_x;
        int curr_y;
        int width;

    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = QPoint;
        using difference_type = std::ptrdiff_t;
        using pointer = QPoint*;
        using reference = QPoint;

        Iterator(QPoint origin, int width, int curr_x, int curr_y);

        Iterator& operator++();

        QPoint operator*() const;

        bool operator!=(const Iterator& other) const;

        bool operator==(const Iterator& other) const;
    };

    Iterator begin() const;
    Iterator end() const;
};


#endif  // GRID_RANGE_H
