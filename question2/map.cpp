#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

/************************************************************************/
/*                                                                      */
/*   This program maps the attributes from a source mesh on to a         */
/*   destination mesh.                                                  */
/*                                                                      */
/************************************************************************/

class Point {
	public:
		double x=0,y=0,z=0;
};

class Node : public Point {
	public:
		int id;
};

class Element {
	public:
		int id;
	        Node* nodes[20];
		int nodeids[20];
		void setAttribute(int att) { attribute=att; };
		int getAttribute() { return attribute; };
		Point maxcoord,mincoord;
		Point getCentroid() { return centroid; };
		virtual bool isPointInside(Point* point) { cout << "base check\n"; return false; };
		virtual void calculateCentroid() { };
	protected:
		Point centroid;
	private:
		int attribute=0;
};

class Vector {
	public:
		double x=0,y=0,z=0;
};

bool isPointOnTop (Node** nodes, Point* point) {
	// this function returns true if a point is one top of a plane
	// | ab x ac | to find the plane normal n
	// | ac x bd | to find the plane normal n
	// ap . n positive means on top
	Vector ac, bd, ap;
	ac.x = nodes[2]->x - nodes[0]->x;
	ac.y = nodes[2]->y - nodes[0]->y;
	ac.z = nodes[2]->z - nodes[0]->z;

	bd.x = nodes[3]->x - nodes[1]->x;
	bd.y = nodes[3]->y - nodes[1]->y;
	bd.z = nodes[3]->z - nodes[1]->z;

	ap.x = point->x - nodes[0]->x;
	ap.y = point->y - nodes[0]->y;
	ap.z = point->z - nodes[0]->z;
	
	//ac x abd
	Vector ac_x_bd;
	ac_x_bd.x = ac.y * bd.z - ac.z * bd.y;
	ac_x_bd.y = ac.z * bd.x - ac.x * bd.z;
	ac_x_bd.z = ac.x * bd.y - ac.y * bd.x;
	
	//positive dot product means point is above the face, or toward the element inside
	double dotprod = ac_x_bd.x * ap.x + ac_x_bd.y * ap.y +ac_x_bd.z * ap.z;
	if (dotprod < 0) return false;

	return true;
};

class Tet: public Element {
	// an element with 4 nodes is assumed to be a tet
	public:
		Tet(Node** in_nodes) {
			// for simplicity, node id and vector index are the same
			for (int i=0; i < 4; i++) nodes[i] = in_nodes[i];
			calculateCentroid();
		}
		void calculateCentroid() override {
			// average node location, not exact centroid but close enough for now
			// TBD: replace with centroid
			// find max and min box for intermediate inside check
			centroid.x = (nodes[0]->x)/4;
			centroid.y = (nodes[0]->y)/4;
			centroid.z = (nodes[0]->z)/4;
			maxcoord.x = mincoord.x = nodes[0]->x;
			maxcoord.y = mincoord.y = nodes[0]->y;
			maxcoord.z = mincoord.z = nodes[0]->z;
			for (int i=1; i < 4; i++) {
				centroid.x += (nodes[i]->x)/4;
				centroid.y += (nodes[i]->y)/4;
				centroid.z += (nodes[i]->z)/4;
				// max and min for temporary inside calculation
				if (nodes[i]->x > maxcoord.x) maxcoord.x = nodes[i]->x;
				if (nodes[i]->y > maxcoord.y) maxcoord.y = nodes[i]->y;
				if (nodes[i]->z > maxcoord.z) maxcoord.z = nodes[i]->z;
				if (nodes[i]->x < mincoord.x) mincoord.x = nodes[i]->x;
				if (nodes[i]->y < mincoord.y) mincoord.y = nodes[i]->y;
				if (nodes[i]->z < mincoord.z) mincoord.z = nodes[i]->z;
			}
		}
		bool isPointInside(Point* point) override { 
			// fill this in with tet inclusion check
			// simple box jump out quickly
			if (point->x > maxcoord.x) return false;
			if (point->y > maxcoord.y) return false;
			if (point->z > maxcoord.z) return false;
			if (point->x < mincoord.x) return false;
			if (point->y < mincoord.y) return false;
			if (point->z < mincoord.z) return false;
			// if inside the coarse box do an exact check
			// use isPointOnTop and loop through element faces
			int facemap[4][4] = {{0,1,2,2}, {2,1,3,3}, {0,2,3,3}, {1,0,3,3}};
			for (int i=0; i < 4; i++) {
				// loop through each face
				Node* facenodes[4];
				facenodes[0] = nodes[facemap[i][0]];
				facenodes[1] = nodes[facemap[i][1]];
				facenodes[2] = nodes[facemap[i][2]];
				facenodes[3] = nodes[facemap[i][3]];
				if (!isPointOnTop(facenodes,point)) return false;
			}
			return true; 
		}
};

