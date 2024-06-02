#include"RTree.h"
#include<assert.h>
#include<iostream>

RTREE_TEMPLATE RTREE_QUAL::RTree()
{
	assert(MAXNODES > MINNODES);
	assert(MINNODES > 0);

	//only support machinde word size simple data type eg.integer index or object pointer
	//since we are storing as union with non data branch
	//assert(sizeof(DATATYPE) == sizeof(void*) || sizeof(DATATYPE) == sizeof(int));
	
	//precompute volumes of the unit spheres for the first few dimensions
	const float UNIT_SPHERE_VOLUMES[] = {
	  0.000000f, 2.000000f, 3.141593f, // Dimension  0,1,2
	  4.188790f, 4.934802f, 5.263789f, // Dimension  3,4,5
	  5.167713f, 4.724766f, 4.058712f, // Dimension  6,7,8
	  3.298509f, 2.550164f, 1.884104f, // Dimension  9,10,11
	  1.335263f, 0.910629f, 0.599265f, // Dimension  12,13,14
	  0.381443f, 0.235331f, 0.140981f, // Dimension  15,16,17
	  0.082146f, 0.046622f, 0.025807f, // Dimension  18,19,20
	};

	root = AllocNode();
	root->level = 0;
	unitSphereVolume = (ELEMTYPEREAL)UNIT_SPHERE_VOLUMES[NUMDIMS];
}

RTREE_TEMPLATE RTREE_QUAL::~RTree()
{
	Reset();//free or reset node memory
}

RTREE_TEMPLATE void RTREE_QUAL::Insert(const ELEMTYPE x_min[NUMDIMS], const ELEMTYPE x_max[NUMDIMS], const DATATYPE& entryId)
{
#ifdef _DEBUG
	for (int index = 0; index < NUMDIMS; ++index)
	{
		assert(x_min[index] <= x_max[index]);
	}
#endif

	Rect rect;

	for (int axis = 0; axis < NUMDIMS; ++axis)
	{
		rect.rect_min[axis] = x_min[axis];
		rect.rect_max[axis] = x_max[axis];
	}

	InsertRect(&rect, entryId, &root, 0);
}

RTREE_TEMPLATE typename RTREE_QUAL::Node* RTREE_QUAL::AllocNode()
{
	Node* newNode;
#ifdef PTREE_DONT_USE_MEMPOOLS
	newNode = new Node;
#else
//
#endif
	InitNode(newNode);
	return newNode;
}

RTREE_TEMPLATE void RTREE_QUAL::FreeNode(Node* node)
{
	assert(node);

#ifdef RTREE_DONT_USE_MEMPOOLS
	delete node;
#else
#endif
}

RTREE_TEMPLATE void RTREE_QUAL::InitNode(Node* node)
{
	node->count = 0;
	node->level = -1;
}

/// <summary>
///  
///
/// </summary>
/// <param name="rect"></param>
/// <param name="id"></param>
/// <param name="node"></param>
/// <param name="newNode"></param>
/// <param name="level"></param>
/// <returns></returns>
RTREE_TEMPLATE bool RTREE_QUAL::InsertRectR(Rect* rect, const DATATYPE& id, Node* node, Node** newNode, int level)
{
	assert(rect);
	assert(node);
	assert(newNode);
	assert(level >= 0);
	assert(level <= node->level);

	int index;
	Branch branch;
	Node* otherNode;


	if (node->level > level)	//if node doesn't point to leaf,find the best branch
	//repeat until it points to leaf
	{
		index = PickBranch(rect, node);
		if (!InsertRectR(rect, id, node->branch[index].child, &otherNode, level))
		{
			//child was not split
			node->branch[index].rect = CombineRect(rect, &(node->branch[index].rect));
			return false;
		}
		else //child was split
		{
			node->branch[index].rect = NodeCover(node->branch[index].child);
			branch.child = otherNode;
			branch.rect = NodeCover(otherNode);
			return AddBranch(&branch, node, newNode);
		}
	}
	else if (node->level == level)
	{
		branch.rect = *rect;
		branch.data = id;
		//child field of leaves contains id of data record
		return AddBranch(&branch, node, newNode);
	}
	else
	{
		//should never accur
		assert(0);
		return false;
	}
}

// Insert a data rect into and index structure
// InsertRect provides for splitting the root;
// return 1 if root was split,0 if not
// the level argument specifies the number of steps up from leaf
// InsertRect2 does recursion
RTREE_TEMPLATE bool RTREE_QUAL::InsertRect(Rect* rect, const DATATYPE& id, Node** root, int level)
{
	assert(rect);
	assert(root);
	assert(level >= 0);
	assert(level <= (*root)->level);
#ifdef _DEBUG
	for (int index = 0; index < NUMDIMS; ++index)
	{
		assert(rect.rect_min[index] <= rect.rect_max[index]);
	}
#endif //DEBUG

	Node* newRoot;
	Node* newNode;
	Branch branch;

	if (InsertRectR(rect, id, *root, &newNode, level))
	{
		newRoot = AllocNode();//root split
		newRoot->level = (*root)->level + 1;
		branch.rect = NodeCover(*root);
		branch.child = *root;
		AddBranch(&branch, newRoot, NULL);
		branch.rect = NodeCover(newNode);
		branch.child = newNode;
		AddBranch(&branch, newRoot, NULL);
		*root = newRoot;
		return true;
	}

	return false;
}

