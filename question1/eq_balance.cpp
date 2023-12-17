#include <iostream>
using namespace std;

/******************************************************************************/
/*                                                                            */
/*   This program reads an equation with parentheses stripped out and         */
/*   outputs whether or not the equation is balanced.                         */
/*                                                                            */
/******************************************************************************/

bool balanced(string &equation) {
	int open_count = 0;
	//cout << equation << endl; // debug
	string::iterator it;
	for (it = equation.begin(); it != equation.end(); it++) {
		//cout << *it << endl; // debug
		if (*it == '(') open_count++;	
		if (*it == ')') open_count--;	
		// if at any time the count is negative, the equation is unbalanced
		if (open_count < 0) return false;
	}	
	if (open_count == 0) {
	       	return true;
	} else {
		return false;
	}
}

int main () {
	string equation;
	cout << "Enter a stripped equation, parenthesies only: \n"; cin >> equation; cout << endl;
	if (balanced(equation)) {
		cout << "Your equation is balanced.\n" << endl;
	} else {
		cout << "Your equation is unbalanced!\n" << endl;
	}
	return 0;
}
