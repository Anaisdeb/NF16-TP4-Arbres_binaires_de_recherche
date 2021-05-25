#include "tp4.h"

//initialiser le programme depuis un fichier
T_ABR* initialiserInstanceFch (T_ABR* abr) {
    printf( "NOM DE FICHIER « .txt » : " );
    char filename[100];
    traiterEntree( false, filename );
    strcat( filename, ".txt" );
    printf( "ATTENTION : NOM DE FICHIER : « %s »\n", filename );
    FILE* pf = fopen( filename, "rt" );
    if ( pf == NULL ) {
        printf( "ERREUR : Ouverture « %s » échouée !\n", filename );
        return NULL;
    }
    for ( int i = 0 ; i < 3 ; ++i ) {
        char descript1[30], descript2[30];
        fscanf( pf, "// %s %s\n", descript1, descript2 );
    }
    int nb_dates;
    fscanf( pf, "%d\n", &nb_dates );
    for ( int i = 0 ; i < nb_dates ; ++i ) {
        char date[100], marque[100];
        int nb_mk;
        unsigned int nb_vaccins;
        fscanf( pf, "%s %d\n", date, &nb_mk );
        for ( int j = 0 ; j < nb_mk ; ++j ) {
            fscanf( pf, "%s %u\n", marque, &nb_vaccins );
            ajouterVaccinA( &abr, date, marque, nb_vaccins );
        }
    }
    fclose( pf );
    printf( "SUCCÈS : %d dates de livraison ont été lues depuis « %s ».\nSUCCÈS : Instance créée !\n",
            nb_dates, filename );
    return abr;
}

//afficher toutes les dates de l'ABR
void afficherDates (T_ABR* abr) {
    if ( abr == NULL )
        return;
    afficherDates( abr->fils_gauche );
    printf( "%s, ", abr->date );
    afficherDates( abr->fils_droite );
}

void afficherStockL (T_ListeVaccins* listeVaccins) {
    if ( listeVaccins == NULL )
        return;
    printf( "   --- %s : %u [Marque : Quantité]\n", listeVaccins->marque, listeVaccins->nombre_vaccins );
    afficherStockL( listeVaccins->suivant );
}

// Parcours infixe pour l’affichage : permet d’obtenir un ordre croissant */
void afficherStockA (T_ABR* abr) {
    if ( abr == NULL )
        return;
    afficherStockA( abr->fils_gauche );
    printf( " %s [Date] :\n", abr->date );
    afficherStockL( abr->liste_vaccins );
    afficherStockA( abr->fils_droite );
}

//afficher toutes les marques d'une date
void afficherDateMarques (T_ABR* abr, char* date) {
    if ( abr == NULL )
        return;
    afficherDateMarques( abr->fils_gauche, date );
    if ( strcmp( date, abr->date ) == 0 ) {
        T_ListeVaccins* pVac = abr->liste_vaccins;
        while ( pVac != NULL ) {
            printf( "%s, ", pVac->marque );
            pVac = pVac->suivant;
        }
    return;
    }
    afficherDateMarques( abr->fils_droite, date );
}
//contrôler le format de la Date entrée
//retourner true une fois le format non respecté
bool verifierDate (char* date) {
    /* vérifie le format "AAAA-MM-JJ" */
    for ( int i = 0 ; i < 10 ; ++i ) {
        if ( date[i] == '-' && (i == 4 || i == 7) )
            continue;
        if ( !isdigit( date[i] ) ) //isdigit retourne 0 si le caractère passé n'est pas numérique
            return true; //format non respecté
    }
    /* vérifie le format "AAAA-MM-JJ" */
    char* end_ptr = NULL;
    //strlo : String To Long
    int annee = strtol( date, &end_ptr, 10 ); //convertit l'année en base 10 et end_ptr pointe sur le reste
    int cldr[12][2] = { { 1,  31 },
                        { 2,  28 },
                        { 3,  31 },
                        { 4,  30 },
                        { 5,  31 },
                        { 6,  30 },
                        { 7,  31 },
                        { 8,  31 },
                        { 9,  30 },
                        { 10, 31 },
                        { 11, 30 },
                        { 12, 31 } };
    int mois = strtol( end_ptr+1, &end_ptr, 10 ); //convertit le mois en base 10 et end_ptr pointe sur le reste
    int jour = strtol( end_ptr+1, NULL, 10 ); //convertit le jour en base 10 et end_ptr pointe sur le reste
    if ( annee%400 == 0 || (annee%4 == 0 && annee%100 != 0) )
        cldr[1][1]++; //année bissextile : 366 jours
    if ( !(mois >= 1 && mois <= 12) || (!(jour >= 1 && jour <= cldr[mois-1][1])) )
        return true; //format non respecté
    return false;
}

