#include <stdio.h>
#include <stdlib.h>

enum {RED, BLACK};

typedef struct {
    int capital, broj, productID;
} Ucesnik;

typedef struct Node{
    struct Node *left, *right, *parent;
    Ucesnik ucesnik;
    int boja;
} Node;

Node *alociraj(int capital, int broj, int productID){
    Node *res = (Node *)malloc(sizeof(Node));

    res->left = res->right = res->parent = NULL;
    res->boja = RED;

    res->ucesnik.broj = broj;
    res->ucesnik.capital = capital;
    res->ucesnik.productID = productID;

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
        if(r->parent->left == r) r->parent->left = l;
        else r->parent->right = l;
    }

    l->right = r;
    r->parent = l;

    *root = l;
}

void fixUP(Node **root, Node *noda){
    Node *grandparent, *uncle;
    while(noda->parent && noda->parent->boja == RED){
        grandparent = noda->parent->parent;
        if (grandparent->left == noda->parent){
            uncle = grandparent->right;
            if (uncle && uncle->boja == RED){
                grandparent->boja = RED;
                noda->parent->boja = uncle->boja = BLACK;

                noda = grandparent;             
            }
            else{
                if (noda->parent->right == noda){
                    noda = noda->parent;
                    leftR(&noda);

                    if(noda->parent == NULL) *root = noda; 
                    noda = noda->left;                      
                }        
                grandparent = noda->parent->parent;
                noda->parent->boja = BLACK;
                grandparent->boja = RED;
                
                rightR(&grandparent);
                if (grandparent->parent == NULL)
                    *root = grandparent;
            }
        }
        else{
            uncle = grandparent->left;
            if (uncle && uncle->boja == RED){
                grandparent->boja = RED;
                noda->parent->boja = uncle->boja = BLACK;

                noda = grandparent; 
            }
            else{
                if (noda == noda->parent->left){
                    noda = noda->parent;
                    rightR(&noda);
                    if (noda->parent == NULL) *root = noda;

                    noda= noda->right; 
                }
                grandparent = noda->parent->parent;
                grandparent->boja = RED;
                noda->parent->boja = BLACK;

                leftR(&grandparent);
                if (grandparent->parent == NULL) *root = grandparent; 
            }
        }
    }
    (*root)->boja = BLACK;
}

void dodaj(Node **root, Node *nov){
    Node *tr = *root, *pret = NULL;

    while (tr)
    {
        pret = tr;
        if (tr->ucesnik.capital > nov->ucesnik.capital || (tr->ucesnik.capital ==  nov->ucesnik.capital && tr->ucesnik.broj > nov->ucesnik.broj))
            tr = tr->left;
        else tr = tr->right;    
    }
    
    nov->parent = pret;
    if (pret == NULL) *root = nov;
    else if(pret->ucesnik.capital > nov->ucesnik.capital || (pret->ucesnik.capital ==  nov->ucesnik.capital && pret->ucesnik.broj > nov->ucesnik.broj))
        pret->left = nov;
    else pret->right = nov;

    fixUP(root, nov);
}

Node *formiraj(){
    Node *root = NULL;
    int n, kapital, broj, id;
    scanf("%d", &n);
    while(n--){
        scanf("%d %d %d", &kapital, &broj, &id);
        dodaj(&root, alociraj(kapital, broj, id));
    }
    return root;
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
    printf ( "%d %d %d\n", root->ucesnik.capital, root->ucesnik.broj, root->ucesnik.productID);
    structure ( root->left, level + 1 );
  }
}

void izbrisi(Node **newRoot, Node *noda, int niz[]){
    if (noda == NULL) return;

    Node *left = noda->left;
    Node *right = noda->right;

    if (noda->ucesnik.capital >= noda->ucesnik.broj* niz[noda->ucesnik.productID]){
        noda->left = noda->right = noda->parent = NULL;
        noda->boja = RED;
        dodaj(newRoot, noda);
    }
    else 
        free(noda);

   izbrisi(newRoot, left, niz);
   izbrisi(newRoot, right, niz);    
}

void pronadjiSumu(Node* noda, int *suma, int *brojac, int granica){
    if (noda == NULL) return;

    if (noda->ucesnik.capital >= granica){
        *suma += noda->ucesnik.capital;
        *brojac+= 1;

        pronadjiSumu(noda->right, suma, brojac, granica);
        pronadjiSumu(noda->left, suma, brojac, granica);       
    } 
    else
        pronadjiSumu(noda->right, suma, brojac, granica);
}

int main(){
    Node *root = formiraj();
    structure(root, 0);
    printf("=======================================\n");
    int m, niz[100];
    scanf("%d", &m);

    for (int i = 0 ; i < m ; i++)
        scanf("%d", &niz[i]);
    
    Node *newRoot = NULL;
    izbrisi(&newRoot, root, niz);
    structure(newRoot, 0);

    int x, suma = 0, brojac = 0; 
    scanf("%d", &x);
    
    pronadjiSumu(newRoot, &suma, &brojac, x); 
    printf("%d %d \n", suma, brojac );
}