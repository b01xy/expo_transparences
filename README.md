# Exposition Transparences

Code et plans pour 2 modules de l'expo "Transparences" réalisés en collaboration avec [l'Atelier TAC ](https://ateliertac.fr/) pour la Cité des Sciences

Les 2 modules :

## G4 Dévier la lumière de l'expo Transparence

Scénario d'usage :
* On appuie sur le bouton jaune
* On allume le premier laser
* On déclenche un compteur qui fait office de Time-out (si la personne arrête, on éteind à un moment)
* On mesure la valeur de lumière reçue sur la résistance LDR 
* si lumière > seuil, cela veut dire que le laser tape dans la LDR, c'est gagné !
* on déclenche un son et on allume la LED verte
* on éteind le laser

 * Un nouvel appui sur le bouton jaune allume le laser suivant

 * Si pas de détection de laser au bout de 20 secondes, on considère qu'il n'y a plus personne devant le dispositif
On éteind

## C6 Transparent un peu, beaucoup, pas du tout de l'expo Transparence

Scénario d'usage :
* On insère un premier disque dans l'une des fentes
* On évalue son niveau de transparence
* Si cela correspond à la fente dans lequel le disque a été inséré, on augmente le score de 1 et on passe la LED témoin au vert
* Si cela ne correspond pas à la fente dans lequel le disque a été inséré, on passe la LED témoin à rouge
* Un compte à rebour s'affiche sur les 4 afficheurs à LED
* Le score s'affiche sur les 2 afficheurs à LED
* On insère un premier disque dans l'une des fentes
* Transparent tout à droite // Translucide milieu // Opaque tout à gauche
* On évalue son niveau de transparence
  * Si cela correspond à la fente dans lequel le disque a été inséré, on augmente le score de 1 et on passe la LED témoin au vert
  *  Si cela ne correspond pas à la fente dans lequel le disque a été inséré, on passe la LED témoin à rouge
* On incrémente un score et décréminte un compte à rebours de 90 secondes
