#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include "PointQuadtree.h"
using namespace std;

bool checkCityCoordination(const int & x, const int & y, const int & x_limit, const int & y_limit)
{
	if(x > x_limit || x < 0 || y > y_limit || y < 0) return false;
	return true;
}

int main()
{
	const string cityFileName = "cities.txt", queryFileName = "queries.txt";
	bool isCityFirstLine = true;
	ifstream cityFile, queryFile;
	string s, name;
	int x, y, radius;
	int limitX, limitY;

	cityFile.open(cityFileName.c_str());
	PointQuadtree myTree = PointQuadtree("<None>");
	
	while(getline(cityFile, s))
	{
		istringstream ssLine(s);
		if(isCityFirstLine)
		{
			ssLine >> limitX >> limitY;
			isCityFirstLine = false;
		}
		else
		{
			ssLine >> name >> x >> y;
			if(checkCityCoordination(x,y, limitX, limitY))
			{
				myTree.insert(name, x, y);
			}
		}
	}

	myTree.pretty_print();

	cout << endl;

	queryFile.open(queryFileName.c_str());
	string value;
	int valueArr[3];
	while (getline(queryFile, s))
	{
		istringstream ssLine(s);
		int index = 0;
		while(getline(ssLine, value, ','))
		{
			valueArr[index] = stoi(value);
			index++;
		}
		x = valueArr[0];
		y = valueArr[1];
		radius = valueArr[2];
		// get output operations here
		myTree.search_location(x,y,radius);
		myTree.print_inRangeNodes();
		myTree.print_visitedNodes();
		cout << endl;
	}

	return 0;
}