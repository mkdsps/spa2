#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Prijava{
    int broj;
    char naziv[100];
} Prijava;

typedef struct Node{
    Prijava prijava;
    struct Node *left, *right;
} Node;

Node *alociraj(int broj, char naziv[]){
    Node *res = (Node*)malloc(sizeof(Node));

    res->left = NULL;
    res->right = NULL;
    res->prijava.broj = broj;

    strcpy(res->prijava.naziv, naziv);
    return res;
}

Node* dodaj(Node *root, int broj, char naziv[]){
    Node *tr = root, *pret = NULL;

    if (root == NULL) return alociraj(broj, naziv);

    while (tr)
    {   
        pret = tr;
        if (strcmp(tr->prijava.naziv,naziv) > 0) tr = tr->left;
        else tr = tr->right;
    }

    if (strcmp(pret->prijava.naziv,naziv) > 0) pret->left = alociraj(broj, naziv);
    else pret->right = alociraj(broj, naziv);

    return root;      
}

// ISPIS
void padding ( char ch, int n )
{
  int i;
  for ( i = 0; i < n; i++ )
    putchar ( ch );
}
void structure (  Node *root, int level )
{
  int i;
  if ( root == NULL ) {
    padding ( '\t', level );
    puts ( "~" );
  }
  else {
    structure ( root->right, level + 1 );
    padding ( '\t', level );
    printf ( "%s %d\n", root->prijava.naziv, root->prijava.broj);
    structure ( root->left, level + 1 );
  }
}

int MAX(int a, int b){
    if (a > b) return a;
    return b;
}

int vratiMax(Node *noda, int broj){
    if (noda == NULL) return 0;

    if (broj) return MAX(vratiMax(noda->left, broj-1), vratiMax(noda->right, broj-1));
    return noda->prijava.broj;  
} 


int main(){
    Node *root = NULL;
    int n;
    scanf("%d", &n);
    while(n--){
        int broj;
        char naziv[100];

        scanf("%s %d", naziv, &broj);
        root = dodaj(root, broj, naziv);
    }
    structure(root, 0);
    printf("\n ========================= \n\n");

    
    scanf("%d", &n);
    printf("%d", vratiMax(root, n-1));

}