#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
    struct Node *left , *right;
    int data;
} Node;

Node *alociraj(int data){
    Node *res = (Node *)malloc(sizeof(Node));
    res->data = data;
    res->left = res->right = NULL;

    return res;
}

Node *find(Node *noda, int data){
    if (noda == NULL) return NULL;
    if (noda->data == data) return noda;

    Node *left = find(noda->left, data);
    if (left)
        return left;

    return find(noda->right, data);    
}

void dodaj(Node *root, int gde , int l, int d){
    Node *noda = find(root, gde);

    if (noda == NULL) return;

    if (l)
        noda->left = alociraj(l);

    if (d)
        noda->right = alociraj(d);
}

Node* formiraj(){
    int n, l, d;
    scanf("%d ", &n);
    
    Node *root = alociraj(n);
    scanf("%d ", &n);

    while (n)
    {
        scanf("%d %d", &l, &d);
        dodaj(root,n,l,d);
        scanf("%d", &n);
    }
    return root;
}

void padding ( char ch, int n )
{
  int i;
  for ( i = 0; i < n; i++ )
    putchar ( ch );
}
void structure ( Node *root, int level )
{
  int i;
  if ( root == NULL ) {
    padding ( '\t', level );
    puts ( "~" );
  }
  else {
    structure ( root->right, level + 1 );
    padding ( '\t', level );
    printf ( "%d\n", root->data );
    structure ( root->left, level + 1 );
  }
}

Node *findLCA(Node *noda, int x, int y, int *count){
    if (noda == NULL) return NULL;

    Node *left = findLCA(noda->left, x,y,count);
    Node *right = findLCA(noda->right, x,y,count);

    if (noda->data == x || noda->data == y){
        *count += 1;
        return noda;
    }

    if (left && right) return noda;

    return left ? left : right;
}

void resi(Node *root){
    int k;
    scanf("%d", &k);
    while (k--)
    {
        int l, d, count = 0;
        scanf("%d %d", &l, &d);
        Node *lca = findLCA(root, l , d, &count);

        if(count == 2 && lca || count == 1 && l==d) 
            printf("NZP(%d,%d)=%d\n",l, d, lca->data);
        else 
            printf("NZP(%d,%d)=NULL\n", l, d);

    }
    
}


int main(){
    Node *root = formiraj();
    structure (root, 0);
    resi(root);
}