#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

const float TableWidth = 1000.0f;
const float TableHeight = 550.0f;
const float TableBorder = 40.0f;
const float BallRadius = 18.0f;
const float MinVelocity = 0.99f;

const float WindowWidth = TableWidth + TableBorder * 2;
const float WindowHeight = TableHeight + TableBorder * 2;
const float BackWidth = WindowWidth + 300.0f;
const float BackHeight = WindowHeight + 200.0f;
const float OFFSET_X = (BackWidth - WindowWidth) / 2;
const float OFFSET_Y = (BackHeight - WindowHeight) / 2;

#endif