#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    int data;
    struct node *left, *right;   
} node;

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



node *alociraj(int data){
    node *noda = (node*)malloc(sizeof(node));
    noda->left = NULL;
    noda->right = NULL;

    noda->data = data;
    return noda;
}

void dodaj(node **root, int data){
    if (*root == NULL){
        *root = alociraj(data);
        return;
    }

    node *tr = *root, *pret = NULL;

    while(tr){
        pret = tr;
        if (tr->data > data) tr = tr->left;
        else tr = tr->right;
    }

    if (pret->data < data) pret->right = alociraj(data);
    else pret->left = alociraj(data);  
}

node *formiraj(){
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

// void transform(node* noda, int *suma){
//     if(noda == NULL) return;
    
//     transform(noda->right, suma);
//     noda->data += *suma;
//     *suma= noda->data;
//     transform(noda->left, suma);
// }

void transform(node* noda){
    static int suma = 0;
    if(noda == NULL) return;
    
    transform(noda->right);
    noda->data += suma;
    suma= noda->data;
    transform(noda->left);
}


int main(){
    node *root = formiraj();
    structure(root, 0);
    printf("=============================\n");    

    int suma = 0;
    transform(root);
    structure(root, 0);
}