unsigned int compterVaccins (T_ABR* abr, char* marque) {
    if ( abr == NULL || abr->liste_vaccins == NULL )
        return 0;
    unsigned int nb = compterVaccins( abr->fils_droite, marque );
    nb += compterVaccins( abr->fils_gauche, marque );
    T_ListeVaccins* pVaccin = abr->liste_vaccins;
    while ( pVaccin != NULL ) {
        if ( strcmp( marque, pVaccin->marque ) == 0 )
            nb += pVaccin->nombre_vaccins;
        pVaccin = pVaccin->suivant;
    }
    return nb;
}

void libererRessrc (T_ABR* abr) {
    if ( abr == NULL )
        return;
    libererRessrc( abr->fils_droite );
    libererRessrc( abr->fils_gauche );
    if ( abr->date != NULL ) {
        free( abr->date );
    }
    libererVaccins( abr->liste_vaccins );
    free( abr );
    abr = NULL;
}

void libererVaccins (T_ListeVaccins* listeVaccins) {
    if ( listeVaccins == NULL )
        return;
    libererVaccins( listeVaccins->suivant );
    if ( listeVaccins->marque != NULL ) {
        free( listeVaccins->marque );
    }
    free( listeVaccins );
}

//créer un nœud de l'arbre
T_ABR* creerNoeudABR (char* date, char* marque, unsigned int nombre_vaccins) {
    T_ABR* newABR = ( T_ABR* ) malloc( sizeof( T_ABR ) );
    if ( newABR == NULL )
        exit( EXIT_FAILURE );
    newABR->liste_vaccins = newABR->fils_gauche = newABR->fils_droite = NULL;
    newABR->date = strdup( date );
    ajouterVaccinL( &(newABR->liste_vaccins), marque, nombre_vaccins );
    return newABR;
}

//créer un élément de la liste chaînée
T_ListeVaccins* creerNoeudVaccin (char* marque, unsigned int nombre_vaccins) {
    T_ListeVaccins* newVaccin = ( T_ListeVaccins* ) malloc( sizeof( T_ListeVaccins ) );
    if ( newVaccin == NULL )
        exit( EXIT_FAILURE );
    newVaccin->suivant = NULL;
    newVaccin->nombre_vaccins = nombre_vaccins;
    newVaccin->marque = strdup( marque );
    return newVaccin;
}

inline void afficherMenu () {
    printf( "==================================== MENU ====================================\n"
            "1. Initialiser un ABR.\n"
            "2. Ajouter un nombre de vaccins d'une marque dans un ABR par une date indiquee.\n"
            "3. Afficher tous les stocks disponibles dans un ABR.\n"
            "4. Compter le nombre de vaccins disponible pour une marque indiquee.\n"
            "5. Deduire un nombre de vaccins d'une marque indiquee dans un ABR.\n"
            "6. Quitter.\n"
            "==============================================================================\nCHOIX : " );
}

/*si drapo==true, retourner une valeur comme valeur renvoyée
si drapo==false, retourner une chaîne de caractères (dest) et -1*/
int traiterEntree (bool drapo, char dest[100]) {
    char entree[100];
    fflush( stdin );
    scanf( "%s", entree );
    if ( drapo == true ) {
        //Entrées traitées comme une valeur
        int ret = strtol( entree, NULL, 10 );
        printf( "SAISIE : \"%d\"\n", ret );
        return ret;
    } else {
        //Entrée traitée comme une chaîne de caractères
        //Conversion des caractères non alphabétiques (sauf '-') en points d'interrogation
        for ( char* carac = &entree[0] ; *carac != '\0' ; ++carac )
            if ( !(isalnum( *carac ) || *carac == '-') )
                *carac = '?';
        printf( "SAISIE : \"%s\"\n", entree );
        strcpy( dest, entree );
        return -1;
    }
}

