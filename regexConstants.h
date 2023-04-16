#pragma once
namespace  pcb2svg
{ 
static const std::regex E(R"(^\$(\d+)+\s+(r|rect|oval|donut_r)(-?\d+\.?\d*)x?(-?\d+\.?\d*)?x?(r|c)?(-?\d+\.?\d*)?x?(\d{0,4}))");

static const std::regex P(R"(^P\s+(-?\d+\.?\d*)\s+(-?\d+\.?\d*)\s+(\d)\s+(P|N)\s+(\d)\s+(\d)\s+;(.*);ID=(\d*))");
static const std::regex L(
    R"(^L\s+(-?\d+\.?\d*)\s+(-?\d+\.?\d*)\s+(-?\d+\.?\d*)\s+(-?\d+\.?\d*)\s+(\d)\s+(P|N)\s+(\d)\s+;(.*);ID=(\d*))");
static const std::regex A(R"(^A\s+(-?\d+\.?\d*)\s+(-?\d+\.?\d*)\s+(-?\d+\.?\d*)\s+(-?\d+\.?\d*)\s+(-?\d+\.?\d*)\s+(-?\d+\.?\d*)\s+(\d)\s+(P|N)\s+(\d)\s+(N|Y)?\s+;(.*);ID=(\d*))");


static const std::regex OB(R"(^OB\s+(-?\d+\.?\d*)\s+(-?\d+\.?\d*)\s+(I|H)?.*)");
static const std::regex OS(R"(^OS\s+(-?\d+\.?\d*)\s+(-?\d+\.?\d*)\s*(N|Y)?.*)");
static const std::regex OC(R"(^OC\s+(-?\d+\.?\d*)\s+(-?\d+\.?\d*)\s+(-?\d+\.?\d*)\s+(-?\d+\.?\d*)\s*(N|Y)?)");

static const std::regex OE(R"(^OE)");

static const std::regex S(R"(^S\s+(P|N)\s+(\d)\s+;(.*);ID=(\d*).*)");

static const std::regex SE(R"(^SE)");
}