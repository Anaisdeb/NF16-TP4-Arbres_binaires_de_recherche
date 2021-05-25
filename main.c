#include "tp4.h"

int main () {
    setbuf( stdout, NULL );
    int opt = 0;
    T_ABR* abr = NULL;
    while ( opt != 6 ) {
        afficherMenu();
        opt = traiterEntree( true, NULL );
        switch ( opt ) {
            case 1:
                if ( abr != NULL ) {
                    libererRessrc( abr );
                    abr = NULL;
                }
                abr = initialiserInstanceFch( abr );
                break;
            case 2: {
                if ( abr == NULL ) {
                    printf( "ERREUR : Veuillez d'abord initialiser ! " );
                    break;
                }
                // imprimer les dates existantes
                // attendre la saisie de l'utilisateur
                printf( " - Date ( " );
                afficherDates( abr );
                printf( ") : " );
                char date[100], marque[100];
                traiterEntree( false, date );
                // imprimer les marques existantes de la date saisie
                // attendre la saisie de l'utilisateur
                printf( " - Marque ( " );
                afficherDateMarques( abr, date );
                printf( ") : " );
                traiterEntree( false, marque );
                // attendre la saisie de la quantité de l'utilisateur
                printf( " - Quantité à ajouter : " );
                int nb_vacs = traiterEntree( true, NULL );
                // Ajout
                ajouterVaccinA( &abr, date, marque, nb_vacs );
                break;
            }
            case 3:
                if ( abr == NULL ) {
                    printf( "ERREUR : Veuillez d'abord initialiser ! " );
                    break;
                }
                afficherStockA( abr );
                break;
            case 4: {
                if ( abr == NULL ) {
                    printf( "ERREUR : Veuillez d'abord initialiser ! " );
                    break;
                }
                // obtenir les marques existantes puis les imprimer
                // attendre la saisie de l'utilisateur
                char** mks = NULL;
                char marque[100];
                int nb_mks = 0;
                enregistrerMarques( abr, &mks, &nb_mks );
                printf( " - Marque ( " );
                for ( int i = 0 ; i < nb_mks ; i++ )
                    printf( "%s, ", *(mks+i) );
                printf( ") : " );
                traiterEntree( false, marque );
                // imprimer la quantite correspontante de la marque saisie
                printf( " - %s : %u [Marque : Quantité]\n", marque, compterVaccins( abr, marque ) );
                for ( int i = 0 ; i < nb_mks ; ++i )
                    free( mks[i] );
                free( mks );
                break;
            }
            case 5: {
                if ( abr == NULL ) {
                    printf( "ERREUR : Veuillez d'abord initialiser ! " );
                    break;
                }
                // obtenir les marques existantes puis les imprimer
                // attendre la saisie de l'utilisateur
                char** mks = NULL;
                char marque[100];
                int nb_mks = 0;
                enregistrerMarques( abr, &mks, &nb_mks );
                printf( " - Marque ( " );
                for ( int i = 0 ; i < nb_mks ; i++ )
                    printf( "%s, ", *(mks+i) );
                printf( ") : " );
                traiterEntree( false, marque );
                if ( !verifierMarque( marque, mks, nb_mks ) ) {
                    printf( "ERREUR : La marque n’existe pas !\nERREUR : Déduction échouée !\n" );
                    break;
                }
                // obtenir la quantité correcte depuis l'utilisateur
                printf( " - Quantité à déduire [Total = %u] : ", compterVaccins( abr, marque ) );
                int nb_vacs = traiterEntree( true, NULL );
                if ( nb_vacs > compterVaccins( abr, marque ) || nb_vacs <= 0 ) {
                    printf( "ERREUR : Quantité érronée !\nERREUR : Déduction échouée !\n" );
                    break;
                }
                // Déduction
                int nb_total = compterVaccins( abr, marque );
                int nb_tour = nb_vacs;
                while ( nb_total-nb_vacs < compterVaccins( abr, marque ) ) {
                    int nb_total_avant = compterVaccins( abr, marque );
                    deduireVaccinA( &abr, marque, nb_tour );
                    nb_tour -= (nb_total_avant-compterVaccins( abr, marque ));
                }
                //liberer les marques
                for ( int i = 0 ; i < nb_mks ; ++i )
                    free( mks[i] );
                free( mks );
                break;
            }
            case 6:
                if ( abr != NULL )
                    libererRessrc( abr );
                printf( "------------- PROGRAMME TERMINÉ --------------" );
                break;
            default:
                printf( "ERREUR : Votre choix n'est pas validé !" );
        }
        printf( "\n" );
    }
    return 0;
}
