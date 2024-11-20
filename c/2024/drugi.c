#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {RED, BLACK};

typedef struct{
    int id, brojProdatihKarata;
    char datum[10];
} Projekcija;

typedef struct node{
    struct node *left, *right, *parent;
    int boja;
    Projekcija projekcija;
}node; 

void padding ( char ch, int n )
{
  int i;
  for ( i = 0; i < n; i++ )
    putchar ( ch );
}
void structure (  node *root, int level )
{
  int i;
  if ( root == NULL ) {
    padding ( '\t', level );
    puts ( "~" );
  }
  else {
    structure ( root->right, level + 1 );
    padding ( '\t', level );
    //printf ( "%s \n", root->projekcija.datum);
    printf ( "%s %d %d \n", root->projekcija.datum, root->projekcija.id, root->projekcija.brojProdatihKarata);
    structure ( root->left, level + 1 );
  }
}


node *alociraj(int id, int brojProdatihKarata, char datum[]){
    node *res = (node *)malloc(sizeof(node));
    res->left = NULL;
    res->right = NULL;
    res->parent = NULL;
    res->boja = RED;

    res->projekcija.id = id;
    res->projekcija.brojProdatihKarata = brojProdatihKarata;
    strcpy(res->projekcija.datum, datum);   

    return res;
}

void leftR(node **root){
    node *l = *root;
    node *r = l->right;

    l->right = r->left;
    if (l->right) l->right->parent = l;

    r->parent = l->parent;
    if (l->parent){
        if(l->parent->left == l) l->parent->left = r;
        else l->parent->right = r;
    }

    r->left = l;
    l->parent = r;

    *root = r;        
}

void rightR(node **root){
    node *r = *root;
    node *l = r->left;

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

void fixUP(node **root, node *noda){
    node *uncle, *grandparent;

    while (noda->parent && noda->parent->boja == RED){
        grandparent = noda->parent->parent;
        if (grandparent->left == noda->parent){// roditelj je levo dete
            uncle = grandparent->right;
            if (uncle && uncle->boja == RED){
                grandparent->boja = RED;
                uncle->boja = BLACK;
                noda->parent->boja = BLACK;

                noda = grandparent;        
            }
            else{
                if (noda == noda->parent->right){
                    noda = noda->parent;
                    leftR(&noda);
                    if (noda->parent == NULL) *root = noda;

                    noda = noda->left;
                }        
                grandparent = noda->parent->parent;

                grandparent->boja = RED;
                noda->parent->boja = BLACK;  
                rightR(&grandparent);

                if (grandparent->parent == NULL) *root = grandparent;
            }

        }
        else{
            uncle = grandparent->left;
            if (uncle && uncle->boja == RED){
                grandparent->boja = RED;
                uncle->boja = BLACK;
                noda->parent->boja = BLACK;

                noda = grandparent;       
            }
            else{
                if (noda == noda->parent->left){
                    noda = noda->parent;
                    rightR(&noda);
                    if (noda->parent == NULL) *root = noda;

                    noda = noda->right;
                }        
                grandparent = noda->parent->parent;

                grandparent->boja = RED;
                noda->parent->boja = BLACK;

                leftR(&grandparent);
                if(grandparent->parent == NULL) *root = grandparent;
            }
        }
    }
    (*root)->boja = BLACK;
}

int veci(Projekcija p1, Projekcija p2){
    int a = strcmp(p1.datum,p2.datum);  
    if (a > 0 ) return 1;
    if(a < 0 ) return 0;

    if(p1.id > p2.id) return 1;
    return 0;
}

void dodaj(node **root, node *noda){
    node *tr = *root, *pret = NULL;

    while(tr){
        pret = tr;
        if (veci(tr->projekcija, noda->projekcija)) tr = tr->left;
        else tr = tr->right;
    } 

    noda->parent = pret;
    if (pret == NULL) *root = noda;
    else if (veci(pret->projekcija, noda->projekcija)) pret->left = noda;
    else pret->right = noda;

    fixUP(root, noda);
}

node *formiraj(){
    node *root = NULL;

    int n;
    scanf("%d", &n);

    int id;
    int k;
    char datum[22];

    while(n--){
        scanf("%d", &id);
        scanf("%d", &k);
        scanf("%s",  datum);

        //printf("%d %d %s", id, k, datum);
        dodaj(&root, alociraj(id,k,datum));
    }    

    return root;
}

void izbrisi(node **root, node *noda, int thr){
    if (noda == NULL) return;
    node *levo = noda->left, *desno = noda->right;


    if (strcmp(noda->projekcija.datum,"2023/11/19") < 0 || noda->projekcija.brojProdatihKarata>=thr) {
        noda->left = NULL;
        noda->right = NULL;
        noda->parent = NULL;         
        dodaj(root, noda);
    }
    else
        free(noda);
    
    izbrisi(root, levo, thr);
    izbrisi(root, desno, thr);

}

int main(){
    node *root = formiraj();
    structure(root, 0);
    node *rootNovi = NULL;

    printf("======================================\n");

    int k;
    scanf("%d", &k );
    izbrisi(&rootNovi, root, k); 
    structure(rootNovi, 0); 
}