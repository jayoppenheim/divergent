This program takes as input a string of parenthesis, with the rest of the equation stripped out, and returns true if the parenthesis are balanced, and false otherwise.

Example balanced equation “(()())” – return true
Example unbalanced equation “())())” – return false

To build the program:
g++ eq_balance.cpp

To run the program:
./a.out

Since the program only counts open and close parenthesis, a full equation may be input as well. All characters besides parenthesis are ignored.

Parsing from left to right, any time the number of closed parenthesis exceeds open parenthesis, the equation is determined to be unbalanced. At the end if the number of closed parenthesis exceeds open, the equation is also unbalanced.

Regarding edge cases, there may be unbalanced scenarios with exponents or trigonometric functions or natural logs, but it is difficult to infer those scenarios without the context of operators and operands.
