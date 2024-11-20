#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    int data;
    struct node *left, *right;
}node;

node *alociraj(int x){
    node *res = (node *)malloc(sizeof(node));
    res->left = NULL;
    res->right = NULL;
    res->data = x;

    return res;
}

void dodaj(node **root, int data){
    node *tr = *root, *pret = NULL;

    if (*root == NULL){
        *root = alociraj(data);
        return;
    }

    while (tr)
    {
        pret = tr;
        if (tr->data > data) tr = tr->left;
        else tr = tr->right;
    }

    if (pret->data > data) pret->left = alociraj(data);
    else pret->right = alociraj(data);    
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

node *form(){
    node *root = NULL;
    int n, k;
    scanf("%d", &n);

    while (n--)
    {
        scanf("%d", &k);
        dodaj(&root, k);
    }

    return root;    
}

int isValid(node *noda, int max, int min){ // prvim pozivom prethodnik ce biti postaveljen na min
    if (noda == NULL) return 1;

    if (noda->data < min || noda->data > max)
        return 0;

    return isValid(noda->left, noda->data, min) && isValid(noda->right, max, noda->data);    

    
}

int main(){
    node *root = form();
    structure(root, 0);

}