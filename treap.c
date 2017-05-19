
/*	TREAP -> A data structure which is a mixture of a binary search tree and a heap
 * This example shows a Min Treap, i.e. the priority of the child elements is greater than the priority of its parent.
 * Priority is assigned randomly, ensures random distribution of priorities, i.e tree comes out more balanced.
 */

#include<stdio.h>
#include<stdlib.h>

/* STATUS CODE
 * 0 -> FAILURE
 * 1 -> SUCCESS
 * returned by functions as StatusCode 
 */
typedef enum{FAIL,SUCCESS}StatusCode;

/* RANDOM NUMBER GENERATOR
 * returns a random number in the range 1 to max
 */
long random_at_most(long max) {
  unsigned long
    // max <= RAND_MAX < ULONG_MAX, so this is okay.
    num_bins = (unsigned long) max + 1,
    num_rand = (unsigned long) RAND_MAX + 1,
    bin_size = num_rand / num_bins,
    defect   = num_rand % num_bins;

  long x;
  do {
   x = random();
  }
  // This is carefully written not to overflow
  while (num_rand - defect <= (unsigned long)x);

  // Truncated division is intentional
  return x/bin_size;
}


/* Structure of Node
 * data -> data value to be stored in node. (Could be of type itemtype)
 * priority -> random priority number assigned to each node to ensure balanced tree property
 * left -> pointer to left child/subtree 
 * right -> pointer to right child/subtree
 */
typedef struct node_tag{
	char data;.
	int priority;
	struct node_tag *left;
	struct node_tag *right;
}node;



/* FUNCTION MAKENODE
 * Returns a pointer to node with data field set to data
 * left set to NULL
 * right set to NULL
 * priority set to a random value between 1 to 99
 */
node* makeNode(char data){
	node *nptr = (node*)malloc(sizeof(node));
	nptr->data = data;
	nptr->left = NULL;
	nptr->right = NULL;
	nptr->priority = random_at_most(99);

	return(nptr);
}



/* Function Rotate Left
 * rotates nptr one level left
 * returns the pointer to node at current nptr position
 * USAGE: parent -> child = rotateLeft(parent->child); 
 *	--> child can be left or right child of parent node
 */
node* rotateLeft(node *nptr){

	node *q = NULL;
	if(nptr != NULL && nptr->right != NULL){
		node *p;
		p = nptr;
		q = nptr->right;
		p->right = q->left;
		q->left = p;
		
	}
	return q;
	
}


/* FUNCTION Rotate Right
 * rotates nptr node one level right
 * returns new pointer to node at the position of nptr
 * USAGE: parent -> child = rotateRight(parent -> child)
 *	--> child can be left or right child of parent
 */
node* rotateRight(node *nptr){

	node *q = NULL;
	if(nptr != NULL && nptr->left != NULL){
		node *p;
		p = nptr;
		q = nptr->left;
		p->left = q->right;
		q->right = p;
		
	}
	return q;
	
}


/* FUNCTION INSERT IN TREAP (Recursive)
 * Parameters: root -> root of tree , nptr -> node to be inserted
 * Returns: root of tree after insertion
 * Recursively goes through all nodes satisfying BST property (against data) and Min Heap property (against priority).
 * Does rotations to satisfy Min Heap property.
 */
node* insert(node *root,node *nptr){
	
	if(root==NULL)
		root = nptr;		//if tree is NULL
	else{
		if(nptr->data<root->data){
			root->left = insert(root->left,nptr);	//go left if nptr data is less than current root.
			if(root->left->priority < root->priority){	//rotate from bottom up to satisfy min heap property
				root = rotateRight(root);
			}
		}
		else{
			root->right = insert(root->right,nptr);
			if(root->right->priority < root->priority){
				root = rotateLeft(root);
			}
		}
		
		
	}
	return root;
}


/* FUNCTION CREATE TREAP
 * Parameters: n -> number of nodes in treap
 * Returns: root of the treap with n nodes
 * Internally asks for data values of the treap.
 * Functions called: makeNode() and insert()
 */
node* CreateTreap(int n){
	
	int i;
	char d;
	node *tree=NULL,*nptr;
	for(i=0;i<n;i++){
		scanf("%c",&d);scanf("%c",&d);	
		nptr=makeNode(d);
		tree=insert(tree,nptr);
	}
	return tree;
}


/* FUNCTION PRINT INORDER
 * Parameter: tree -> root of the treap to be printed inorder (LVR)
 * Returns: void
 * Utility: to check if BST property is satisfied. (Debugging)
 */
void printTreeinOrder(node *tree){
	

	if(tree !=NULL)
	{
		printTreeinOrder(tree->left);
		printf("%c:%d ",tree->data,tree->priority);
		printTreeinOrder(tree->right);
	}
}



/* FUNCTION PRINT PREORDER
 * Parameter: tree -> root of the treap to be printed preorder (VLR)
 * Returns: void
 * Utility: to check if min heap property is satisfied. (Debugging)
 */
void printTreepreOrder(node *tree){
	if(tree !=NULL)
	{
		printf("%c:%d ",tree->data,tree->priority);		
		printTreepreOrder(tree->left);
		printTreepreOrder(tree->right);
	}
}