//initialiser le programme selon l'entrée de l'utilisateur
T_ABR* initialiserInstance (T_ABR* abr) {
    printf( " - Nombre de dates : " );
    int nb_dates = traiterEntree( true, NULL );
    for ( int i = 0 ; i < nb_dates ; ++i ) {
        char date[100], marque[100];
        printf( " - Date n° %d \"AAAA-MM-JJ\" : ", 1+i );
        traiterEntree( false, date );
        printf( " --- Nombre de marques \"%s\" : ", date );
        int nb_mk = traiterEntree( true, NULL );
        if ( nb_mk <= 0 )
            continue;
        for ( int j = 0 ; j < nb_mk ; ++j ) {
            printf( " ------ Marque n° %d : ", j+1 );
            traiterEntree( false, marque );
            printf( " ------ Nombre de vaccins num %d : ", j+1 );
            int nb_vaccins = traiterEntree( true, NULL );
            ajouterVaccinA( &abr, date, marque, nb_vaccins );
        }
    }
    return abr;
}

void ajouterVaccinL (T_ListeVaccins** listeVaccins, char* marque, unsigned int nb_vaccins) {
    if ( nb_vaccins <= 0 ) {
        printf( "ERREUR : Entrée incorrecte !\nERREUR : Ajout échoué !\n" );
        return;
    }
    if ( listeVaccins == NULL ) {
        printf( "ERREUR FATALE !\n" );
        return;
    }
    if ( *listeVaccins == NULL ) {
        *listeVaccins = creerNoeudVaccin( marque, nb_vaccins );
        return;
    }
      //The mark already exists, just increase the quantity
    if ( strcmp( marque, (*listeVaccins)->marque ) == 0 ) {
        (*listeVaccins)->nombre_vaccins += nb_vaccins;
        return;
    } else //The mark doesn't exist, need to traverse to the tail of the linked list
        return ajouterVaccinL( &((*listeVaccins)->suivant), marque, nb_vaccins );
}

// Insert a new node with a given date into the BST , the tree can be empty .
void ajouterVaccinA (T_ABR** abr, char* date, char* marque, unsigned int nb_vaccins) {
    if ( nb_vaccins <= 0 || verifierDate( date ) == true ) {
        printf( "ERREUR : Entrée incorrecte !\nERREUR : Ajout échoué !\n" );
        return;
    }
      // If root is empty , that means the tree is empty or reach the proper node
    if ( *abr == NULL ) {
        *abr = creerNoeudABR( date, marque, nb_vaccins ); //la date n'existe pas encore
        return;
    }
      // If the value is bigger than the current node , then insert into its right side sub - tree .
    if ( strcmp( date, (*abr)->date ) > 0 )//le premier caractère qui ne correspond pas a une valeur plus grande dans ptr1 que dans ptr2
        ajouterVaccinA( &((*abr)->fils_droite), date, marque, nb_vaccins );
        // Otherwise , insert it into left side sub - tree .
    else if ( strcmp( date, (*abr)->date ) < 0 ) //le premier caractère qui ne correspond pas a une valeur inférieure dans ptr1 que dans ptr2
        ajouterVaccinA( &((*abr)->fils_gauche), date, marque, nb_vaccins );
    else
      // The node already exists, just add the mark and its quantity
        ajouterVaccinL( &((*abr)->liste_vaccins), marque, nb_vaccins ); //la date existe déjà
}

/*void deduireVaccinL (T_ListeVaccins** listeVaccins, char* marque, unsigned int* nb_vaccins) {
    if ( listeVaccins == NULL ) {
        printf( "ERREUR FATALE !\n" );
        return;
    }
    if ( *listeVaccins == NULL || *nb_vaccins <= 0 )
        return;
    // vaccin trouvé
    if ( strcmp( (*listeVaccins)->marque, marque ) == 0 ) {
        // la quantité à déduire < la quantité de cette marque
        if ( *nb_vaccins < (*listeVaccins)->nombre_vaccins ) {
            (*listeVaccins)->nombre_vaccins -= (*nb_vaccins);
            (*nb_vaccins) = 0;
            return;
        }
        // la quantité à déduire >= la quantité de cette marque
        (*nb_vaccins) -= (*listeVaccins)->nombre_vaccins;
        // cette marque se trouve en queue de la liste chaînée
        if ( (*listeVaccins)->suivant == NULL ) {
            desallouerVaccin( listeVaccins );
            return;
        }
        //cette marque ne se trouve pas en queue de la liste chaînée
        T_ListeVaccins* pVac = *listeVaccins;
        *listeVaccins = (*listeVaccins)->suivant;
        desallouerVaccin( &pVac );
        return;
    }
    //vaccin non trouvé
    deduireVaccinL( &((*listeVaccins)->suivant), marque, nb_vaccins );
}

void deduireVaccinA (T_ABR** abr, char* marque, unsigned int* nb_vaccins) {
    if ( *abr == NULL || *nb_vaccins <= 0 )
        return;
    deduireVaccinA( &((*abr)->fils_gauche), marque, nb_vaccins );
    if ( *nb_vaccins > 0 )
        deduireVaccinL( &((*abr)->liste_vaccins), marque, nb_vaccins );
    deduireVaccinA( &((*abr)->fils_droite), marque, nb_vaccins );
}*/

