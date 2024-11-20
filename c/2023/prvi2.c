#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    struct node *left, *right;
    int data;
}node;


node *alociraj(int data){
    node *res = (node *)malloc(sizeof(node));
    
    res->left = res->right = NULL;
    res->data = data;

    return res;
}

void dodaj(node **root, node *noda){
    node *tr = *root, *pret = NULL;

    if (tr == NULL) {
        *root = noda;
        return;
    }

    while(tr){
        pret = tr;
        if (tr->data > noda->data) tr = tr->left;
        else tr = tr->right;
    }

    if(pret ->data > noda->data) pret->left = noda;
    else pret->right = noda;
}

node *form(){
    node *root = NULL;
    int n, k;
    scanf("%d", &n);

    while (n--)
    {
        scanf("%d", &k);
        dodaj(&root, alociraj(k));
    }

    return root;    
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

node *trim(node *noda, int l, int d){
    if (noda == NULL) return NULL;

    if (noda->data >= l && noda->data <= d){
        noda->left = trim(noda->left, l, d);
        noda->right = trim(noda->right, l, d);
        return noda;
    }

    if (noda->data < l)
        return trim(noda->right,l,d);

    if (noda->data > d)
        return trim(noda->left, l, d);
}   

int main(){
    node *root = form();
    structure(root, 0);

    int l,d;
    scanf("%d %d", &l, &d);
    root = trim(root, l, d);
    structure(root, 0);
}