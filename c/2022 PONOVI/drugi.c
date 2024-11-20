#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {RED, BLACK};

typedef struct Node {
    struct Node *left, *right, *parent;
    int broj, color;
    char igracevaBoja[30];
} Node;

Node *alociraj(int broj, char igracevaBoja[]){
    Node *res = (Node *)malloc(sizeof(Node));

    res->left = res->right = res->parent = NULL;
    res->color = RED;

    strcpy(res->igracevaBoja, igracevaBoja);
    res->broj = broj;

    return res;   
}


void leftR(Node **root){
    Node *l = *root;
    Node *r = l->right;

    l->right = r->left;
    if (l->right) l->right->parent = l;

    r->parent = l->parent;
    if (l->parent){
        if (l->parent->left == l) l->parent->left = r;
        else l->parent->right = r;
    }

    r->left = l;
    l->parent = r;

    *root = r;
}

void rightR(Node **root){
    Node *r = *root;
    Node *l = r->left;

    r->left = l->right;
    if (r->left) r->left->parent = r;

    l->parent = r->parent;
    if (r->parent){
        if (r->parent->left == r) r->parent->left = l;
        else r->parent->right = l;
    } 

    l->right = r;
    r->parent = l;

    *root = l;
}

void fixUP(Node ** root, Node *noda){
    Node *grandparent, *uncle;
    while (noda->parent && noda->parent->color == RED){
        grandparent = noda->parent->parent;
        if (grandparent->left == noda->parent){
            uncle = grandparent->right;
            if (uncle && uncle->color == RED){
                noda->parent->color = uncle->color = BLACK;
                grandparent->color = RED;

                noda = grandparent; 
            }
            else{
                if (noda->parent->right == noda){
                    noda = noda->parent;
                    leftR(&noda);
                    if (noda->parent == NULL) *root = noda;

                    noda = noda->left;
                }
                grandparent = noda->parent->parent;

                grandparent->color = RED;
                noda->parent->color = BLACK;

                rightR(&grandparent);
                if (grandparent->parent == NULL) *root = grandparent;
            }
        }
        else{
            uncle = grandparent->left;
            if (uncle && uncle->color == RED){
                noda->parent->color = uncle->color = BLACK;
                grandparent->color = RED;

                noda = grandparent; 
            }
            else{
                if (noda->parent->left == noda){
                    noda = noda->parent;
                    rightR(&noda);
                    if (noda->parent == NULL) *root = noda;

                    noda = noda->right;
                }
                grandparent = noda->parent->parent;

                grandparent->color = RED;
                noda->parent->color = BLACK;

                leftR(&grandparent);
                if (grandparent->parent == NULL) *root = grandparent;
            }
        }
    }
    (*root)->color = BLACK;
}

void dodaj(Node **root, Node *noda){
    Node *tr = *root, *pret = NULL;

    while (tr)
    {
        pret = tr;
        if (tr->broj > noda->broj || (tr->broj == noda->broj && strcmp(tr->igracevaBoja, "CRVENI")==0))
            tr = tr->left;
        else tr = tr->right;
    }
    
    noda->parent = pret;
    if (pret == NULL) *root = noda;
    else if (pret->broj > noda->broj || (pret->broj == noda->broj && strcmp(pret->igracevaBoja, "CRVENI")==0))
        pret->left = noda;
    else 
        pret->right = noda;

    fixUP(root, noda);
}

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
    printf ( "%d %s\n", root->broj, root->igracevaBoja);
    structure ( root->left, level + 1 );
  }
}

int kriterijum(Node *noda, int k) {
    //printf("%d %d\n", k, noda->broj);
    return (k%3 == 0 && noda->broj % 3 == 0) || (k%3 != 0 && noda->broj % 3 != 0);
}


void izbrisi(Node **root, Node *noda, int k){
    if (noda==NULL) return;

    Node *left = noda->left;
    Node *right = noda->right;

    if (kriterijum(noda, k)){
        noda->parent = noda->left = noda->right = NULL;
        dodaj(root, noda);
    }
    else
        free(noda);

    izbrisi(root, left, k);    
    izbrisi(root, right, k);    
}

void izbroji(Node *noda, int *brojac){
    if (noda == NULL) return;

    if (strcmp(noda->igracevaBoja,"CRVENI")== 0)
        *brojac += 1;
    else 
        *brojac -= 1;

    izbroji(noda->left, brojac);
    izbroji(noda->right, brojac);
}    

void find(Node *noda, int *najblizi, int k){
    if (noda == NULL) return;
    if (noda->broj == k){ *najblizi = noda->broj; return;} 

    if ( abs(noda->broj - k) < abs(*najblizi - k)) *najblizi = noda->broj;

    if (noda->broj > k) find(noda->left, najblizi, k);
    else find(noda->right, najblizi, k);
}

int main(){
    Node *root = NULL;
    int n,k;
    scanf("%d", &n);
    while (n--)
    {        
        char s[30];
        scanf("%d %s",&k,s);
        dodaj(&root, alociraj(k,s));        
    }
    structure(root, 0);

    scanf("%d", &k);
    Node *newRoot = NULL;
    izbrisi(&newRoot, root, k);
    structure(newRoot, 0);

    int skala = 0;
    izbroji(newRoot, &skala);
    printf("%d", skala);

    int najblizi = newRoot->broj;

    find(newRoot, &najblizi, k);

    printf("%d", najblizi); 
} 