//Save all the marks in the tree to mks
void enregistrerMarques (T_ABR* abr, char*** mks, int* nb_mks) {
    if ( abr == NULL )
        return;
    T_ListeVaccins* pVac = abr->liste_vaccins;
    while ( pVac != NULL ) {
        if ( *mks == NULL ) {
            //aucune marque enregistrée
            *mks = ( char** ) malloc( 1*sizeof( char* ) );
            (*nb_mks)++;
            (*mks)[*nb_mks-1] = strdup( pVac->marque );
        } else {
            //marque(s) déjà enregistrée(s)
            int nbr_tour = *nb_mks;
            bool drp_mk = true;
            //déterminer si la marque a été enregistrée
            for ( int i = 0 ; i < nbr_tour ; i++ )
                if ( strcmp( (*mks)[i], pVac->marque ) == 0 )
                    drp_mk = false;
            //le cas où la marque n'a pas encore été enregistrée
            if ( drp_mk == true ) {
                *mks = ( char** ) realloc( (*mks), (1+*nb_mks)*sizeof( char* ) );
                (*nb_mks)++;
                (*mks)[*nb_mks-1] = strdup( pVac->marque );
            }
        }
        pVac = pVac->suivant;
    }
    enregistrerMarques( abr->fils_gauche, mks, nb_mks );
    enregistrerMarques( abr->fils_droite, mks, nb_mks );
}
//contrôler l'existence de la Marque entrée
//retourner true une fois la marque existante
bool verifierMarque (char* marque, char** mks, int nb_mks) {
    for ( int i = 0 ; i < nb_mks ; ++i )
        if ( strcmp( marque, mks[i] ) == 0 )
            return true;
    return false;
}

//libérer un noeud de l'arbre
void desallouerDate (T_ABR** abr) {
    // Release the resource allocated in this structure.
    if ( (*abr)->date != NULL )
        free( (*abr)->date );
        // Release this structure itself.
    free( *abr );
    *abr = NULL;
}

//libérer un élément de la liste chaînée
void desallouerVaccin (T_ListeVaccins** vaccin) {
    // Release the resource allocated in this structure.
    if ( (*vaccin)->marque != NULL )
        free( (*vaccin)->marque );
        // Release this structure itself.
    free( *vaccin );
    *vaccin = NULL;
}

T_ABR* smallerDroite (T_ABR* abr) {
    // If the tree is empty.
    if ( abr == NULL )
        return NULL;
        // Else if the smallest node is itself.
    if ( abr->fils_gauche == NULL )
        return abr;
        // Otherwise , pass handle to the left side sub - tree of current node .
    return smallerDroite( abr->fils_gauche );
}

// By giving a binary search tree and a value , this function deletes the corresponding .
//reformer l'arbre en supprimant une date qui n'a pas de vaccins
void reformerArbre (T_ABR** abr, char date[]) {
    if ( *abr == NULL )
        return;
        // If the value is bigger than the current node , then pass handle to its right side sub - tree .
    if ( strcmp( date, (*abr)->date ) > 0 ) {
        reformerArbre( &((*abr)->fils_droite), date );
        return;
    }
      // Otherwise , pass handle to left side sub - tree .
    if ( strcmp( date, (*abr)->date ) < 0 ) {
        reformerArbre( &((*abr)->fils_gauche), date );
        return;
    }
      //The node is found .
      //This node has no child.
    if ( (*abr)->fils_gauche == NULL && (*abr)->fils_droite == NULL ) {
        desallouerDate( abr );
        return;
    }
      // The node has no left child .
    if ( (*abr)->fils_gauche == NULL ) {
        T_ABR* ndSup = *abr;
        (*abr)->liste_vaccins = (*abr)->fils_droite->liste_vaccins;
        strcpy( (*abr)->date, (*abr)->fils_droite->date );
        *abr = (*abr)->fils_droite;
        ndSup->fils_gauche = NULL;
        ndSup->fils_droite = NULL;
        desallouerDate( &ndSup );
        return;
    }
      // The node has no right child .
    if ( (*abr)->fils_droite == NULL ) {
        T_ABR* ndSup = *abr;
        (*abr)->liste_vaccins = (*abr)->fils_gauche->liste_vaccins;
        strcpy( (*abr)->date, (*abr)->fils_gauche->date );
        *abr = (*abr)->fils_gauche;
        ndSup->fils_gauche = NULL;
        ndSup->fils_droite = NULL;
        desallouerDate( &ndSup );
        return;
    }
      // Node has two children .
      // Find the smallest value node at right side sub - tree .
    T_ABR* ndSml = smallerDroite( (*abr)->fils_droite );
      // Copy the information of the smallest value node to the node itself (the node to be deleted).
    strcpy( (*abr)->date, ndSml->date );
    (*abr)->liste_vaccins = ndSml->liste_vaccins;
      //The copy (deletion) is complete, we need to delete the smallest value node.
    reformerArbre( &((*abr)->fils_droite), ndSml->date );
}

