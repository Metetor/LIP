#ifndef RTree_H
#define RTree_H
//RTree implementation(C++),refer to https://superliminal.com/sources/
//#include<stdio.h>

#define RTREE_TEMPLATE template <class DATATYPE, class ELEMTYPE, int NUMDIMS, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
#define RTREE_QUAL RTree<DATATYPE, ELEMTYPE, NUMDIMS, ELEMTYPEREAL, TMAXNODES, TMINNODES>

template<class DATATYPE, class ELEMTYPE, int NUMDIMS, class ELEMTYPEREAL = ELEMTYPE, int TMAXNODES = 8, int TMINNODES = TMAXNODES / 2>
class RTree
{
public:
	struct Node;
	enum
	{
		MAXNODES = TMAXNODES,
		MINNODES = TMINNODES,
	};

public:

	RTree();
	virtual ~RTree();

	//Insert entry
	/// \param rect_min Min of bounding rect
	/// \param rect_max Max of bounding rect
	/// \param entryId positive Id of  entry.zero is acceptable but negative not
	void Insert(const ELEMTYPE rect_min[NUMDIMS], const ELEMTYPE rect_max[NUMDIMS], const DATATYPE& entryId);

protected:
	//MBR-Minimal Bounding rectangle (n-dims)
	struct Rect
	{
		ELEMTYPE rect_min[NUMDIMS]; ///< Min of bounding box
		ELEMTYPE rect_max[NUMDIMS]; ///< Max of bounding box
	};

	// may be data or subtree
	//if the parent's level is 0,then this is data
	struct Branch
	{
		Rect rect; //Bounds
		Node* child; ///child node
		DATATYPE data;//data Id or ptr
	};

	// Node for each branch level
	struct Node
	{
		bool IsInternalNode() { return (level > 0); }
		bool IsLeaf() { return (level == 0); }

		int count;  /// Count
		int level;  /// Leaf is zero,others positive
		Branch branch[MAXNODES]; ///Branch
	};

	//A link list of nodes for reinsertion after a delete operation
	struct ListNode
	{
		ListNode* m_next;//next ptr
		Node* head;//node
	};

	//Variables for finding a split partition
	struct PartitionVars
	{
		int partition[MAXNODES + 1];
		int total;
		int minFill;
		int taken[MAXNODES + 1];
		int count[2];
		Rect cover[2];
		ELEMTYPEREAL area[2];

		Branch branchBuf[MAXNODES + 1];
		int branchCount;
		Rect coverSplit;
		ELEMTYPEREAL coverSplitArea;
	};

	Node* AllocNode();
	void FreeNode(Node* node);
	void InitNode(Node* node);
	bool InsertRectR(Rect* rect, const DATATYPE& id, Node* node, Node** newNode, int level);
	bool InsertRect(Rect* rect, const DATATYPE& id, Node** root, int level);
	bool AddBranch(Branch* branch, Node* node, Node** newNode);
	int PickBranch(Rect* rect, Node* node);
	void SplitNode(Node* node, Branch* branch, Node** newNode);
	ELEMTYPEREAL RectSphericalVolume(Rect* rect);
	ELEMTYPEREAL RectVolume(Rect* rect);
	ELEMTYPEREAL CalcRectVolume(Rect* rect);
	void GetBranches(Node* node, Branch* branch, PartitionVars* parVars);
	void RemoveAllRec(Node* node);
	void Reset();


	Node* root; // root of tree
	ELEMTYPEREAL unitSphereVolume; //unit sphere const for required number of dims
};



#endif

