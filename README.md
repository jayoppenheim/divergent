# divergent
Coding test questions for Divergent3D

The question1 and question2 contain solutions for the problems as stated below.
The README.md file is each directory has specific information for each question.


Offline Coding Test
Please respond to the below questions to the best of your ability and ask questions as needed to our technical team.

It is expected to take 4 hours to complete and submit. If you need more time let us know, if you run out of time please explain what you would do to complete the exercise. Bonus, describe any limitations of your solution and next steps you would take to extend the robustness and/or functionality.

Question 1:
Write a function that takes as input a string of parenthesis, with the rest of the equation stripped out, and returns true if the parenthesis are balanced, and false otherwise.

Example balanced equation “(()())” – return true
Example unbalanced equation “())())” – return false
Also consider for all pathological edge cases.

Question 2:
Pseudocode in c++ style is fine

Given a set of elements in mesh_1 with attribute x, what is the most efficient way to map attribute x onto mesh_2 based on element position assuming:

mesh_1 is tetrahedral or hexahedral.
mesh_2 is either tetrahedral or hexahedral but has different geometry and topology from mesh_1.
The degree of overlap between mesh_1 and mesh_2 can range from no overlap at all to complete overlap.
For example, map the white elements in the hex mesh of Figure 1 onto the tet mesh of Figure 2:
