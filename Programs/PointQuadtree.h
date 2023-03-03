#ifndef POINTQUADTREE_H
#define POINTQUADTREE_H
#include <string>
using namespace std;

class QuadrantNode
{
public:
	QuadrantNode();
	QuadrantNode(const string & n, const int & x_coord, const int & y_coord, QuadrantNode * norW, QuadrantNode * norE, QuadrantNode * souW, QuadrantNode * souE); 
	friend class PointQuadtree;

private:
	int x;
	int y;
	string name;
	QuadrantNode * nw;
	QuadrantNode * ne;
	QuadrantNode * sw;
	QuadrantNode * se;
};

class PointQuadtree
{
public:
	PointQuadtree();
	PointQuadtree(const string & notFound);
	~PointQuadtree();
	void insert(const string & name, const int & x, const int & y);
	void search_location(const int & x, const int & y, const int & radius);
	void print_visitedNodes();
	void print_inRangeNodes();
	void makeEmpty();
	void pretty_print();
private:
	QuadrantNode * root;
	const string ITEM_NOT_FOUND;
	QuadrantNode *& compare(const int & x, const int & y, QuadrantNode * parentNode) const;
	void insert(const string & name, const int & x, const int & y, QuadrantNode *& myNode);
	void search_location(const int & x, const int & y, const int & radius, QuadrantNode * myNode) const;
	void decide_direction() const;
	double calculate_distance(const int & x0, const int & y0, const int & x1, const int & y1) const;
	void makeEmpty(QuadrantNode *& myNode) const;
	void pretty_print(QuadrantNode * myNode) const;
};
#endif
