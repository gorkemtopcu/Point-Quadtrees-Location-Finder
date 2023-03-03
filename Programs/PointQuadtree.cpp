#include "PointQuadtree.h"
#include <iostream>
#include <cmath>
using namespace std;



/*** QUADRANT_NODE BEGIN ***/

// Constructor
QuadrantNode::QuadrantNode(const string & n = "NOT_DEFINED", const int & x_coord = 0, const int & y_coord = 0, QuadrantNode * norW = NULL, QuadrantNode * norE = NULL, QuadrantNode * souW = NULL, QuadrantNode * souE = NULL):name(n), x(x_coord), y(y_coord), nw(norW), ne(norE), sw(souW), se(souE){}

/*** QUADRANT_NODE END ***/



/*** POINT_QUADTREE BEGIN ***/

#pragma region Public Functions
string visitedNodes, inRangeNodes; 
// Constructor
PointQuadtree::PointQuadtree(const string & notFound = "<None>"):ITEM_NOT_FOUND(notFound), root(NULL){}

// Destructor
PointQuadtree::~PointQuadtree()
{
	makeEmpty();
}

void PointQuadtree::insert(const string & name, const int & x, const int & y)
{
	insert(name, x, y, root);
}

void PointQuadtree::search_location(const int & x, const int & y, const int & radius)
{
	search_location(x, y, radius, root);
}

void PointQuadtree::makeEmpty()
{
	makeEmpty(root);
}

void PointQuadtree::pretty_print()
{
	pretty_print(root);
}

void PointQuadtree::print_inRangeNodes()
{
	if(inRangeNodes == "") { cout << "<None>" << endl; }
	else
	{
		inRangeNodes = inRangeNodes.substr(0, inRangeNodes.length()-2);
		cout << inRangeNodes << endl;
	}
	inRangeNodes = "";
}

void PointQuadtree::print_visitedNodes()
{
	visitedNodes = visitedNodes.substr(0, visitedNodes.length()-2);
	cout << visitedNodes << endl;
	visitedNodes = "";
}

#pragma endregion Public Functions


#pragma region Private Functions

QuadrantNode *& PointQuadtree::compare(const int & x, const int & y,QuadrantNode * parentNode) const
{
	if(x < parentNode->x)
	{
		return y < parentNode->y ? parentNode->sw:parentNode->nw;
	}
	else
	{
		return y < parentNode->y ? parentNode->se:parentNode->ne;
	}
}

void PointQuadtree::insert(const string & name, const int & x, const int & y, QuadrantNode *& myNode)
{
	//TODO: later check same values also
	if(myNode == NULL)
	{
		myNode = new QuadrantNode(name, x, y, NULL,NULL,NULL,NULL);
	}
	else
	{
		insert(name, x, y, compare(x,y, myNode));
	}
}

void PointQuadtree::makeEmpty(QuadrantNode *& myNode) const
{
	if(myNode != NULL)
	{
		makeEmpty(myNode->nw);
		makeEmpty(myNode->ne);
		makeEmpty(myNode->sw);
		makeEmpty(myNode->se);
		delete myNode;
	}
	myNode = NULL;
}

void PointQuadtree::pretty_print(QuadrantNode * myNode) const
{
	if (myNode != NULL)
	{
		cout << myNode->name << endl;
		pretty_print(myNode->se);
		pretty_print(myNode->sw);
		pretty_print(myNode->ne);
		pretty_print(myNode->nw);

	}
}

