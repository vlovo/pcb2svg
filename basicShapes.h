#pragma once
namespace pcb2svg
{

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
    RoundDonut(double od, double id) : od(od), id(id)
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
} // namespace pcb2svg
