/*
* BSD 3-Clause License

Copyright (c) 2023, Markus Leitz

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/
#pragma once
namespace pcb2svg
{

// Feature symbol names ,e.g   $3 rect13.78x15.748
static const std::regex F(
    R"(^\$(\d+)+\s+(r|rect|oval|donut_r)(-?\d+\.?\d*)x?(-?\d+\.?\d*)?x?(r|c)?(-?\d+\.?\d*)?x?(\d{0,4}))");

// define Pad,e.g  P 2.753818897638 0.214251968504 0 P 0 1 ;2=0;ID=535539
static const std::regex P(R"(^P\s+(-?\d+\.?\d*)\s+(-?\d+\.?\d*)\s+(\d+)\s+(P|N)\s+(\d)\s+(\d)\s*;?(.*?)?;?(ID=\d*)?)");

//  define Line , e.g. L 2.915354330709 1.33271015748 2.915354330709 1.27125984252 2 P 0 ;;ID=535566
static const std::regex L(
    R"(^L\s+(-?\d+\.?\d*)\s+(-?\d+\.?\d*)\s+(-?\d+\.?\d*)\s+(-?\d+\.?\d*)\s+(\d+)\s+(P|N)\s+(\d)\s+;?(.*?)?;?ID=(\d*))");

//  define Arc, e.g. A 2.96062992126 1.22842519685 2.915354330709 1.273700787402 2.96062988189 1.273700748031 2 P 0 Y
//  ;;ID=535565
static const std::regex A(
    R"(^A\s+(-?\d+\.?\d*)\s+(-?\d+\.?\d*)\s+(-?\d+\.?\d*)\s+(-?\d+\.?\d*)\s+(-?\d+\.?\d*)\s+(-?\d+\.?\d*)\s+(\d+)\s+(P|N)\s+(\d)\s+(N|Y)?\s+;?(.*?);?ID=(\d*))");

// OB (begin) line, e.g. OB 0.12631496063 0.956692913386 I
static const std::regex OB(R"(^OB\s+(-?\d+\.?\d*)\s+(-?\d+\.?\d*)\s+(I|H)?.*)");

// OS (segment) lines,e.g. OS 0.039700787402 0.956692913386
static const std::regex OS(R"(^OS\s+(-?\d+\.?\d*)\s+(-?\d+\.?\d*)\s*(N|Y)?.*)");

// OC(arc) lines, e.g. OC 2.711614173228 1.443898622047 2.711614173228 1.409448818898 Y
static const std::regex OC(R"(^OC\s+(-?\d+\.?\d*)\s+(-?\d+\.?\d*)\s+(-?\d+\.?\d*)\s+(-?\d+\.?\d*)\s*(N|Y)?)");

// OE (end) line
static const std::regex OE(R"(^OE)");

//  surface, e.g S P 0 ;;ID=535669
static const std::regex S(R"(^S\s+(P|N)\s+(\d)\s+;?(.*?);?ID=(\d*).*)");

// end surface
static const std::regex SE(R"(^SE)");

} // namespace pcb2svg
