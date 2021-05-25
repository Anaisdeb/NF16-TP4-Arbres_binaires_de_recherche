# NF16 - TP4 : Les Arbres Binaires de Recherche

## Énoncé :
<p>&emsp;&emsp;Le département de l’Oise souhaite mettre en place un système pour tracer son stock de vaccins pendant la période épidémique. Sachant qu’il y a plusieurs marques de vaccins, la date de livraison et la quantité sont différents pour chaque marque. C'est-à-dire que, pour un jour, le département peut recevoir plusieurs vaccins de marque différente.<br>&emsp;&emsp;On va également envoyer les vaccins vers le centre de vaccination de chaque commune s’il y a une demande ou une planification : le nombre de vaccins correspondant sera alors déduit dans le système.<br>&emsp;&emsp;Pour ce faire, on va utiliser un ABR pour enregistrer tous les vaccins reçus dans le département. Chaque nœud dans l’ABR représente la date de réception, la quantité de vaccins ainsi que les marques. Les vaccins reçus dans le même jour sont organisés par une liste chaînée. Chaque élément dans la liste chaînée représente une marque de vaccin ainsi que sa quantité.</p>

### A. Structures de données :
- La structure ***ListeVaccins*** et le type correspondant `T_ListeVaccins`, qui comporte les champs :
  - `marque` de type `char*`
  - `nombre_vaccins` de type `unsigned int` (strictement positif !)
  - `suivant` de type `T_ListeVaccins*`
- La structure ***ABR*** et le type correspondant `T_ABR`, qui comporte les champs :
  - `date` de type `char*` (le format est `AAAA-MM-JJ`, le clé du nœud, la date “2021-02-01” est inférieure à la date “2021-03-02”)
  - `liste_vaccins` de type `T_ListeVaccins*`
  - `fils_gauche` de type `struct ABR*`
  - `fils_droite` de type `struct ABR*`

### B. Fonctions à implémenter :
1. Écrire une fonction qui permet d’ajouter un nombre de vaccins à la fin de la liste de vaccins indiquée. Si la marque est déjà présente dans la liste, on incrémente le nombre de vaccins de la marque.
```c
    void ajouterVaccinL(T_ListeVaccins** listeVaccins, char* marque, int nb_vaccins);
```
2. Écrire une fonction qui permet d’ajouter un nombre de vaccins dans un ABR pour une date donnée et une marque indiquée.
```c
    void ajouterVaccinA(T_ABR** abr, char*date, char* marque, int nb_vaccins);
```
3. Écrire une fonction qui permet d’afficher tous les stocks disponibles dans une liste de vaccins indiquée.
```c
    void afficherStockL(T_ListeVaccins* listeVaccins);
```
4. Écrire une fonction qui permet d’afficher tous les stocks disponibles dans un ABR par ordre croissant de la date.
```c
    void afficherStockA(T_ABR* abr);
```
5. Écrire une fonction qui permet de compter le nombre de vaccins disponibles dans un ABR pour la marque indiquée.
```c
    int compterVaccins(T_ABR* abr, char* marque);
```
6. Écrire une fonction qui permet de déduire un nombre de vaccins dans une liste de vaccins indiquée. Si le nombre de vaccins d’une marque est égale à 0, il faut supprimer l’élément correspondant dans la liste chaînée de vaccins.
```c
    void deduireVaccinL(T_ListeVaccins** listeVaccins, char* marque, int nb_vaccins);
```
7. Écrire une fonction qui permet de déduire un nombre de vaccins d’une marque indiquée dans un ABR. On doit déduire les vaccins par ordre croissant de date de réception pour cette marque de vaccins, jusqu’à déduire complètement le nombre demandé. S’il y a un nœud qui ne possède aucun vaccin, il faut le supprimer de l’ABR.
```c
    void deduireVaccinA(T_ABR** abr, char* marque, int nb_vaccins);
```
>Un exemple: on a 20 vaccins d’une marque Pfeizer qui sont reçus au “2021-03-22” et 30 vaccins qui sont reçus au “2021-03-28” dans le système. On veut envoyer 45 vaccins de Pfeizer vers le centre de vaccination de Compiègne. Dans ce cas, on va déduire 20 vaccins premièrement dans le nœud ayant la date “2021-03-22” et ensuite 25 vaccins dans le nœud ayant la date “2021-03-28”.

### C. Interface :
Utiliser les fonctions précédentes pour proposer à l’utilisateur le menu interactif suivant :
```
1. Initialiser un ABR.
2. Ajouter un nombre de vaccins d’une marque dans un ABR par une date indiquée.
3. Afficher tous les stocks disponibles dans un ABR.
4. Compter le nombre de vaccins disponible pour une marque indiquée.
5. Déduire un nombre de vaccins d’une marque indiquée dans un ABR.
6. Quitter.
```
## Consignes générales :
Votre rapport et vos fichiers source feront l’objet d’une remise de devoir sur Moodle dans l’espace qui sera ouvert à cet effet quelques jours suivant votre démonstration au chargé de TP (un seul rendu de devoir par binôme).
- Sources<br>À la fin du programme, les blocs de mémoire dynamiquement alloués doivent être proprement libérés.<br>Vous devrez également être attentifs à la complexité des algorithmes implémentés.<br>L'organisation MINIMALE du projet est la suivante :
  - Fichier d’en-tête tp4.h, contenant la déclaration des structures/fonctions de base,
  - Fichier source tp4.c, contenant la définition de chaque fonction,
  - Fichier source main.c, contenant le programme principal.
- Rapport<br>Votre rapport de quatre pages maximum contiendra :
  - La liste des structures et des fonctions supplémentaires que vous avez choisi d’implémenter et les raisons de ces choix.
  - Un exposé succinct de la complexité de chacune des fonctions implémentées.