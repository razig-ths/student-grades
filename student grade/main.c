#include<stdbool.h>
#include<stdio.h>
//#include<conio.h>
#include<string.h>
#include<stdlib.h>
#include<limits.h>
#include<time.h>
#include<ctype.h>
#include<stdarg.h>
//#include<windows.h>


#define MAXCHAR 1000
#define MAXCharNames 20
#define MAXmsgCHAR 80

typedef unsigned short int usint_t;

int isNumeric(const char * s);
void delay(double number_of_seconds);
void clrscr();

typedef struct etudiant{
	char nom[20] ;
	char prenom[20] ;
	float note;
}Etudiant ;

typedef struct Noeud{
    Etudiant etudiant ;
	struct Noeud *fgauche;
	struct Noeud *fdroit;
}Noeud ;

typedef struct note{
	Noeud *racine;
	int ne;
}note;

void creer(note *A){
	A->ne =0;
	A->racine = NULL;
}

Noeud *creerNoeud(Etudiant n , Noeud *gauche, Noeud *droit){
	Noeud *p=(Noeud* )malloc(sizeof(Noeud));
	p->etudiant=n;
	p->fgauche=gauche;
	p->fdroit=droit;
	return p;
}

int feuille(Noeud *A){
	return (A->fdroit == NULL)&&(A->fgauche == NULL);
}

float noteEtudiant(Noeud *n){
	return n->etudiant.note;
}

void insererEtudiant(Etudiant x,Noeud **Abr){

	if(*Abr==NULL){
     *Abr=creerNoeud(x,NULL,NULL);
	}else{
		if(noteEtudiant(*Abr)>x.note){
			insererEtudiant(x,&(*Abr)->fgauche);
		}else{
			insererEtudiant(x,&(*Abr)->fdroit);
		}
	}
}

void inserer(note *A){

	    Etudiant e;

	    printf("\t# Nom	:");
		scanf("%s", e.nom);
		printf("\t# Prenom :");
		scanf("%s", e.prenom);
		printf("\t# Note  :");
		scanf("%f", &e.note);
		if((e.note<0) || ( e.note>20)){
            printf("donnez un autre note entre 0 et 20\n");
            printf("\t# Note  :");
            scanf("%f",&e.note);
        }
		insererEtudiant(e, &A->racine);
		A->ne++;
}

Noeud *chercher(Noeud *A,char nom[],char prenom[]){
    if (A != NULL) {
        if (( strcmp(A->etudiant.nom, nom) == 0) && ( strcmp(A->etudiant.prenom ,prenom) == 0) ){
			return A;
        }
        else{
           Noeud * p = chercher(A->fgauche, nom,prenom);
             if (p == NULL){
                return  chercher(A->fdroit , nom, prenom);
             }else{
               return p;
             }
        }
    }else{
      return NULL;
	}
}

///

void supprimer(char nom[], char prenom [], Noeud **A){
	Noeud *P = NULL;
	Noeud *Q = *A;
	if ((*A) == NULL){
		return;
	}
        if ((strcmp((*A)->etudiant.nom,nom) == 0) && (strcmp((*A)->etudiant.prenom, prenom)==0)){
		    if(feuille((*A))){
			   free(*A);
			   *A = NULL;
		    }else if ((*A)->fgauche == NULL){
			         (*A) = (*A)->fdroit;
                    free(Q);
                }else if ((*A)->fdroit == NULL) {
			        (*A) = (*A)->fgauche;
			        free(Q);
                      }else{
			            P = Q;
			            Q = Q ->fdroit;
			           while(Q->fgauche != NULL){
				             P = Q;
				             Q= Q->fgauche;
			           }
			           (*A)->etudiant = Q ->etudiant;
			             if((*A) == P){
				             supprimer(Q->etudiant.nom,Q->etudiant.prenom,&(*A)->fdroit);
                         }else{
				             supprimer(Q->etudiant.nom,Q->etudiant.prenom,&(P)->fgauche);
			             }
		               }
	    }else{
		   supprimer(nom,prenom,&(*A)->fgauche);
		   supprimer(nom,prenom,&(*A)->fdroit);
	    }
}

void supprimerEtudiant(note *A){
	char nom[20],prenom[20];
	if(A->racine == NULL){
		printf("la supprision Impossible :la arbre est vide\n");
	}else{
        printf("donner le nom :" );
        scanf("%s",nom);
        printf("donner le prenom :" );
        scanf("%s",prenom);
        if(chercher(A->racine,nom,prenom) !=NULL){
			 supprimer(nom,prenom,&A->racine);
			 A->ne--;
		 }else{
            printf("l'etudiant n'existe pas\n");
		 }
	}
}

