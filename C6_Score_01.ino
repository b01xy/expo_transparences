/*

Programme associé au dispositif C6 Transparent un peu, beaucoup, pas du tout de l'expo Transparence
en coopération avec l'atelier TAC
Auteur : Benjamin CADON pour la Labomedia - Licence GNU GPL v3.0 https://www.gnu.org/licenses/gpl-3.0.fr.html

Scénario d'usage :
On insère un premier disque dans l'une des fentes
On évalue son niveau de transparence
Si cela correspond à la fente dans lequel le disque a été inséré, on augmente le score de 1 et on passe la LED témoin au vert
Si cela ne correspond pas à la fente dans lequel le disque a été inséré, on passe la LED témoin à rouge
Un compte à rebour s'affiche sur les 4 afficheurs à LED
Le score s'affiche sur les 2 afficheurs à LED
On insère un premier disque dans l'une des fentes
Transparent tout à droite
Translucide milieu
Opaque tout à gauche
On évalue son niveau de transparence
Si cela correspond à la fente dans lequel le disque a été inséré, on augmente le score de 1 et on passe la LED témoin au vert
Si cela ne correspond pas à la fente dans lequel le disque a été inséré, on passe la LED témoin à rouge
Si pas de détection de laser au bout de xx minutes, on considère qu'il n'y a plus personne devant le dispositif
On éteint

Partie qui pilote l'arduino qui affiche le score

Du coté de l'arduino maitre, nécessité de digitalWrite(12, LOW); pour incrémenter le score



Affichage :::::

 This sketch runs a code that print digits 1234 on a 
 common cathode seven segment display.

  The circuit:
 * 4094 Data pin to digital pin 2
 * 4094 Clock pin to digital pin 3
 * 4094 Strobe pin to digital pin 4

 Library originally released 21 May 2017
 by Suhrid Srivastava

 This example code is in the public domain. 


 E2 - out 5
 D3 - out 4
 C4 - out 3
 B6 - out 2
 A7 - out 1
 DP8 - out 8
 F9 - out 6
 G10 - out 7
 
 */

 #include <EmSevenSegment.h>

EmSevenSegment disp(2,'C',2,3,4);

const int scorePin = 12;  // Broche d'entrée pour le déclenchement du +1 au score
const int resetPin = 13;  // Broche d'entrée pour le reset du score
int score = 0;

void setup() {
  pinMode(scorePin, INPUT);
  digitalRead(scorePin) == LOW;
  pinMode(resetPin, INPUT);
  disp.set("ALIGN",'R');
  disp.set("zeros",'Y');
  disp.set("latch",'Y');
  disp.clear();
  disp.print(00);
  Serial.begin(9600);
}

void loop() {
  // Vérifie si l'entrée est HIGH et que le compte à rebours n'est pas déjà lancé
  //disp.print(0130);
  // Serial.print(digitalRead(triggerPin));  
  // Serial.println(" D11");
  if (digitalRead(scorePin) == HIGH ) {
    score = score + 1;
    Serial.println("un point en plus");
    disp.print(score);
    //delay(6);
    delay(199);
  }
  if (digitalRead(resetPin) == HIGH ) {
    delay(10000);
    score = 0;
    Serial.println("reset du score");
    disp.print(score);
    //delay(500);
  }
  //Serial.print(digitalRead(resetPin));  
  delay(2);
}

