#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    int data;
    struct node *left, *right;
} node;

node *alociraj(int data){
    node *res = (node *)malloc(sizeof(node));
    res->left = NULL;
    res->right= NULL;
    res->data = data;

    return res;
}

void dodaj(node **root, int data){
    node *tr = *root, *pret = NULL;

    while(tr){
        pret = tr;
        if (tr->data > data) tr = tr->left;
        else tr = tr->right;
    }

    if (pret == NULL) *root = alociraj(data);
    else if(pret->data > data) pret->left = alociraj(data);
    else pret->right = alociraj(data); 
}

node* form(){
    node *res = NULL;
    int n, k;
    scanf("%d", &n);
    while (n--)
    {
        scanf("%d", &k);
        dodaj(&res, k);
    }
    return res;
}

void padding ( char ch, int n )
{
  int i;
  for ( i = 0; i < n; i++ )
    putchar ( ch );
}
void structure ( node *root, int level )
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

node *nadji(node *noda,int k, int *brojac){
    if (noda == NULL) return NULL;

    node *res = nadji(noda->right,k,brojac);
    if (res)
        return res;

    if (*brojac == k)
        return noda;
    
    *brojac += 1;
    return nadji(noda->left, k, brojac);
}

int main(){
    node *root = form();
    structure(root, 0);
    
    int k, brojac = 1;
    scanf("%d",&k);

    node *res = nadji(root, k, &brojac);
    printf("%d", res->data);
}