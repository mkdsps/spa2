#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

enum {RED, BLACK};

typedef struct {
    int id;
    float udaljenost;
    char tip[30];
} Vozilo;

typedef struct Node {
    struct Node *left,*right, *parent;
    int color;
    Vozilo vozilo;
} Node;

float uda(float x, float y){
    return sqrt(x*x+y*y);
}

Node *alociraj(int id, int x, int y, char tip[]){
    Node *res = (Node *)malloc(sizeof(Node));

    res->left = res->right = res->parent = NULL;
    res->color = RED;

    res->vozilo.udaljenost = uda(x,y);
    res->vozilo.id = id;
    strcpy(res->vozilo.tip, tip);

    return res;
}

void leftR(Node **root){
    Node *l = *root;
    Node *r = l->right;

    l->right = r->left;
    if (l->right) l->right->parent = l;

    r->parent = l->parent;
    if(l->parent){
        if (l->parent->left == l) l->parent->left = r;
        else l->parent->right = r;
    } 
    r->left = l;
    l->parent = r;

    *root =r; 
}

void rightR(Node **root){
    Node *r = *root;
    Node *l = r->left;

    r->left = l->right;
    if (r->left) r->left->parent = r;

    l->parent = r->parent;
    if(r->parent){
        if (r->parent->left == r) r->parent->left = l;
        else r->parent->right = l;
    }
    l->right = r;
    r->parent = l;

    *root = l;
}

void fixUP(Node **root, Node *noda){
    Node *grandparent, *uncle;
    while(noda->parent && noda->parent->color == RED){
        grandparent = noda->parent->parent;
        if (grandparent->left == noda->parent){
            uncle = grandparent->right;
            if (uncle && uncle->color == RED){
                grandparent->color = RED;
                uncle->color = noda->parent->color = BLACK;

                noda= grandparent;
            }
            else{
                if (noda->parent->right == noda){
                    noda = noda->parent;
                    leftR(&noda);
                    if (noda->parent == NULL) *root = noda;

                    noda = noda->left;
                }
                grandparent = noda->parent->parent;

                noda->parent->color = BLACK;
                grandparent ->color = RED;

                rightR(&grandparent);
                if (grandparent->parent == NULL) *root = noda; 
            }
        }
        else{
            uncle = grandparent->left;
            if (uncle && uncle->color == RED){
                grandparent->color = RED;
                uncle->color = noda->parent->color = BLACK;

                noda= grandparent;
            }
            else{
                if (noda->parent->left == noda){
                    noda = noda->parent;
                    rightR(&noda);
                    if (noda->parent == NULL) *root = noda;

                    noda = noda->right;
                }
                grandparent = noda->parent->parent;

                noda->parent->color = BLACK;
                grandparent ->color = RED;

                leftR(&grandparent);
                if (grandparent->parent == NULL) *root = noda; 
            }
        }
    } 
    (*root)->color = BLACK;
}

int uporedi(Vozilo a, Vozilo b){
    return ( a.udaljenost > b.udaljenost ) || (a.udaljenost == b.udaljenost && strcmp(a.tip, "KAMION") == 0);  
}

void dodaj(Node **root, Node *noda){
    Node *tr = *root , *pret = NULL;

    while (tr)
    {
        pret = tr;
        if (uporedi(tr->vozilo, noda->vozilo)) tr = tr->left;
        else tr = tr->right;
    }

    noda->parent = pret;
    if (pret==NULL) *root = noda;
    else if (uporedi(pret->vozilo, noda->vozilo)) pret->left = noda;
    else pret->right = noda;   
    
    fixUP(root, noda);
}

Node *form(){
    Node *root = NULL;
    
    int n, x, y, id;
    char tip[30];

    scanf("%d", &n);
    while (n--)
    {
        scanf("%d %d %d %s", &id, &x, &y, tip);
        dodaj(&root, alociraj(id,x,y,tip));
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
    printf ( "%d %s %f\n", root->vozilo.id, root->vozilo.tip, root->vozilo.udaljenost);
    structure ( root->left, level + 1 );
  }
}

int kriterijum(Node *noda, float udaljenost, char tip[]){
    return strcmp(noda->vozilo.tip,tip) == 0 && udaljenost > noda->vozilo.udaljenost;
}

void izbrisi(Node **newRoot, Node *noda, float udaljenost, char tip[]){
    if (noda == NULL) return;

    Node *left = noda->left;
    Node *right = noda->right;

    if (kriterijum(noda, udaljenost, tip)){
        noda->parent = noda->left = noda->right = NULL;
        dodaj(newRoot, noda);
    }
    else free(noda);

    izbrisi(newRoot, left, udaljenost, tip);
    izbrisi(newRoot, right, udaljenost, tip);
}

Node* nadjiNajblizi(Node* node, float data) {
    if (!node) return NULL; // Ako stablo ne postoji, vraćamo nullptr.

    Node* res = node;

    while (node) {
        // Ako nađemo tačno podudaranje, vraćamo trenutni čvor.
        if (node->vozilo.udaljenost == data) 
            return node;

        // Ažuriramo rezultat ako je trenutni čvor bliži zadatoj vrednosti.
        if (abs(node->vozilo.udaljenost - data) < abs(res->vozilo.udaljenost - data))
            res = node;

        // Krećemo se u zavisnosti od vrednosti u čvoru.
        if (data < node->vozilo.udaljenost) 
            node = node->left;         
        else 
            node = node->right;
    }

    return res;
}


int main(){
    Node *root = form();
    structure(root, 0);

    float udaljenost;
    char tip[30];
    scanf("%f %s", &udaljenost, tip);

    Node *newRoot = NULL;
    izbrisi(&newRoot, root, udaljenost, tip);
    structure(newRoot, 0);

    Node *naj = nadjiNajblizi(newRoot, udaljenost);
    if (naj) printf("%f", naj->vozilo.udaljenost);
    else printf("PRAZNO");
}