#pragma once
struct BoundingBox
{
    double x1, y1, x2, y2;

    BoundingBox()
    {
        x1 = 10000000;
        x2 = -1000000;
        y1 = 10000000;
        y2 = -1000000;

    }
    void bounds(double x, double y)
    {
        x1 = std::min(x, x1);
        x2 = std::max(x, x2);

        y1 = std::min(y, y1);
        y2 = std::max(y, y2);
    };
};

BoundingBox box;

struct Brush
{
    Brush(double w) : width(w)
    {
    }
    double width;
};

struct Unknown
{
     
};

struct Circle
{
    Circle(double d) : d(d)
    {
    }
    double d;
};

struct Rect
{
    Rect(double w, double h) : width(w), height(h)
    {
    }

    double width, height;

};

struct RoundRect
{
    RoundRect(double w, double h, double r) : width(w), height(h), radius(r)
    {
    }

    double width, height, radius;

};

struct RoundDonut
{
    RoundDonut(double od, double id ) : od(od), id(id) 
    {
    }

    double od, id; 

};

struct Oval
{
    Oval(double w, double h) : width(w), height(h)
    {
    }
    double width, height;
};