class Hex: public Element {
	// an element with 8 nodes is assumed to be a hex
	public:
		Hex(Node** in_nodes) {
			// for simplicity, node id and vector index are the same
			for (int i=0; i < 8; i++) nodes[i] = in_nodes[i];
			calculateCentroid();
		}
		void calculateCentroid() override {
			// average node location, not exact centroid but close enough for now
			// TBD: replace with centroid
			centroid.x = (nodes[0]->x)/8;
			centroid.y = (nodes[0]->y)/8;
			centroid.z = (nodes[0]->z)/8;
			maxcoord.x = mincoord.x = nodes[0]->x;
			maxcoord.y = mincoord.y = nodes[0]->y;
			maxcoord.z = mincoord.z = nodes[0]->z;
			for (int i=1; i < 8; i++) {
				centroid.x += (nodes[i]->x)/8;
				centroid.y += (nodes[i]->y)/8;
				centroid.z += (nodes[i]->z)/8;
				// max and min for temporary inside calculation
				if (nodes[i]->x > maxcoord.x) maxcoord.x = nodes[i]->x;
				if (nodes[i]->y > maxcoord.y) maxcoord.y = nodes[i]->y;
				if (nodes[i]->z > maxcoord.z) maxcoord.z = nodes[i]->z;
				if (nodes[i]->x < mincoord.x) mincoord.x = nodes[i]->x;
				if (nodes[i]->y < mincoord.y) mincoord.y = nodes[i]->y;
				if (nodes[i]->z < mincoord.z) mincoord.z = nodes[i]->z;
			}
			//cout << "centroid is " << centroid.x << " " << centroid.y << " " <<centroid.z << " " << endl;
		}
		bool isPointInside(Point* point) override { 
			// fill this in with hex inclusion check
			// Simple box for now
			// cout << point->x << " " << point->y << " " << point->z << endl;
			if (point->x > maxcoord.x) return false;
			if (point->y > maxcoord.y) return false;
			if (point->z > maxcoord.z) return false;
			if (point->x < mincoord.x) return false;
			if (point->y < mincoord.y) return false;
			if (point->z < mincoord.z) return false;
			// if inside the coarse box do an exact check
			// use isPointOnTop and loop through element faces
			int facemap[6][4] = {{0,1,2,3}, {1,0,4,5}, {2,1,5,6}, {3,2,6,7}, {0,3,7,4}, {7,6,5,4}};
			for (int i=0; i < 6; i++) {
				// loop through each face
				Node* facenodes[4];
				facenodes[0] = nodes[facemap[i][0]];
				facenodes[1] = nodes[facemap[i][1]];
				facenodes[2] = nodes[facemap[i][2]];
				facenodes[3] = nodes[facemap[i][3]];
				if (!isPointOnTop(facenodes,point)) return false;
			}
			return true; 
		}
};

Element* inverseIsoparametricMap(vector<Element*>* elemSource,Point* p) {
        return NULL;
}

