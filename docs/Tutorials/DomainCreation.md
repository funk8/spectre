\cond NEVER
Distributed under the MIT License.
See LICENSE.txt for details.
\endcond
# %Domain Creation {#tutorial_domain_creation}

### Domain Decomposition
The spatial domain on which the solution is computed is divided into
non-overlapping cells. For 2D and 3D domains, we use curvilinear
quadrilateral and curvilinear hexahedral elements, respectively. The
specification of a particular spatial domain is given by the DomainCreator.
DomainCreators specify the properties of the initial elements (before AMR takes
place), which are then used by Domain to create Blocks. Each Block
holds a CoordinateMap (described below) as well as information about its
neighbors. For geometrical domains, (e.g. rectilinear, spherical) there are a
few shortcuts that can be used such that a user does not need to specify by
hand the map and neighbor information for each %Block; these methods are
explained below.

## CoordinateMaps
Each Block in the Domain must hold a CoordinateMap which describes how to
map the logical cube (a reference cell that covers the interval \f$[-1, 1]\f$
in each dimension) to the curvilinear hexahedral element the %Block describes.
The %CoordinateMap also provides the jacobian of the mapping.

### Shortcuts for CoordinateMaps
For spherical domains, `Wedge<3>` implements the cubed-sphere map, and there
exists the method `sph_wedge_coordinate_maps` in Domain/DomainHelpers.hpp to
quickly construct six of these maps at once. For rectilinear multicube domains,
Affine and Equiangular map the logical cubes to cartesian cubes. The method
`maps_for_rectilinear_domains` in DomainHelpers allows the user to obtain
the CoordinateMaps for all of the Blocks in such a domain at once. These
can both be used to provide the map arguments to the Domain constructor.

## Boundary Information
Each Block must know which of the other Blocks in the Domain are its
neighbors and which of its logical directions points to an external boundary.

### Shortcuts for Boundary Information
A quick way to encode the neighbor and boundary information for the blocks
in a domain is through our convention of numbering and ordering the corners
of the blocks in a well-defined way. The corner numbering scheme is described
in the \ref tutorial_orientations tutorial. For spherical domains,
DomainHelpers has the methods `corners_for_radially_layered_domains` and
`corners_for_biradially_layered_domains`, which provides the proper corner
numberings for the maps obtained from the method `sph_wedge_coordinate_maps`.
These methods are used in the \ref domain::creators::Shell "Shell" and
\ref domain::creators::Sphere "Sphere" DomainCreators.
For rectilinear multicube domains, DomainHelpers has the methods
`corners_for_rectilinear_domains`, which provides the proper corner
numberings for the maps obtained from the method
`maps_for_rectilinear_domains`.

## Creating Rectilinear Domains with Shortcuts:
The construction of a rectilinear domain begins with the specification of
the total extents of the domain in each cartesian direction, in terms of
the number of blocks. These extents are held in an Index object.
For an illustrative example, we will explain how to construct a cubical
domain which has an extent of two blocks in each dimension:

\image html eightcubes.png "A 2x2x2 domain."

The first step is to generate the corner numbering for this Domain. For
the purposes of this example, we will construct all blocks with their
logical directions aligned with one another. As each block must also have
an associated block id, we must be aware of the order in which the corners
for each block are constructed.

The algorithm `corners_for_rectilinear_domains` always begins with the block
located in the lowest cartesian corner of the domain. The second block is
its immediate neighbor in the \f$+x\f$ direction, and so on until the block
in this row with the highest \f$x\f$ coordinate is reached. The next block is
the the immediate neighbor of the lowest corner block in the \f$+y\f$
direction, and then continues through the neighboring blocks in the \f$+x\f$ as
before. This is the same order in which the global corners numbers are assigned
to the vertices of the blocks.

\image html eightcubes_numbered.png "With global corners."

The block corners generated by `corners_for_rectilinear_domains` are then:

```
{0,1,3,4,9,10,12,13},
{1,2,4,5,10,11,13,14},
{3,4,6,7,12,13,15,16},
{4,5,7,8,13,14,16,17},
{9,10,12,13,18,19,21,22},
{10,11,13,14,19,20,22,23},
{12,13,15,16,21,22,24,25},
{13,14,16,17,22,23,25,26}

```

