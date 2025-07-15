/*

Programme associé au dispositif G4 Dévier la lumière de l'expo Transparence
en coopération avec l'atelier TAC
Auteur : Benjamin CADON pour la Labomedia - Licence GNU GPL v3.0 https://www.gnu.org/licenses/gpl-3.0.fr.html

Scénario d'usage :
On appuie sur le bouton jaune
On allume le premier laser
On déclenche un compteur qui fait office de Time-out (si la personne arrête, on éteind à un moment)
On mesure la valeur de lumière reçue sur la résistance LDR 
si lumière > seuil, cela veut dire que le laser tape dans la LDR, c'est gagné !
on déclenche un son et on allume la LED verte
on éteind le laser

Un nouvel appui sur le bouton jaune allume le laser suivant

Si pas de détection de laser au bout de 20 secondes, on considère qu'il n'y a plus personne devant le dispositif
On éteint

 */

#include <TimeOut.h>

int numlaser = 0;
int start = 0;
const int analogInPin = A0;  // Analog input pin Capteur lumière LDR
int sensorValue = 0;  // value read from the pot
int seuil = 650; // seuil de détection du laser sur le capteur LDR = cible qui valide (650 pour rectangle 550 pour disque)
// régler la valeur de base de la lumiere captée à ~750 avec la petite vis du bornier bleu 

TimeOut timeout0;
void callback0();


void setup() {
   pinMode(2, INPUT_PULLUP);    // déclarer la broche 2 comme entrée pour l'interrupteur jaune
   pinMode(4, OUTPUT); // déclarer la broche 4 comme sortie laser 1
   pinMode(5, OUTPUT); // déclarer la broche 5 comme sortie laser 2
   pinMode(6, OUTPUT); // déclarer la broche 6 comme sortie laser 3
   pinMode(9, OUTPUT);// Circuit: Audio output on digital pin 9 on a Uno
   pinMode(13, OUTPUT); // déclarer la broche 13 comme sortie pour la LED verte
   Serial.begin(9600);
}



void loop() {
  timeout0.handler();

  if (digitalRead(2) == LOW) { // Bouton jaune pressé, on allume le laser suivant
      Serial.println("Bouton_pressé");
      start = 1;
      timeout0.cancel(); 
      numlaser = numlaser +1;
      if (numlaser == 4) numlaser =1;
      Serial.println("Laser_numero");
      Serial.println(numlaser);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(13, LOW); // LED
      delay(500); // delay pour éviter la répétition du bouton
      timeout0.timeOut(20000, callback0); // Timeout au bout de 20 secondes : on éteint les lasers si pas gagnés //delay, callback function      
  }
  if (numlaser == 1 && start == 1) {
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
  }
  if (numlaser == 2 && start == 1) {
        digitalWrite(4, LOW);
        digitalWrite(5, HIGH);
        digitalWrite(6, LOW);
  }
  if (numlaser == 3 && start == 1) {
        digitalWrite(4, LOW);
        digitalWrite(5, LOW);
        digitalWrite(6, HIGH);
  }
  // On lit la valeur du capteur
  sensorValue = analogRead(analogInPin);
  // print the results to the Serial Monitor:
 // Serial.println("sensor_LDR");
  Serial.println(sensorValue);
  // Le Laser a atteint la cible !!
  if (sensorValue < seuil && start == 1) { // Gagné !!!
    start = 0;
    digitalWrite(13, HIGH); // LED
    digitalWrite(9, HIGH); // Audio
    Serial.println("Gagne");
    delay(3000);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(13, LOW); // LED
    timeout0.cancel(); 
  }
  if (start == 0) { // On eteind tout car jeu gagné ou pas d'activité
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(9, LOW);  // Audio
      digitalWrite(13, LOW); // LED
  }
  delay(2);

}

void callback0() {
  start = 0;
  Serial.println("Timer0 a été déclenché au bout de 20 sec.");
  Serial.println("");
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(9, LOW);  // Audio
  digitalWrite(13, LOW); // LED

}
