// kCodeParser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "mainparser.hpp"

#include <iostream>
#include <fstream>
#include <string>

#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

using namespace kudroma::code_parser;

const std::string test_package_filename = "D:/Projects/kCodeParser/resources/sample_package_structure_1";

int main()
{
    MainParser parser;
    parser.parseProjectTree(test_package_filename, "D:/Projects/kCodeParser/resources/result");
    const auto res = parser.buildProjectTree();

    return 0;
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