/// <summary>
/// Add a branch to a node. Split the node if necessary.
/// return 0 if node not split.Old node update
/// return 1 if node split,set *newNode to address of new node.
/// Old node update,become one of twos
/// </summary>
/// <param name="branch"></param>
/// <param name="node"></param>
/// <param name="newNode"></param>
/// <returns></returns>
RTREE_TEMPLATE bool RTREE_QUAL::AddBranch(Branch* branch, Node* node, Node** newNode)
{
	assert(branch);
	assert(node);
	
	if (node->count < MAXNODES)//Split won't be necessary
	{
		node->branch[node->count] = *branch;
		++node->count;

		return false;
	}
	else
	{
		assert(newNode);

		SplitNode(node, branch, newNode);
		return true;
	}
}

/// <summary>
/// Pick a branch.  Pick the one that will need the smallest increase
/// in area to accomodate the new rectangle.  This will result in the
/// least total area for the covering rectangles in the current node.
/// In case of a tie, pick the one which was smaller before, to get
/// the best resolution when searching.
/// </summary>
/// <param name="rect"></param>
/// <param name="node"></param>
/// <returns></returns>
RTREE_TEMPLATE int RTREE_QUAL::PickBranch(Rect* rect, Node* node)
{
	assert(rect);
	assert(node);

	bool firstTime = true;
	ELEMTYPEREAL increase;
	ELEMTYPEREAL bestIncr = (ELEMTYPEREAL)-1;
	ELEMTYPEREAL area;
	ELEMTYPEREAL bestArea;
	int best;
	Rect tempRect;

	for (int i = 0; i < node->count; ++i)
	{
		Rect* curRect = &node->branch[i].rect;
		area = CalcRectVolume(curRect);
		tempRect = CombineRect(rect, curRect);
		increase = CalcRectVolume(&tempRect) - area;
		if ((increase < bestIncr) || firstTime)
		{
			best = i;
			bestArea = area;
			bestIncr = increase;
			firstTime = false;
		}
		else if ((increase == bestIncr) && (area < bestArea))
		{
			best = i;
			bestArea = area;
			bestIncr = increase;
		}
	}
	return best;
}

/// <summary>
/// Split a node
/// Divides the nodes branches and the extra one between two nodes
/// Old node is one of the new ones,and one really new one is created
/// Tries more than one method to choose a partion,uses best result
/// </summary>
/// <param name="node"></param>
/// <param name="branch"></param>
/// <param name="newNode"></param>
RTREE_TEMPLATE void RTREE_QUAL::SplitNode(Node* node, Branch* branch, Node** newNode)
{
	assert(node);
	assert(branch);

	//could just use local here,but member or external is faster since it is reused
	PartitionVars localVars;
	PartitionVars* parVars = &localVars;
	int level;

	//Load all the branches into a buffer,initialize old node
	level = node->level;
	GetBranches(node, branch, parVars);

	//Find partition
	ChoosePartition(parVars, MINNODES);

	//Put branches from buffer into 2 nodes according to chosen partition
	*newNode = AllocNode();
	(*newNode)->level = node->level = level;
	LoadNodes(node, newNode, parVars);

	assert((node->count + (*newNode)->count) == parVars->total);
}

//the exact volume of the bounding sphere for the given Rect
RTREE_TEMPLATE ELEMTYPEREAL RTREE_QUAL::RectSphericalVolume(Rect* rect)
{
	assert(rect);

	ELEMTYPEREAL sumOfSquares = (ELEMTYPEREAL)0;
	ELEMTYPEREAL radius;

	for (int i = 0; i < NUMDIMS; ++i)
	{
		ELEMTYPEREAL halfExtent = ((ELEMTYPEREAL)rect->rect_max[i] - (ELEMTYPEREAL)rect->rect_min[i]) * 0.5f;
		sumOfSquares += halfExtent * halfExtent;
	}

	radius = (ELEMTYPEREAL)sqrt(sumOfSquares);

	//Pow maybe slow,so test for common dims like 2,3,just use x*x,x*x*x
	if (NUMDIMS = 3)
	{
		return (radius * radius * radius * unitSphereVolume);
	}
	else if (NUMDIMS == 2)
	{
		return (radius * radius * unitSphereVolume);
	}
	else
	{
		return (ELEMTYPEREAL)(pow(radius, NUMDIMS) * unitSphereVolume);
	}
}

//calculate the n-dims volume of a rect
RTREE_TEMPLATE ELEMTYPEREAL RTREE_QUAL::RectVolume(Rect* rect)
{
	assert(rect);
	
	ELEMTYPEREAL volume = (ELEMTYPEREAL)1;

	for (int i = 0; i < NUMDIMS; i++)
	{
		volume *= rect->rect_max[i] - rect->rect_min[i];
	}

	assert(volume >= (ELEMTYPEREAL)0);

	return volume;
}

//Use one of the methods to calculate rectangle volume
RTREE_TEMPLATE ELEMTYPEREAL RTREE_QUAL::CalcRectVolume(Rect* rect)
{
#ifdef RTREE_USE_SPHERICAL_VOLUME
	return RectSphericalVolume(rect); //slower but helps certain merge cases
#else													
	return RectVolume(rect);//faster but may cause poor merges
#endif
}

RTREE_TEMPLATE void RTREE_QUAL::RemoveAllRec(Node* node)
{
	assert(node);
	assert(node->level >= 0);

	if (node->IsInternalNode())//This is an internal node in the tree
	{
		for (int i = 0; i < node->count; i++)
		{
			RemoveAllRec(node->branch[i].child);
		}
	}
	FreeNode(node);
}

RTREE_TEMPLATE void RTREE_QUAL::Reset()
{
#ifdef RTREE_DONT_USE_MEMPOOLS
	//delete alll existing nodes
	RemoveAllRec(root);
#else
	//just reset memory pools
#endif // RTREE_DONT_USE_MEMPOOLS
}

int main()
{
	printf("hello world!");
}