#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <sstream>
#include <fstream>

// Rounds given value to specified interval - UNUSED
void roundThisToInterval(
    double & roundThis,
    double interval
)
{
    roundThis = round(roundThis / interval) * interval;
}

double degToRad(
    double degree
)
{
    return (degree * M_PI / 180);
}

// Turns the pitch, yaw, and roll that you input, into mat3 values
std::vector<double> angle_to_mat3(
    double pitch,
    double yaw,
    double roll
)
{
    double sy = sin(degToRad(yaw));
    double cy = cos(degToRad(yaw));
    double sp = sin(degToRad(pitch));
    double cp = cos(degToRad(pitch));
    double sr = sin(degToRad(roll));
    double cr = cos(degToRad(roll));

    double mat0X = cp * cy;
    double mat0Y = cp * sy;
    double mat0Z = -sp;
    double mat1X = sr * sp * cy + cr * -sy;
    double mat1Y = sr * sp * sy + cr * cy;
    double mat1Z = sr * cp;
    double mat2X = cr * sp * cy + -sr * sy;
    double mat2Y = cr * sp * sy + -sr * cy;
    double mat2Z = cr * cp;

    return std::vector<double>{mat0X, mat0Y, mat0Z, mat1X, mat1Y, mat1Z, mat2X, mat2Y, mat2Z};
}

// Takes the numbers that were input as a string, and turns them into doubles, then adds them to a vector
std::vector<double> stringToVector(
    std::string inputStr
)
{
    std::vector<double> vectorized;
    std::stringstream tempStream(inputStr);

    double currentStreamItem;
    while (tempStream >> currentStreamItem)
    {
        vectorized.push_back(currentStreamItem);
    }

    return vectorized;
}

// Specify a string, the item to replace, and its replacement
void replaceThisInString(
    std::string & stringThing,
    std::string replaceThis,
    std::string replacement
)
{
    size_t startPoint = stringThing.find(replaceThis);
    size_t endPoint = replaceThis.length();
    stringThing.replace(startPoint, endPoint, replacement);
}

// Takes the given text file and turns its contents to a vector
std::vector<std::string> textFileToVector(
    std::string filePath
)
{
    std::ifstream textTemplate(filePath);
    std::vector<std::string> makeVector;
    for (std::string currentLine; std::getline(textTemplate, currentLine);)
    {
        makeVector.push_back(currentLine);
    }
    return makeVector;
}

int main()
{
    // Get the template and turn it into a vector
    static const std::vector<std::string> templateVector = textFileToVector("template.txt");

    std::cout << "Angles should be input in the format pitch yaw roll, exe. 0 90 0\nThe fourth value from where is yaw, the fifth is pitch.\nPitch is looking up/down, yaw is looking left/right, roll is leaning left/right.\n";

    while (true)
    {
        // Get pitch yaw roll from user
        std::string* inputStr = new std::string;
        std::cout << "\nInput the pitch, yaw, and roll\n";
        std::getline(std::cin, *inputStr);
        std::cout << "\n";

        // Turn it to a vector of doubles
        std::vector<double>* pitchYawRoll = new std::vector<double>;
        *pitchYawRoll = stringToVector(*inputStr);

        // Use the pitch yaw roll to get mat3 values
        std::vector<double>* spawnOrientation = new std::vector<double>;
        *spawnOrientation = angle_to_mat3((*pitchYawRoll).at(0), (*pitchYawRoll).at(1), (*pitchYawRoll).at(2));

        // This vector will be a duplicate of the template vector we made earlier, but this one will be edited instead
        std::vector<std::string>* generatedEntity = new std::vector<std::string>;
        *generatedEntity = templateVector;

        // Replace the placeholders from the template with the real mat3 values we calculated
        // (the mat3 values need to be converted to strings for the replacement to work)
        replaceThisInString((*generatedEntity).at(0), "PITCH", std::to_string((*pitchYawRoll).at(0)));
        replaceThisInString((*generatedEntity).at(0), "YAW", std::to_string((*pitchYawRoll).at(1)));
        replaceThisInString((*generatedEntity).at(0), "ROLL", std::to_string((*pitchYawRoll).at(2)));
        replaceThisInString((*generatedEntity).at(3), "MAT0X", std::to_string((*spawnOrientation).at(0)));
        replaceThisInString((*generatedEntity).at(4), "MAT0Y", std::to_string((*spawnOrientation).at(1)));
        replaceThisInString((*generatedEntity).at(5), "MAT0Z", std::to_string((*spawnOrientation).at(2)));
        replaceThisInString((*generatedEntity).at(8), "MAT1X", std::to_string((*spawnOrientation).at(3)));
        replaceThisInString((*generatedEntity).at(9), "MAT1Y", std::to_string((*spawnOrientation).at(4)));
        replaceThisInString((*generatedEntity).at(10), "MAT1Z", std::to_string((*spawnOrientation).at(5)));
        replaceThisInString((*generatedEntity).at(13), "MAT2X", std::to_string((*spawnOrientation).at(6)));
        replaceThisInString((*generatedEntity).at(14), "MAT2Y", std::to_string((*spawnOrientation).at(7)));
        replaceThisInString((*generatedEntity).at(15), "MAT2Z", std::to_string((*spawnOrientation).at(8)));

        // Display the final spawnOrientation by printing every line of the vector
        for (std::string currentLine : *generatedEntity)
        {
            std::cout << currentLine << "\n";
        }

        // Delete pointer variables
        delete inputStr;
        delete pitchYawRoll;
        delete spawnOrientation;
        delete generatedEntity;
    }

    return 0;
}
