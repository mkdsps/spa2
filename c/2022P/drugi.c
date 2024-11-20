#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

enum {RED, BLACK};

typedef struct {
    int id,x,y;
    char tip[50]; 
} Vozilo; 

typedef struct Node{
    struct Node *left, *right, *parent;
    int color;
    Vozilo vozilo;
} Node;

float uda(Vozilo a){
    return sqrt((float)(a.x *a.x + a.y*a.y));
}

int uporedi(Vozilo a, Vozilo b){ // vraca 1 ako je prvo vece;
    float udaljenostA = uda(a);
    float udaljenostB =  uda(b);
    return ((udaljenostA > udaljenostB) || (udaljenostA == udaljenostB && strcmp(a.tip,"KAMION")==0));
}

Node *alociraj(int id, int x, int y, char tip[]){
    Node *res = (Node *)malloc(sizeof(Node));
    res->left = res->right = res->parent = NULL;
    res->color = RED;

    res->vozilo.id = id; 
    res->vozilo.x = x;
    res->vozilo.y = y; 
    strcpy(res->vozilo.tip,tip);

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

void fixUP(Node **root, Node *noda){
    Node *grandparent, *uncle;
    while (noda->parent && noda->parent->color == RED){
        grandparent = noda->parent->parent;
        if (noda->parent == grandparent->left){
            uncle = grandparent->right;
            if (uncle && uncle->color == RED){
                grandparent->color = RED;
                uncle->color = noda->parent->color =BLACK;
                noda = grandparent;
            }
            else{
                if(noda->parent->right == noda){
                    noda = noda->parent;
                    leftR(&noda);
                    if (noda->parent == NULL) *root =noda;
                    noda = noda->left;
                }                
                grandparent = noda->parent->parent;
                grandparent->color = RED;
                noda->parent->color = BLACK;

                rightR(&grandparent);
                if (grandparent -> parent == NULL) *root = grandparent; 
            }
        }   
        else{
            uncle = grandparent->left;
            if (uncle && uncle->color == RED){
                grandparent->color = RED;
                uncle->color = noda->parent->color =BLACK;
                noda = grandparent;
            }
            else{
                if(noda->parent->left == noda){
                    noda = noda->parent;
                    rightR(&noda);
                    if (noda->parent == NULL) *root =noda;
                    noda = noda->right;
                }
                grandparent = noda->parent->parent;
                grandparent->color = RED;
                noda->parent->color = BLACK;

                leftR(&grandparent);
                if (grandparent -> parent == NULL) *root = grandparent; 
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
        if (uporedi(tr->vozilo, noda->vozilo)) tr = tr->left;
        else tr = tr->right;
    }

    noda->parent = pret;
    if (pret == NULL) *root = noda;
    else if(uporedi(pret->vozilo, noda->vozilo)) pret->left = noda;
    else pret->right = noda;
    
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
    printf ( "%d %s %f\n", root->vozilo.id, root->vozilo.tip, uda(root->vozilo));
    structure ( root->left, level + 1 );
  }
}
void izbrisi(Node **root, Node *noda, float udaljenost, char tip[]){
    if (noda == NULL) return;

    Node *left = noda->left;
    Node *right = noda->right;

    if (uda(noda->vozilo)<= udaljenost && strcmp(tip, noda->vozilo.tip) == 0){
        noda->left = noda->right = noda->parent = NULL;
        dodaj(root, noda);
    }
    else 
        free(noda);

    izbrisi(root, left, udaljenost, tip);
    izbrisi(root, right, udaljenost, tip);
}

int main(){
    Node *root = NULL;
    int n, id, x, y;
    char tip[50];
    scanf("%d", &n);
    while(n--){
        scanf("%d %d %d %s", &id, &x, &y, tip);
        dodaj(&root, alociraj(id,x,y,tip));
    }
    structure(root, 0);
    
    float udaljenost;
    scanf("%f %s", &udaljenost, tip);
    printf("====================================\n");
    Node *noviRoot = NULL;
    izbrisi(&noviRoot, root, udaljenost, tip);
    structure(noviRoot,0);

    Node *pomoc = noviRoot;
    while(pomoc->left) pomoc = pomoc->left;
    printf("%d", pomoc->vozilo.id);

}