void PointQuadtree::search_location(const int & x, const int & y, const int & radius, QuadrantNode * parentNode) const
{
	if(parentNode != NULL)
	{
		visitedNodes += parentNode->name + ", ";
		// location is in boundaries
		if (calculate_distance(x,y, parentNode->x, parentNode->y) <= radius)
		{
			inRangeNodes += parentNode->name + ", ";
			search_location(x,y,radius, parentNode->se);
			search_location(x,y,radius, parentNode->sw);
			search_location(x,y,radius, parentNode->ne);
			search_location(x,y,radius, parentNode->nw);
		}
		else //location is outside of boundaries
		{
			if(parentNode->y > y + radius) // over upper boundary
			{
				if(parentNode -> x <= x - radius) // on and left handside of left vertical boundary
				{
					search_location(x,y,radius,parentNode -> se);
				}
				else if(parentNode -> x <= x + radius) // between vertical boundaries and on right boundary
				{
					search_location(x,y,radius, parentNode -> se);
					search_location(x,y,radius, parentNode -> sw);
				}
				else // right handside of right vertical boundary
				{
					search_location(x,y,radius,parentNode -> sw);
				}
			}
			else if(parentNode->y == y + radius) // on upper horizontal boundary
			{
				if (parentNode->x <= x-radius) // left handside and left corner
				{
					search_location(x,y,radius,parentNode->se);
					search_location(x,y,radius,parentNode->ne);
				}
				else if(parentNode->x < x) // middle to left corner(not included)
				{
					search_location(x,y,radius,parentNode->se);
					search_location(x,y,radius,parentNode->sw);
					search_location(x,y,radius,parentNode->ne);
				}
				else if(parentNode->x <= x-radius) // middle to right corner(included)
				{
					search_location(x,y,radius,parentNode->se);
					search_location(x,y,radius,parentNode->sw);
					search_location(x,y,radius,parentNode->nw);
				}
				else // right handside
				{
					search_location(x,y,radius,parentNode->sw);
					search_location(x,y,radius,parentNode->nw);
				}
			}
			else if(parentNode->y > y - radius && parentNode->x <= x-radius) // between horizontal boundaries left handside
			{
				search_location(x,y,radius,parentNode->se);
				search_location(x,y,radius,parentNode->ne);
			}
			else if(parentNode->y > y && parentNode->x == x+radius) // on right vertical boundary middle upper part
			{
				search_location(x,y,radius,parentNode->se);
				search_location(x,y,radius,parentNode->sw);
				search_location(x,y,radius,parentNode->nw);
			}
			else if(parentNode->y > y-radius && parentNode->x == x+radius) // on right vertical boundary middle lower part
			{
				search_location(x,y,radius,parentNode->sw);
				search_location(x,y,radius,parentNode->nw);
				search_location(x,y,radius,parentNode->ne);
			}
			else if(parentNode->y > y - radius && parentNode->x > x+radius) // between horizontal boundaries right handside
			{
				search_location(x,y,radius,parentNode->sw);
				search_location(x,y,radius,parentNode->nw);
			}
			else if(parentNode->y == y-radius) // on lower horizontal boundary
			{
				if(parentNode->x <= x-radius) // left handside and left corner(included)
				{
					search_location(x,y,radius,parentNode->ne);
				}
				else if(parentNode->x <= x+radius) // right(included) to left corner(not included)
				{
					search_location(x,y,radius,parentNode->ne);
					search_location(x,y,radius,parentNode->nw);
				}
				else // right handside
				{
					search_location(x,y,radius,parentNode->nw);
				}
			}
			else if(parentNode->y < y-radius) // below lower boundary
			{
				if (parentNode->x <= x-radius) // left handside(included)
				{
					search_location(x,y,radius,parentNode->ne);
				}
				else if(parentNode->x <= x+radius) // between vertical boundaries and right(included) boundary
				{
					search_location(x,y,radius,parentNode->ne);
					search_location(x,y,radius,parentNode->nw);
				}
				else // right handside
				{
					search_location(x,y,radius,parentNode->nw);
				}
			}
			else // inner square but not in range
			{
				if (parentNode->y > y) // upper
				{
					if(parentNode->x < x) // left
					{
						search_location(x,y,radius,parentNode->se);
						search_location(x,y,radius,parentNode->sw);
						search_location(x,y,radius,parentNode->ne);
					}
					else // right
					{
						search_location(x,y,radius,parentNode->se);
						search_location(x,y,radius,parentNode->sw);
						search_location(x,y,radius,parentNode->nw);
					}
				}
				else // lower
				{
					if(parentNode->x < x) // left
					{
						search_location(x,y,radius,parentNode->se);
						search_location(x,y,radius,parentNode->ne);
						search_location(x,y,radius,parentNode->nw);
					}
					else // right
					{
						search_location(x,y,radius,parentNode->sw);
						search_location(x,y,radius,parentNode->ne);
						search_location(x,y,radius,parentNode->nw);
					}
				}
			}
		}
	}
}

double PointQuadtree::calculate_distance(const int & x0, const int & y0, const int & x1, const int & y1) const
{
	return pow(pow(x0-x1, 2) + pow(y0-y1, 2), .5);
}

#pragma endregion Private Functions

/*** POINT_QUADTREE END ***/