#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <wchar.h>
#include <stdbool.h>
#include <memory.h>
#include <ctype.h>
#include <locale.h>

#ifndef PROJET_TP4_H
#define PROJET_TP4_H

typedef struct ListeVaccins {
    char* marque;
    unsigned int nombre_vaccins;
    struct ListeVaccins* suivant;
} T_ListeVaccins;

typedef struct ABR {
    char* date;
    T_ListeVaccins* liste_vaccins;
    struct ABR* fils_gauche;
    struct ABR* fils_droite;
} T_ABR;

T_ABR* initialiserInstance (T_ABR* abr);

T_ABR* initialiserInstanceFch (T_ABR* abr);

int traiterEntree (bool drapo, char dest[100]);

void afficherMenu ();

T_ListeVaccins* creerNoeudVaccin (char* marque, unsigned int nombre_vaccins);

T_ABR* creerNoeudABR (char* date, char* marque, unsigned int nombre_vaccins);

void afficherStockL (T_ListeVaccins* listeVaccins);

void afficherStockA (T_ABR* abr);

unsigned int compterVaccins (T_ABR* abr, char* marque);

void deduireVaccinL (T_ListeVaccins** listeVaccins, char* marque, unsigned int nb_vaccins);

void libererRessrc (T_ABR* abr);

void libererVaccins (T_ListeVaccins* listeVaccins);

void ajouterVaccinL (T_ListeVaccins** listeVaccins, char* marque, unsigned int nb_vaccins);

void ajouterVaccinA (T_ABR** abr, char* date, char* marque, unsigned int nb_vaccins);

void deduireVaccinA (T_ABR** abr, char* marque, unsigned int nb_vaccins);

void afficherDates (T_ABR* abr);

void afficherDateMarques (T_ABR* abr, char* date);

bool verifierDate (char* date);

void enregistrerMarques (T_ABR* abr, char*** mks, int* nb_mks);

bool verifierMarque (char* marque, char** mks, int nb_mks);

void desallouerDate (T_ABR** abr);

void desallouerVaccin (T_ListeVaccins** vaccin);

T_ABR* smallerDroite (T_ABR* abr);

void reformerArbre (T_ABR** abr, char date[]);

#endif //PROJET_TP4_H