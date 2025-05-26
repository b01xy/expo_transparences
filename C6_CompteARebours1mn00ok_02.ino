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

Du coté de l'arduino maitresse, nécessité de digitalWrite(11, LOW); pour figer le compte à rebours, digitalWrite(11, HIGH); pour le démarrer
La broche de sortie Digital 10 de l'arduino Compte à Rebours passe à High quand le compte à rebours et fini afin d'envoyer l'info à l'arduino maitresse

Affichage :::::

 This sketch runs a code that print digits 12.34 on a 
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

EmSevenSegment disp(4,'C',2,3,4);

const unsigned long countdownDuration = 60000; // Durée du compte à rebours : 1 min 30 sec = 90000 en millisecondes + 500 ms pour démarrage en souplesse
const int triggerPin = 11;  // Broche d'entrée pour le déclenchement
const int finPin = 10;  // Broche de sortie pour signifier la fin du compte à rebours
bool countdownStarted = false;
unsigned long countdownStartTime = 0;
unsigned long remainingTime = 0;
int secondsLeft = 0;

void setup() {
  pinMode(triggerPin, INPUT);
  digitalWrite(triggerPin, LOW);
  pinMode(finPin, OUTPUT);
  disp.set("ALIGN",'R');
  disp.set("zeros",'Y');
  disp.set("latch",'Y');
  disp.clear();
  disp.print(130);
  Serial.begin(9600);
}

void loop() {
  // Vérifie si l'entrée est HIGH et que le compte à rebours n'est pas déjà lancé
  //disp.print(0130);
  // Serial.print(digitalRead(triggerPin));  
  // Serial.println(" D11");
  if (digitalRead(triggerPin) == LOW && !countdownStarted) {
      disp.print(100);
      digitalWrite(finPin, LOW);
      delay(10);
  }
  if (digitalRead(triggerPin) == HIGH && !countdownStarted) {
    countdownStarted = true;
    countdownStartTime = millis();
    remainingTime = countdownDuration;
    Serial.println("Compte à rebours démarré : 1 minute 00 secondes");
    delay(10);
  }

  // Si le compte à rebours est en cours
  if (countdownStarted) {
    unsigned long elapsedTime = millis() - countdownStartTime;

    if (elapsedTime < countdownDuration) {
      if (remainingTime == countdownDuration || countdownStartTime + countdownDuration - remainingTime <= millis() - 1000) {
        remainingTime = countdownDuration - elapsedTime;
        secondsLeft = remainingTime / 1000;
        Serial.print("Temps restant : ");
        Serial.print(remainingTime);
        Serial.println(" ms");
      
        if (secondsLeft >= 60) {
          int timeadisplay = secondsLeft - 60 + 100;
          disp.print(timeadisplay);
        }
        if (secondsLeft < 60) {
          disp.print(secondsLeft);
          Serial.println(remainingTime);
          Serial.println(secondsLeft);
        }
      }
      delay(1);  // Met à jour toutes les secondes
    } 
    if (remainingTime < 990){
      Serial.println("Compte à rebours terminé !");
       digitalWrite(finPin, HIGH);
       Serial.println(digitalRead(finPin));
        for (int i = 9; i > -1; i--) {
            disp.clear();
            delay(500);
            disp.print(0000);
            delay(500);           
          }
      //digitalWrite(resetPin, HIGH);    
      countdownStarted = false;
    }
  }
  //Serial.print(digitalRead(triggerPin));
  delay(2);
}

