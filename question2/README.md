The question:

Pseudocode in c++ style is fine

Given a set of elements in mesh_1 with attribute x, what is the most efficient way to map attribute x onto mesh_2 based on element position assuming:

mesh_1 is tetrahedral or hexahedral.
mesh_2 is either tetrahedral or hexahedral but has different geometry and topology from mesh_1.
The degree of overlap between mesh_1 and mesh_2 can range from no overlap at all to complete overlap.

To build the program:
g++ map.cpp

To run the program:
./a.out

Implementation:

- Read in mesh_1 and mesh_2 from csv files
  -- Calculate centroids
  -- Find extreme coordinates to be used as a bounding box
- Select only the mesh_1 elements with a white or exclusion attribute
- Order mesh_2 elements spatially by x,y,z coordinate
- Create octants or a coarse brick mesh around mesh_2 (TBD)
  -- Mark each octant as white or red
  -- Create a list of white elements in white octants
- Find mesh_2 elements in the mesh_1 exclusion set and mark them as excluded
  -- First, quick check if the element is in a white octant
     -- Entire chunks of elements can be skipped if they are in a red zone
  -- For each mesh_2 element, check if it is inside a marked mesh_1 element
     -- Quick check each mesh_1 element by bounding box
        This will be fast as any coordinate component outside of the min/max is an immediate false
     -- Last, confirm centroid is inside by looping through the faces and checking the point is above each plane

Alternate approach:
- This may work better if the number of white elements are not well clustered
  -- Share the above approach of selecting only mesh_1 white elements and ordering mesh_2
- For each white element in mesh one, quick search for all mesh_2 elements with centroids in the bounding box
- Confirm centroid is inside by looping through the faces and checking the point is above each plane

Assumptions:
- The attribute is constant and determined at the centroid
- All elements have a valid topology, follow a right hand rule for nodal connectivity
- All hex elements near planar surfaces, not too curved (planar inside check)

Limitations:
- Mapped element boundaries will not match source mesh boundaries exactly

Edge case:
- A destination element centroid coincides with a source node
- A destination element centroid may lie exactly on a face
- A fine source mesh compared to destination where an element marked for exclusion is missed by the destination element centroid

Extensions:
- Switch from csv to json or something more flexible
- Pass in arguments for mesh_1.csv and mesh_2.csv
- Consider degenerate shapes
- Add quadratic element types
- Concatenate nodes and elements files into one
- Switch to a library, like boost, for geometry
