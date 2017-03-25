#include<stdio.h>
#include<stdlib.h>

typedef enum{FAIL,SUCCESS}StatusCode;

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

typedef struct node_tag{
	char data;
	int priority;
	struct node_tag *left;
	struct node_tag *right;
}node;

node* makeNode(char data){
	node *nptr = (node*)malloc(sizeof(node));
	nptr->data = data;
	nptr->left = NULL;
	nptr->right = NULL;
	nptr->priority = random_at_most(99);

	return(nptr);
}

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


node* insert(node *root,node *nptr){
	
	if(root==NULL)
		root = nptr;
	else{
		if(nptr->data<root->data){
			root->left = insert(root->left,nptr);
			if(root->left->priority < root->priority){
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

void printTreeinOrder(node *tree){
	

	if(tree !=NULL)
	{
		printTreeinOrder(tree->left);
		printf("%c:%d ",tree->data,tree->priority);
		printTreeinOrder(tree->right);
	}
}

void printTreepreOrder(node *tree){
	if(tree !=NULL)
	{
		printf("%c:%d ",tree->data,tree->priority);		
		printTreepreOrder(tree->left);
		printTreepreOrder(tree->right);
	}
}

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
