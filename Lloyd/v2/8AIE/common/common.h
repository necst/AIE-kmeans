#pragma once
#include "constants.h"

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
    
    float x_accum;
    float y_accum;

    Cluster(float x, float y)
    {
        this->x = x;
        this->y = y;
        this->numPoints = 1;
        this->x_accum = 0;
        this->y_accum = 0;
    }

    Cluster()
    {
        this->x = 0;
        this->y = 0;
        this->numPoints = 0;
        this->x_accum = 0;
        this->y_accum = 0;
    }

    void addPoint(Point point)
    {
        this->x_accum += point.x;
        this->y_accum += point.y;
        this->numPoints++;
    }
};