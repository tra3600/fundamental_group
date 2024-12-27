Le groupe fondamental est un concept central en topologie algébrique, représentant les classes d'homotopie de boucles basées en un point donné dans un espace topologique. En termes simples, il capture les informations sur la structure des chemins fermés dans l'espace.

Pour illustrer le groupe fondamental en C++, nous allons modéliser un espace simple, comme un cercle, et montrer comment les boucles basées en un point peuvent être représentées et manipulées.

Explications
Définition d'un Point sur le Cercle :

La structure Point représente un point sur le cercle unitaire avec des coordonnées (x, y).
Fonction point_on_circle :

Cette fonction calcule les coordonnées d'un point sur le cercle unitaire pour un angle donné en utilisant les fonctions trigonométriques cos et sin.
Classe Loop :

La classe Loop représente une boucle sur le cercle, définie par une séquence de points.
Le constructeur de la classe prend une séquence d'angles et convertit chaque angle en un point sur le cercle.
La méthode print affiche les points de la boucle.
La méthode compose concatène deux boucles en combinant leurs séquences d'angles.
Programme Principal :

Deux boucles sont définies par des séquences d'angles.
Les boucles sont affichées.
Les deux boucles sont composées (concaténées) pour créer une nouvelle boucle, qui est ensuite affichée.
Utilisation
Compilez le programme avec un compilateur C++ :

g++ fundamental_group.cpp -o fundamental_group -lm
Exécutez le programme :

./fundamental_group
Ce programme illustre le concept de base du groupe fondamental en représentant des boucles sur un cercle et en montrant comment ces boucles peuvent être composées. Pour des applications plus complexes en topologie algébrique, d'autres structures et algorithmes devraient être utilisés.
