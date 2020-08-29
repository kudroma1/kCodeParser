// kCodeParser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "PackageParser.hpp"

#include <iostream>
#include <fstream>
#include <string>

#include <boost/algorithm/string.hpp>

using namespace kudroma::code_assistant;

const std::string test_package_filename = "P:/kCodeParser/resources/sample_package_structure_1";

int main()
{
    PackageParser parser;
    parser.parseFromFile(test_package_filename);

    std::fstream file, out;
    out.open("out.txt", std::ios_base::out);
    file.open("D:/tmp/choose.csv", std::ios_base::in);
    std::string line;
    std::vector<std::string> parts;
    std::map<std::string, std::vector<std::string> > voices;
    if (file.is_open())
    {
        while (!file.eof())
        {
            std::getline(file, line);
            boost::split(parts, line, boost::is_any_of(","));
            int k = 0;
            const auto fio = parts[parts.size() - 3];
            boost::split(parts, parts[parts.size() - 2], boost::is_any_of(";"));
            for (const auto& voice : parts)
            {
                if (voices.find(voice) != voices.cend())
                    voices[voice].push_back(fio);
                else
                {
                    voices[voice] = std::vector<std::string>();
                    voices[voice].push_back(fio);
                }
            }
        }

        for (const auto& voice : voices)
        {
            out << voice.first << " ";
            for (const auto& v : voice.second)
                out << v << ",";
            out << std::endl;
        }

        file.close();
        out.close();
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file