void modifier(note *A){
		char nom[20];
		char prenom[20];
		if(A->racine  == NULL){
			printf("Erreur :la arbre est vide\n");
		}else{
			 printf("donner le nom :" );
             scanf("%s",nom);
             printf("donner le prenom :" );
             scanf("%s",prenom);

			 Noeud *P = chercher(A->racine,nom,prenom);
			 if (P == NULL){
				printf("l'etudiant n'existe pas \n");
			 }else {
			    Etudiant E = P->etudiant;
                float newNote;
                printf(" la nouvelle note : ");
                scanf("%f",&newNote);
                if(newNote<0 || newNote>20){
                    printf("donnez un autre note entre 0 et 20\n");
                    printf("la nouvelle note:");
                    scanf("%f",&newNote);
                }
                E.note = newNote;
			    supprimer(P->etudiant.nom,P->etudiant.prenom,&A->racine);
			    insererEtudiant(E,&A->racine);
			}
		 }
}

int nombreEtudiant(note A){
   return A.ne;
}

void afficher(Noeud *A){
    printf("\n");
    printf("Nom : %s\n",A->etudiant.nom);
    printf("Prenom : %s\n",A->etudiant.prenom);
    printf("Note : %f\n",A->etudiant.note);
    printf("\n");
}

void listEtudiant(Noeud *A){
    if(A != NULL){
        listEtudiant(A->fgauche);
        afficher(A);
        listEtudiant(A->fdroit);
    }
}

Noeud *Min(Noeud *A){
	if(A == NULL){
		return NULL;
	}else{
		while(A->fgauche != NULL){
			A = A->fgauche;
		}
		return A;
	}
}

Noeud *Max(Noeud *A){
	if(A == NULL){
		return NULL;
	}else{
       while(A->fdroit != NULL){
			A = A->fdroit;
       }
		return A;
	}
}

float  movaiseNote(Noeud *A){
	Noeud *P = Min(A);
	while(A != NULL){
		if(A->etudiant.note == P->etudiant.note){
			afficher(A);
		}
		A=A->fgauche;
	}
}

float  meilleureNote(Noeud *A){
	Noeud *P = Max(A);
	Noeud * Q;
	if (P!= NULL){
		afficher(P);
		Q = P->fgauche;
		while(Q != NULL){
			if (Q->etudiant.note == P->etudiant.note){
				afficher(Q);
				Q=Q->fgauche;
			}else{
				Q = Q->fdroit;
			}
		}
	}
}

void admis(Noeud *A){
	if( A != NULL){
		admis(A->fdroit);
       if(noteEtudiant(A)>=10){
		   afficher(A);
	   }
	   admis(A->fgauche);

	}
}
void red () {
  printf("\033[1;31m");
}
void main(){
    printf("\n");

    printf("      --- Welcome to my TP ---  ");
	note A;
	creer(&A);
	int n, nb ;
	int h;
	do{
	 red();
         printf("\n");
         printf("\n");
         printf("\n");
         printf(" >> pour saisir un etudiant taper   1\n\n");
	 printf(" >> pour supprimer la note d'un etudiant taper   2 \n\n");
         printf(" >> pour modifier la note d'un etudiant taper   3 \n\n");
	 printf(" >> pour afficher le nomber des etudiant taper   4 \n\n");
         printf(" >> pour afficher la liste des etudiant taper   5 \n\n");
         printf(" >> pour afficher les etudiants qui ont obtenus la meilleure note taper   6 \n\n");
         printf(" >> pour afficher les etudiants qui ont obtenus la mauvaise note taper   7 \n\n");
         printf(" >> pour les etudiant admis dans ordre decroissant taper   8 \n\n");
         printf(" >> pour Exit taper   0 \n\n");
         printf("\n");
         printf("\n");
         printf ("Entrer votre choix :  \n\n ");
         scanf("%d",&h);
         switch (h){
		 case 1 :
			     printf("enter nombre des etudiant :");
                 scanf("%d",&nb);
                 int i =1;
                 for (i = 1;i<=nb ; i++){
                 printf("Etudiant",&i) ;
                 printf("\n");
                    inserer(&A);
                    printf("\n");
                 }break;
		case 2 :
		        supprimerEtudiant(&A);break;
		case 3 :
                modifier(&A);break;
		case 4 :
		        printf(" le nombre ds etudiants : %d \n ", nombreEtudiant(A));break;
		case 5 :
		        listEtudiant(A.racine);break;
		case 6 :
		        meilleureNote(A.racine);break;
		case 7 :
		        movaiseNote(A.racine);break;
		case 8 :
		        admis(A.racine);break;
                case 0 :exit(0);break;
               
                
                
                break;
        }
system("pause");

   }while (h!=0);
}