int main () {

	vector<Node*> nodesSource, nodesDestination;
	vector<Element*> elementsSource, elementsDestination;
	bool debug = false;

	//open the source file and populate source nodes and elements, with attribute
	cout << endl << "Read the Source mesh" << endl << endl;
	//fstream file1("2brick.csv", ios::in);
	fstream file1("10tet.csv", ios::in);
	string line;
	bool createNodes = false, createElems = false, setAttributes = false;
	double dvalue[3];
	Node* nodevalue[8];
	int ivalue[8];
	while(getline(file1,line))
	{
    		stringstream   linestream(line);
    		string         value;

		if (debug) cout << line << endl;
		if (line=="Nodes") { createNodes = true; createElems = false; setAttributes = false; continue;}
		if (line=="Elements") { createNodes = false; createElems = true; setAttributes = false; continue;}
		if (line=="Attributes") { createNodes = false; createElems = false; setAttributes = true; continue;}

		int i = 0;
    		while(getline(linestream,value,','))
    		{
			if (createNodes) dvalue[i] = stod(value);
			if (createElems) nodevalue[i] = nodesSource[stoi(value)];
			if (setAttributes) ivalue[i] = stoi(value);
			i++;
			//if (debug) cout << "Value(" << value << ")\n";
		}
		if (createNodes) { 
			Node* n = new Node(); // TBD clean up later
			cout << "create node at " << dvalue[0] << " " << dvalue[1] << " " <<dvalue[2] << " " << endl;
			n->x = dvalue[0]; n->y = dvalue[1]; n->z = dvalue[2];
			nodesSource.push_back(n);
		}
		if (createElems) {
			if (i == 4) {
 				Element* e = new Tet(&nodevalue[0]); // TBD clean up later
				e->id = elementsSource.size();
				elementsSource.push_back(e);
			} else if (i == 8) {
 				Element* e = new Hex(&nodevalue[0]); // TBD clean up later
				e->id = elementsSource.size();
				elementsSource.push_back(e);			}
		}
		if (setAttributes) { 
			elementsSource[ivalue[0]]->setAttribute(ivalue[1]);
		}
		//if (debug) cout << "Line Finished" << std::endl;
	}
	
	cout << "Source mesh has " << nodesSource.size() << " nodes " << std::endl;
	cout << "Source mesh has " << elementsSource.size() << " elements " << std::endl;

	//return 0;
	createNodes = false; createElems = false; setAttributes = false;
	//open the destination file and populate destination nodes and elements
	cout << endl << "Read the Destination mesh" << endl << endl;
	fstream file2("5brick.csv", ios::in);
	while(getline(file2,line))
	{
    		stringstream   linestream(line);
    		string         value;

		//if (debug) cout << line << endl;
		if (line=="Nodes") { createNodes = true; createElems = false; setAttributes = false; continue;}
		if (line=="Elements") { createNodes = false; createElems = true; setAttributes = false; continue;}
		if (line=="Attributes") { createNodes = false; createElems = false; setAttributes = true; continue;}

		int i = 0;
    		while(getline(linestream,value,','))
    		{
			if (createNodes) dvalue[i] = stod(value);
			if (createElems) ivalue[i] = stoi(value);
			if (createElems) nodevalue[i] = nodesDestination[stoi(value)];
			if (setAttributes) ivalue[i] = stoi(value);
			i++;
			//if (debug) cout << "Value(" << value << ")\n";
		}
		//if (debug) cout << "Line Finished" << std::endl;
		if (createNodes) { 
			Node* n = new Node(); // clean up later
			cout << "create node at " << dvalue[0] << " " << dvalue[1] << " " <<dvalue[2] << " " << endl;
			n->x = dvalue[0]; n->y = dvalue[1]; n->z = dvalue[2];
			nodesDestination.push_back(n);
		}
		if (createElems) { 
			Element* e = new Hex(&nodevalue[0]); // clean up later
			e->id = elementsDestination.size();
			elementsDestination.push_back(e);
		}
	}
	cout << "Destination mesh has " << nodesDestination.size() << " nodes " << endl;
	cout << "Destination mesh has " << elementsDestination.size() << " elements " << endl;

	cout << endl << "Start MAPPING" << endl << endl;
	vector<Element*>::iterator itd, its;
	vector<Element*> elementsExclude;
	// drop elements with unassigned attributes, only consider the white source elements
	for (its = elementsSource.begin(); its != elementsSource.end(); ++its) {
  		// marked for exclusion
		if ((*its)->getAttribute()==1) elementsExclude.push_back(*its);
	}
	cout << "Exclusion mesh for stress results has " << elementsExclude.size() << " elements " << endl;

	// TBD, reorder the destination mesh by centroid coordinate
    	//for (itd = elementsDestination.begin(); itd != elementsDestination.end(); ++itd) {
	//}
	
	// TBD, create a coarse brick bounding mesh around the destination mesh
	//      Find the extreme cooardinates and divide into large bricks
	//      This will be called zoneExclude
	
	//loop through the destination mesh
    	for (itd = elementsDestination.begin(); itd != elementsDestination.end(); ++itd) {
		//loop through the exclude elements source mesh, find an element that coincides, and copy the attribute
		Point p = (*itd)->getCentroid();

		//quick check on white zone which is coarse and only needs bounding box check
    		//for (its = zoneExclude.begin(); its != zoneExclude.end(); ++its) {
		//	if ((*its)->isPointInside(&p)) {
		//		if ((*its)->getAttribute()!=1) break;
		//	}
		//}
		
    		for (its = elementsExclude.begin(); its != elementsExclude.end(); ++its) {
			// brute force loop
			// if the destination element centroid is inside the source element to exclude
			if ((*its)->isPointInside(&p)) {
				//if (debug) cout << " SET ATT " << (*its)->getAttribute() << endl;
				(*itd)->setAttribute((*its)->getAttribute());
				break;
			}
		}
		cout << "Destination element " << (*itd)->id << " assigned attribute " << (*itd)->getAttribute() << endl;
	}
	return 0;
}
