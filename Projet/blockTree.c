#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "blockchain.h"
#include "blockTree.h"

CellTree *create_node(Block *b) {
    CellTree *new = (CellTree *)malloc(sizeof(CellTree));
    new->block = b;
    new->father = NULL;
    new->firstChild = NULL;
    new->nextBro = NULL;
    new->height = 0;
    return new;
}

int update_height(CellTree *father, CellTree *child)    {
    
    if ((father == NULL)||(child == NULL))  {
        fprintf(stderr,"Error : update_height : father or child null\n");
    }
    if (child->father != father)	{
        fprintf(stderr,"Error: update_height, You are NOT the father\n");
    }
    if (father->height < child->height+1){
        father->height = child->height+1;
        return 1;
    }else {
        //on ne modifie pas le père
        return 0;
    }
}

void add_child(CellTree *father, CellTree *child)   {
    if ((father == NULL)||(child == NULL))  {
        fprintf(stderr,"Error : add_child : father or child null\n");
        return;
    }
    //On actualise le pere de child
    child->father = father;
    //on ajoute le fils
    CellTree *curr = father->firstChild;
    if (curr == NULL)   {
        father->firstChild = child;
    }else{
        //on cherche le dernier des frères du fils du père
        while (curr->nextBro)   {
            curr = curr->nextBro;
        }
        curr->nextBro = child;
    }

    //on met à jour la hauteur des pères tant qu'il y a des modifications
    CellTree *fathers = father;
    CellTree *children = child;
    int modification = 1;
    while ((fathers)&&(modification==1))    {
        modification = update_height(fathers,children);
        children = fathers;
        fathers = fathers->father;
    }  
}

void print_tree(CellTree *tree) {
    if (!tree)  {
        return;
    }
    //on affiche le noeud courrant
    printf("Block de hauteur : %d, et de d'identifiant : %s\n", tree->height, tree->block->hash);
    
    //on appelle la fonction pour ses frères puis ses fils
    if (tree->nextBro){
        print_tree(tree->nextBro);
    }
    if (tree->firstChild){
        print_tree(tree->firstChild);
    }
}

void delete_node(CellTree *node)    {
    delete_block(node->block);
    free(node);
}

void delete_tree(CellTree *tree){
    if (tree){
        CellTree *brothers = tree->nextBro;
        CellTree *children =  tree->firstChild;

        //on supprime le noeud courant 
        delete_node(tree);

        //on supprime ses frères puis ses fils
        delete_tree(brothers);
        delete_tree(children);
    }
}

CellTree* highest_child(CellTree* cell){
    //retourne l'adresse du fils dont la hauteur est la plus grande
    if (cell==NULL)  {
        fprintf(stderr,"Error : highest_child : cell null\n");
        return;
    }
    CellTree* child = cell->firstChild;
    CellTree* highest_child = cell->firstChild;
    while(child){
        if (child->height > highest_child->height){
            highest_child = child;
        }
        child = child->nextBro;
    }
    return highest_child;
} 

CellTree *last_node(CellTree *tree) {
    //Retourne la feuille de la plus longue branche
    if (tree == NULL)   {
        fprintf(stderr, "Erreur : last_node, tree NULL\n");
    }
    //renvoie une feuille
    if (tree->firstChild == NULL)   {
        return tree;
    //parcourt le plus grand fils    
    } else {
        last_node(highest_child(tree));
    }
}

CellProtected *votesBrancheMax(CellTree *tree)  {
    //on fusionne les listes de votes de la plus longue branche
    if (tree == NULL)   {
        fprintf(stderr, "Erreur : votesBrancheMax, tree NULL\n");
    }
    //renvoie une feuille
    if (tree->firstChild == NULL)   {
        return copie_list_protected(tree->block->votes);
    //parcourt le plus grand fils    
    } else {
        return fusionner_list_protected(copie_list_protected(tree->block->votes),votesBrancheMax(highest_child));
    }
}


