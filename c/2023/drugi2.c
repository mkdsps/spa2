#include <stdio.h>
#include <stdlib.h>

enum {RED, BLACK};

typedef struct Node{
    int capital, number , productID, color;
    struct Node *left, *right, *parent;        
} Node;


Node *alociraj(int capital, int number, int productID){
    Node *root = (Node *)malloc(sizeof(Node));
    root->left = root->right = root->parent = NULL;
    root->color = RED;

    root->capital = capital;
    root->number = number;
    root->productID = productID;

    return root;
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
    if (r->left) r->left ->parent = r;

    l->parent= r->parent;
    if (r->parent){
        if(r->parent->left) r->parent->left = l;
        else r->parent->right = l;
    }

    l->right = r;
    r->parent = l;
    *root = l;
}

void fixUP(Node **root, Node *node){
    Node *grandparent, *uncle;
    while (node->parent && node->parent ->color == RED){
        grandparent = node->parent->parent;
        if (grandparent->left == node->parent){
            uncle = grandparent->right;
            if (uncle && uncle -> color == RED){
                node->parent->color = uncle->color = BLACK;
                grandparent->color = RED;        

                node = grandparent; 
            }
            else{
                if (node ->parent->right == node){
                    node = node->parent;
                    leftR(&node);
                    if (node -> parent == NULL) *root = node;

                    node = node->left;
                }
                grandparent = node->parent->parent;
                grandparent -> color = RED;
                node->parent-> color = BLACK;

                rightR(&grandparent);
                if (grandparent->parent == NULL) *root = grandparent;  
            }
        }
        else{
            uncle = grandparent->left;
            if (uncle && uncle -> color == RED){
                node->parent->color = uncle->color = BLACK;
                grandparent->color = RED;        

                node = grandparent; 
            }
            else{
                if (node ->parent->left == node){
                    node = node->parent;
                    rightR(&node);
                    if (node -> parent == NULL) *root = node;

                    node = node->right;
                }
                grandparent = node->parent->parent;
                grandparent -> color = RED;
                node->parent-> color = BLACK;

                leftR(&grandparent);
                if (grandparent->parent == NULL) *root = grandparent;  
            }
        }
        
    }
    (*root)->color = BLACK;
}

int veci(Node *a, Node *b){
    return a->capital > b->capital || (a->capital == b->capital && a->number > b->number); // pot greska
}

void dodaj(Node **root, Node *noda){
    Node *tr = *root, *pret = NULL;

    while (tr)
    {
        pret = tr;
        if (veci(tr,noda)) tr = tr->left;
        else tr = tr->right;
    }

    noda->parent = pret;
    if(pret == NULL) *root = noda;
    else if(veci(pret, noda)) pret->left = noda;
    else pret->right = noda;
    
    //fixUP(root, noda);
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
    printf ( "%d %d %d\n", root->capital, root->number, root->productID);
    structure ( root->left, level + 1 );
  }
}

Node *form(){
    Node *root = NULL;

    int cap, num, id, n;
    scanf("%d", &n); 
    while (n--){
        scanf("%d %d %d", &cap, &num, &id);
        dodaj(&root,alociraj(cap,num,id));
    }
    return root;
}

int kriterijum(Node *noda, int cene[]){
    return noda->capital >= cene[noda->productID]*noda->number; 
}

void izbrisi(Node **newRoot, Node *noda, int cene[]){
    if (noda == NULL) return;

    Node *l = noda->left, *r = noda->right;

    if (kriterijum(noda, cene)){
        noda->left = noda->right = noda->parent = NULL;
        dodaj(newRoot, noda);
    }
    else 
        free(noda); 

    izbrisi(newRoot, l, cene); 
    izbrisi(newRoot, r, cene); 
}

int main(){
    Node *root = form();
    structure(root, 0);

    int m, cene[100];

    scanf("%d", &m);
    for (int i = 0 ;i  < m ; i++ )
        scanf("%d", &cene[i]);

    Node *newRoot = NULL;
    izbrisi(&newRoot, root, cene);
    structure(newRoot, 0);

    int x, suma = 0, brojac = 0;
    scanf("%d", &x);

    
}