/*
void deduireVaccinA (T_ABR** abr, char* marque, unsigned int nb_vaccins) {

    if (*abr == NULL || nb_vaccins <= 0)
        return;

    deduireVaccinA (&((*abr)->fils_gauche), marque, nb_vaccins);

    T_ListeVaccins* header = (*abr)->liste_vaccins;
    int avant=0, apres=0;
    while(header != NULL){
        avant += header->nombre_vaccins;
        header=header->suivant;
    }
    printf("avant : %d \n", nb_vaccins);
    deduireVaccinL(&((*abr)->liste_vaccins), marque, nb_vaccins);
    header = (*abr)->liste_vaccins;
    while(header != NULL){
        apres += header->nombre_vaccins;
        header=header->suivant;
    }
    printf("diff : %d \n ", avant-apres);
    nb_vaccins -= (avant-apres);
    printf("après : %d \n", nb_vaccins);

    deduireVaccinA (&((*abr)->fils_droite), marque, nb_vaccins);
}
*/

// On déduit avec un parcours dans l'ordre infixe pour respecter l'ordre croissant des dates

void deduireVaccinA (T_ABR** abr, char* marque, unsigned int nb_vaccins) {
    if ( *abr == NULL || nb_vaccins <= 0 )
        return;

    //L'objectif de la déclaration de nb_x est de vérifier si la déduction a été effectuée.
    //Cette fonction prend fin dès que la déduction est effectuée.
    unsigned int nb_1 = compterVaccins( *abr, marque );
    deduireVaccinA( &((*abr)->fils_gauche), marque, nb_vaccins );

    unsigned int nb_2 = compterVaccins( *abr, marque );
    if ( nb_2 != nb_1 )
        return;

    deduireVaccinL( &((*abr)->liste_vaccins), marque, nb_vaccins );
    // Si un noeud ne possède aucun vaccin, le supprimer
    if ( (*abr)->liste_vaccins == NULL )
        reformerArbre( abr, (*abr)->date );

    if ( *abr == NULL )
        return;

    unsigned int nb_3 = compterVaccins( *abr, marque );
    if ( nb_3 != nb_2 )
        return;

    deduireVaccinA( &((*abr)->fils_droite), marque, nb_vaccins );
}


void deduireVaccinL (T_ListeVaccins** listeVaccins, char* marque, unsigned int nb_vaccins) {
    if ( listeVaccins == NULL ) {
        printf( "ERREUR FATALE !\n" );
        return;
    }
    if ( *listeVaccins == NULL || nb_vaccins <= 0 )
        return;
    // vaccin trouvé
    if ( strcmp( (*listeVaccins)->marque, marque ) == 0 ) {
        // la quantité à déduire < la quantité de cette marque
        if ( nb_vaccins < (*listeVaccins)->nombre_vaccins ) {
            (*listeVaccins)->nombre_vaccins -= nb_vaccins;
            return;
        }
        // Prendre l'élément à supprimer
        T_ListeVaccins* pVac = *listeVaccins;
        *listeVaccins = (*listeVaccins)->suivant;
        // Isoler l'élément à supprimer
        pVac->suivant = NULL;
        desallouerVaccin( &pVac );
        return;
    }
    //vaccin non trouvé
    deduireVaccinL( &((*listeVaccins)->suivant), marque, nb_vaccins );
}

