// Markus Buchholz

#include <iostream>
#include <vector>
#include <tuple>
#include <random>
#include <math.h>
#include <map>
#include <algorithm>

float xmin = -5.0;
float xmax = 5.0;
float ymin = -5.0;
float ymax = 5.0;
int HARMONY = 100;
int EVOLUTIONS = 50000;
float HMCR = 0.95;
float PAR = 0.95;
float BW = 0.2;
int DIM = 2;

//--------------------------------------------------------------------------------

struct Pos
{

    float x;
    float y;
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

    return Pnew;
}
//--------------------------------------------------------------------------------

std::vector<Pos> initPosXY()
{

    std::vector<Pos> pos;

    for (int ii = 0; ii < HARMONY; ii++)
    {

        pos.push_back({valueGenerator(xmin, xmax), valueGenerator(ymin, ymax)});
    }

    return pos;
}

//--------------------------------------------------------------------------------

std::vector<float> function(std::vector<Pos> pos)
{
    std::vector<float> funcValue;

    for (auto &ii : pos)
    {

        funcValue.push_back(ii.x * ii.y);
    }

    return funcValue;
}

//--------------------------------------------------------------------------------

float func(Pos pos)
{

    return (pos.x * pos.y);
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
        }
        return positionUpdateCheck(newPos);
    }

    return positionUpdateCheck({valueGenerator(xmin, xmax), valueGenerator(ymin, ymax)});
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

    std::cout << "-------Optimization problem: quadratic function----------\n";
    std::cout << "min value = " << currentfuncValue[fmin()] << "\n";
    std::cout << "x1= " << currentPos[fmin()].x << " x2= " << currentPos[fmin()].y << "\n";
}

//-------------------------------------------------------------------------------

int main()
{

    runHSA();
}