What remains is to specify the CoordinateMaps that each Block will hold.
This is handled by `maps_for_rectilinear_domains` and currently supports
both Affine and Equiangular mappings. The coordinate extents of each map
is set by the argument to `block_demarcations`. For a 2x2x1 domain, the
call to `maps_for_rectilinear_domains` could be:

\snippet Test_DomainHelpers.cpp show_maps_for_rectilinear_domains

For this choice of arguments we obtain the maps for a domain that extends
from 0.0 to 2.0 in the \f$x\f$-direction, from 0.0 to 2.0 in the \f$y\f$
-direction, and from -0.4 to 0.3 in the \f$z\f$ direction.

With the corners and maps in hand, we can pass these as arguments to the
Domain constructor.

### Non-trivial Domain Example
The aforementioned functions can also take an additional argument to exclude
blocks from a domain. For this one needs to know the Index<Dim> for each block
they wish to exclude from the domain. For the 2x2x2 domain example, the block
indices are:

```
{0,0,0},
{1,0,0},
{0,1,0},
{1,1,0},
{0,0,1},
{1,0,1},
{0,1,1},
{1,1,1}

```

In this example, we construct a domain with topology \f$S^3\f$, which begins
with the net for a tesseract. We begin by specifying that the domain extents
are three blocks along the \f$x\f$ and \f$y\f$ directions, and four blocks
along the \f$z\f$ direction. In addition, we exclude the following block
indices:

```
{0,0,0}, //first block
{1,0,0}, //second block
{2,0,0}, //third block
{0,1,0}, //fourth block
{2,1,0}, //sixth block
{0,2,0}, //seventh block
{1,2,0}, //eight block
{2,2,0}, //ninth block

{0,0,1}, //10th block
{1,0,1}, //11th block
{2,0,1}, //12th block
{0,1,1}, //13th block
{2,1,1}, //15th block
{0,2,1}, //16th block
{1,2,1}, //17th block
{2,2,1}, //18th block

{0,0,2}, //19th block
{2,0,2}, //21st block
{0,2,2}, //25th block
{2,2,2}, //27th block

{0,0,2}, //28rd block
{1,0,2}, //29th block
{2,0,2}, //30th block
{0,1,2}, //31th block
{2,1,2}, //33th block
{0,2,2}, //34th block
{1,2,2}, //35th block
{2,2,2}, //36th block

```
Alternatively, we can exclude none of the blocks in this way and instead
selectively copy the block corners from the returned vector into a new vector
that only contains the corners for the desired blocks, if one prefers to work
with single-number array indices as opposed to tuples.

Either way, we end up with a vector of block corners corresponding to the
following diagram:

\image html tesseract_numbered.png "A numbered tesseract net."

The maps are obtained similarly. In this case we suggest using the Equiangular
maps for this Domain since they adapt the logical coordinates to the angular
coordinates on a sphere. We also need to identify corresponding faces with each
other to obtain a domain of topology \f$S^3\f$, so we also need to supply the
constructor of Domain with PairsOfFaces. For the \f$S^3\f$ domain, these faces
are:

```

//Folding highest cube downward:
{{53,54,69,70},{53,54,49,50}},
{{53,57,69,73},{53,57,52,56}},
{{57,58,61,62},{57,58,73,74}},
{{54,58,70,74},{54,58,55,59}},

//Folding cross cubes together:
{{54,55,38,39},{54,50,38,34}},
{{58,59,42,43},{58,62,42,46}},
{{53,52,37,36},{53,49,37,33}},
{{57,56,41,40},{57,61,41,45}},

//Folding second lowest cube upward:
{{38,42,39,43},{38,42,22,26}},
{{33,34,37,38},{21,22,37,38}},
{{36,37,40,41},{21,37,25,41}},
{{41,42,45,46},{41,42,25,26}},

//Folding bottom cube around domain:
{{33,34,49,50},{21,22,5,6}},
{{39,43,55,59},{22,26,6,10}},
{{45,46,61,62},{25,26,9,10}},
{{36,40,52,56},{21,25,5,9}},
{{5,6,9,10},{69,70,73,74}}

```
