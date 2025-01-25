#pragma once
#include "constants.h"

typedef union my_union_u
{
    float f;
    uint32_t i;
} my_union_t;

struct Point
{
    float x;
    float y;

    Point(float x, float y)
    {
        this->x = x;
        this->y = y;
    }

    Point()
    {
        this->x = 0;
        this->y = 0;
    }
};

struct Cluster
{
    float x;
    float y;
    int32_t numPoints;

    Cluster(float x, float y)
    {
        this->x = x;
        this->y = y;
        this->numPoints = 1;
    }

    Cluster()
    {
        this->x = 0;
        this->y = 0;
        this->numPoints = 0;
    }

    void addPoint(Point point)
    {
        float x_accum = this->x * this->numPoints + point.x;
        float y_accum = this->y * this->numPoints + point.y;

        this->numPoints++;

        this->x = x_accum / this->numPoints;
        this->y = y_accum / this->numPoints;
    }
};