# Groupe fondamental

Programme C++ et cours autour du **groupe fondamental** $\pi_1(X)$ : les classes d'homotopie de lacets d'un espace, qui comptent « combien de fois on tourne » autour des trous.

* **[`COURS.md`](COURS.md)** — cours complet (théorie, exemples, comparaison des espaces, applications physiques et techniques, exercices corrigés).
* **`fundamental_group.cpp`** — le programme qui illustre chaque chapitre.

## Ce que le programme calcule

| Module | Espace / phénomène | Résultat vérifié |
|---|---|---|
| 1 | cercle $S^1$ | $\pi_1=\mathbb Z$ : degré additif, inverse, invariance par homotopie |
| 2 | polynôme sur un cercle | principe de l'argument : indice = nombre de racines |
| 3 | tore $T^2$ | $\pi_1=\mathbb Z^2$ |
| 4 | plan privé de 2 points | groupe libre $F_2$ : mots réduits, commutateur |
| 5 | champ de phase sur réseau | vortex / antivortex (superfluides, modèle XY) |
| 6 | Aharonov–Bohm | phase $2\pi n\Phi/\Phi_0$ |
| 7 | $SO(3)$ | $\pi_1=\mathbb Z/2$, quaternions, spineurs |
| 8 | chaîne SSH | invariant d'enroulement d'une phase topologique |

Un lacet sur le cercle est stocké par son **relèvement** dans $\mathbb R$, ce qui conserve le nombre de tours (la première version, qui recomposait par `atan2`, le perdait).

## Utilisation

```
g++ -std=c++17 -O2 -Wall -Wextra fundamental_group.cpp -o fundamental_group
./fundamental_group
```

Chaque vérification affiche `[OK]` ou `[ECHEC]` ; le code de retour est `0` si tout passe.