/* FUNCTION SEARCH TREAP (Recursive)
 * Parameter: root -> root of the treap , n -> character/data to be searched for
 * Returns: StatusCode ---> SUCCESS = 1 if data is found, FAILURE = 0 if not found.
 * Checks current root data value and uses BST property to recurse to left or right subtree.
 */
StatusCode Search(node *root,char n){
	StatusCode SC;
	if(root == NULL){
		SC = FAIL;
	}
	else if(root->data == n){
		SC = SUCCESS;
	}
	else if(n < root->data){
		SC = Search(root->left,n);
	}
	else if(n > root->data){
		SC = Search(root->right,n);
	}
	return SC;
}

/* FUNCTION DELETION IN BST
 * Parameters: nptr -> node to be deleted in the treap
 * Return value: pointer to new root of the treap
 * Case 1: nptr is leaf node
 	--> delete nptr and assign parent to NULL
 * Case 2: nptr has one subtree
 	--> delete nptr and attach the subtree directly to the parent node
 * Case 3: nptr has two subtrees
 	--> Deletes nptr and finds the largest value in the left subtree to replace the hole with.
	
 ******* THIS FUNCTION IS JUST FOR DEBUGGING, NOT REQUIRED IN THE TREAP *******
 */
node* deleteBST(node *nptr){
	node *newroot=NULL;
	if(nptr !=NULL){
		if(nptr->left == NULL){
			newroot = nptr->right;
		}
		else if(nptr->right == NULL){
			newroot = nptr->left;
		}
		else{
			node *p,*q;
			q=nptr->left; p=nptr->left;
			for(;q->right!=NULL;){
				p = q;
				q = q->right;
			}
			if(p==q){
				q->right = nptr->right;
				newroot = q;
			}
			else{
				p->right = q->left;
				q->left = nptr->left;
				q->right = nptr->right;
				newroot = q;
				newroot = rotateRight(newroot);
				node *r = newroot->right;
				int flag = 0;
				for(;r!=NULL && r->left!=NULL && flag==0;){
					flag=1;
					if(r->priority > r->right->priority){
					r = rotateLeft(r);
					r=r->left;
					flag=0;
					}
				}
				printf("\n*Rotated*\n");
			}
			
			
		}
		free(nptr);
	}
	return newroot;
}

/* FUNCTION DELETION IN TREAP (Recursive)
 * PARAMETERS: root -> root of the treap, d -> data part of the node to be deleted, SC -> passed by reference, stores the StatusCode (SUCCESS or FAILURE of DELETION)
 * RETURNS: new root of the treap after deletion
 * Searches for character d in the treap
 * Once found, brings the node to be deleted to leaf/single child level by rotations so that min heap property is satisfied after deletion.
 * If not found, SC is set to FAIL
 */
node* delete(node *root,char d,StatusCode *SC){
	if(root == NULL){
		*SC = FAIL;
	}
	else if(d<root->data){
		root->left = delete(root->left,d,SC);
	}
	else if(d > root->data){
		root->right = delete(root->right,d,SC);
	}
	else{
		if(root->right == NULL){
			node *temp = root;
			root = root->left;
			free(temp);
			*SC = SUCCESS;
		}
		else if(root->left == NULL){
			node *temp = root;
			root = root->right;
			free(temp);
			*SC=SUCCESS;
		}
		else{
			if(root->left->priority < root->right->priority){
				root = rotateRight(root);
				root->right = delete(root->right,d,SC);
			}
			else{
				root = rotateLeft(root);
				root->left = delete(root->left,d,SC);
			}
			
		}
	}
	return root;
}

void main(){

	node *tree,*nptr;
	int n;
	printf("Enter no. of elements: ");
	scanf("%d",&n);
	tree = CreateTreap(n);	
	printf("\n");
		printf("\n");
		printf("Inorder: ");
		printTreeinOrder(tree);
		printf("\n");
		printf("PreOrder: ");
		printTreepreOrder(tree);
		printf("\n");
		printf("\n-----------------------------------------------------------------------------------------\n");

	int choice;
	char c;
	do{
		// menu 
		printf("Enter your choice: \n1.Insert\n2.Delete\n3.Search\n4.Exit\nEnter: ");
		scanf("%d",&choice);
		switch(choice){
			case 1:	printf("Enter character to be inserted: ");
				scanf("%c",&c);scanf("%c",&c);
				nptr = makeNode(c);
				tree = insert(tree,nptr);
				break;
			case 2: printf("Enter character to be deleted: ");
				scanf("%c",&c);scanf("%c",&c);
				StatusCode SC;
				tree = delete(tree,c,&SC);
				if(SC) printf("Character Found and Deleted.\n");
				else printf("Character not found.\n");
				break;
			case 3: printf("Enter number to be searched: ");
				scanf("%c",&c); scanf("%c",&c);
				SC = Search(tree,c);
				if(SC) printf("Character Found.\n");
				else printf("Character not found.\n");
				break;
			case 4:	
				break;
			default: printf("Invalid choice");
		}
		
		//to check if the treap is satisfying all properties
		printf("\n");
		printf("\n");
		printf("Inorder: ");
		printTreeinOrder(tree);
		printf("\n");
		printf("PreOrder: ");
		printTreepreOrder(tree);
		printf("\n");
		printf("\n-----------------------------------------------------------------------------------------\n");
	
		
	}while(choice!=4);
	
}
