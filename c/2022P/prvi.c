#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int data;
    struct node *left, *right;
} node;

node *alociraj(int data){
    node *res = (node*)malloc(sizeof(node));
    res->left = res->right = NULL;
    res->data = data;

    return res;
}

void dodaj(node **root, int data){
    node *tr = *root, *pret = NULL;

    if (tr == NULL) {
        *root = alociraj(data);
        return;
    }

    while(tr){
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

node *formiraj(){
    node *root = NULL;
    int n, k;
    scanf("%d", &n);
    while (n--)
    {
        scanf("%d", &k);
        dodaj(&root, k);
        //structure (root, 0);
    }
    return root;
}

int dajK(node *root, int *brojac, int k){
    if (root == NULL) return 0;

    int broj = dajK(root->right, brojac, k);
    if (broj) return broj;

    //printf("%d %d \n",  root->data, *brojac);
    if (*brojac == k) return root->data;

    *brojac += 1;
    return dajK(root->left,brojac, k);
}  

int main(){
    node *root = formiraj();
    int brojac = 1;
    int k;
    scanf("%d", &k);
    printf("%d",dajK(root, &brojac, k));
}