// Markus Buchholz

#include <iostream>
#include <vector>
#include <tuple>
#include <random>
#include <math.h>
#include <map>
#include <algorithm>

//--------------------------------------------------------------------------------

float xmin = 0.05;
float xmax = 2.0;
float ymin = 0.25;
float ymax = 1.3;
float zmin = 2.0;
float zmax = 15.0;

//--------------------------------------------------------------------------------

int HARMONY = 100;
int EVOLUTIONS = 50000;
float HMCR = 0.95;
float PAR = 0.95;
float BW = 0.2;
int DIM = 2;
float A = 2.0;
float R = 0.045;

//--------------------------------------------------------------------------------

struct Pos
{

    float x;
    float y;
    float z;
};

//--------------------------------------------------------------------------------

float generateRandom()
{

    std::random_device engine;
    std::mt19937 generator(engine());
    std::uniform_real_distribution<float> distrib(0.0, 1.0);
    return distrib(generator);
}

//--------------------------------------------------------------------------------
float generateRandom_05()
{

    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_real_distribution<float> distribution(-0.5, 0.5);
    return distribution(generator);
}

//--------------------------------------------------------------------------------

float valueGenerator(float low, float high)
{

    return low + generateRandom() * (high - low);
}

//--------------------------------------------------------------------------------

Pos positionUpdateCheck(Pos actualPos)
{

    Pos Pnew = actualPos;

    if (Pnew.x < xmin)
    {
        Pnew.x = xmin;
    }

    if (Pnew.x > xmax)
    {
        Pnew.x = xmax;
    }

    if (Pnew.y < ymin)
    {
        Pnew.y = ymin;
    }

    if (Pnew.y > ymax)
    {
        Pnew.y = ymax;
    }

    if (Pnew.z < zmin)
    {
        Pnew.z = zmin;
    }

    if (Pnew.z > zmax)
    {
        Pnew.z = zmax;
    }

    return Pnew;
}
//--------------------------------------------------------------------------------

std::vector<Pos> initPosXY()
{

    std::vector<Pos> pos;

    for (int ii = 0; ii < HARMONY; ii++)
    {

        pos.push_back({valueGenerator(xmin, xmax), valueGenerator(ymin, ymax), valueGenerator(zmin, zmax)});
    }

    return pos;
}

//--------------------------------------------------------------------------------

std::vector<float> function(std::vector<Pos> pos)
{
    std::vector<float> funcValue;

    for (auto &ii : pos)
    {
        float p1{0}, p2{0}, p3{0}, p4{0};
        auto f1 = 1 - (float)(std::pow(ii.y, 2) * ii.z) / (float)(71785.0 * std::pow(ii.x, 4));

        auto f2 = (float)(4.0 * std::pow(ii.y, 2) - ii.x * ii.y) / (float)(12556.0 * (ii.y * std::pow(ii.x, 3) - std::pow(ii.x, 4))) + 1.0 / (float)(5108.0 * std::pow(ii.x, 2));

        auto f3 = 1.0 - (float)(140.45 * ii.x) / (float)(std::pow(ii.y, 2) * ii.z);

        auto f4 = ((float)(ii.x + ii.y) / (float)1.5) - 1.0;

        if (f1 > 0)
        {
            p1 = std::pow(f1, 2);
        }
        if (f2 > 0)
        {
            p2 = std::pow(f2, 2);
        }
        if (f3 > 0)
        {
            p3 = std::pow(f3, 2);
        }

        if (f4 > 0)
        {
            p4 = std::pow(f4, 2);
        }

        auto f = (ii.z + 2) * ii.y * std::pow(ii.x, 2) + R * p1 + R * p2 + R * p3 + R * p4;

        funcValue.push_back(f);
    }

    return funcValue;
}

//--------------------------------------------------------------------------------

float func(Pos pos)
{
    float p1{0}, p2{0}, p3{0}, p4{0};
    auto f1 = 1 - (float)(std::pow(pos.y, 2) * pos.z) / (float)(71785.0 * std::pow(pos.x, 4));

    auto f2 = (float)(4.0 * std::pow(pos.y, 2) - pos.x * pos.y) / (float)(12556.0 * (pos.y * std::pow(pos.x, 3) - std::pow(pos.x, 4))) + 1.0 / (float)(5108.0 * std::pow(pos.x, 2));

    auto f3 = 1.0 - (float)(140.45 * pos.x) / (float)(std::pow(pos.y, 2) * pos.z);

    auto f4 = ((float)(pos.x + pos.y) / (float)1.5) - 1.0;

    if (f1 > 0)
    {
        p1 = std::pow(f1, 2);
    }
    if (f2 > 0)
    {
        p2 = std::pow(f2, 2);
    }
    if (f3 > 0)
    {
        p3 = std::pow(f3, 2);
    }

    if (f4 > 0)
    {
        p4 = std::pow(f4, 2);
    }

    return (pos.z + 2) * pos.y * std::pow(pos.x, 2) + R * p1 + R * p2 + R * p3 + R * p4;
}

//--------------------------------------------------------------------------------

int chooseWorstHarmony(std::vector<Pos> pos, std::vector<float> func)
{
    int index;
    float max = 0.0;

    for (int ii = 0; ii < func.size(); ii++)
    {

        if (func[ii] > max)
        {
            max = func[ii];
            index = ii;
        }
    }
    return index;
}
//--------------------------------------------------------------------------------

int chooseHarmony()
{

    std::random_device engine;
    std::mt19937 mt(engine());
    std::uniform_int_distribution<int> distribution(0, HARMONY);

    return distribution(mt);
}
//--------------------------------------------------------------------------------

Pos improvisation(std::vector<Pos> positions)
{
    Pos newPos;

    if (generateRandom() <= HMCR)
    {

        newPos = positions[chooseHarmony()];

        if (generateRandom() < PAR)
        {

            // pitch adjustments
            newPos.x = newPos.x + BW * (generateRandom_05()) * std::abs(xmax - xmin);
            newPos.y = newPos.y + BW * (generateRandom_05()) * std::abs(ymax - ymin);
            newPos.z = newPos.z + BW * (generateRandom_05()) * std::abs(zmax - zmin);
        }
        return positionUpdateCheck(newPos);
    }

    return positionUpdateCheck({valueGenerator(xmin, xmax), valueGenerator(ymin, ymax), valueGenerator(zmin, zmax)});
}

//--------------------------------------------------------------------------------

void runHSA()
{

    std::vector<Pos> currentPos = initPosXY();
    std::vector<float> currentfuncValue = function(currentPos);

    for (int jj = 0; jj < EVOLUTIONS; jj++)
    {
        auto index = chooseWorstHarmony(currentPos, currentfuncValue);
        auto newPos = improvisation(currentPos);
        float newValueFunc = func(newPos);

        if (newValueFunc < currentfuncValue[index])
        {

            currentPos[index] = newPos;
            currentfuncValue[index] = newValueFunc;
        }
    }

    auto fmin = [&]()
    {
        float max = std::numeric_limits<int>::max();
        int index;
        for (int ii = 0; ii < currentfuncValue.size(); ii++)
        {

            if (currentfuncValue[ii] < max)
            {
                max = currentfuncValue[ii];
                index = ii;
            }
        }
        return index;
    };

    std::cout << "-------Optimization problem: tension/compression spring----------\n";
    std::cout << "min weight= " << currentfuncValue[fmin()] << "\n";
    std::cout << "values   d= " << currentPos[fmin()].x << " D= " << currentPos[fmin()].y << " N= " << currentPos[fmin()].z << "\n";
}

//-------------------------------------------------------------------------------

int main()
{

    runHSA();
}
