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
On éteind

 */

 #include "RunningAverage.h"// Numéros de sorties Digital de l'Arduino pour connecter les LEDS 
int TransparentROUGE = 10;
int TransparentVERT = 9;
int TransparentBLEU = 8;
int TranslucideROUGE = 7;
int TranslucideVERT = 6;
int TranslucideBLEU = 5;
int OpaqueROUGE = 4;
int OpaqueVERT = 3;
int OpaqueBLEU = 2;

int sensorTransparent = A0;   // entrée analogique pour le capteur LDR Opaque
int sensorTranslucide = A1;   // entrée analogique pour le capteur LDR Translucide
int sensorOpaque = A2;   // entrée analogique pour le capteur LDR Transparent

const int triggerCompteAReboursPin = 11;  // Broche de sortie pour le déclenchement du compte à rebours
const int scorePin = 12;  // Broche de sortie pour le déclenchement du compte à rebours
const int resetscorePin = 9;  // Broche de sortie pour le déclenchement du compte à rebours
const int finPin = 7;  // Broche de sortie pour le déclenchement du compte à rebours


int ValeurSensorOpaque = 1000;   // variable pour le capteur LDR opaque
int ValeurSensorTranslucide = 1000;   // variable pour le capteur LDR translucide
int ValeurSensorTransparent = 1000;   // variable pour le capteur LDR transparent


bool running = false; // déclaration d'une variable de type booléenne appelée running et initialisée à false
int score;
int myTime; // mesure du temps écoulé
int compteRebour; // mesure du temps écoulé en millisecondes
int compteRebourSeconde; // mesure du temps écoulé en millisecondes 
int delaisMax = 90000; // temps de jeu en millisecondes
int delaisEssai = 100; // délais entre chaque essai pour laisser allumé les LEDs RVB quand un disque est inséré
int compteRebourPostEssai; // variable pour éteindre les leds après 'delaisEssai' ms
int nbreMoy = 50; // check nombre de val[] !
int val[ 50 ] ; // val is an array of 50 integers

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;  // will store last time LED was updated

// constants won't change:
const long interval = 1000;  // interval at which to blink (milliseconds) 

int scoreOpaque = 0;
int scoreTranslucide = 0;
int scoreTransparent = 0;

int TopscoreOpaque = 40;
int TopscoreTranslucide = 12;
int TopscoreTransparent = 20;

float SeuilSensorDemarrage = 880; // Seuil pour définir quand le disque traverse l'une des fentes 
float SeuilSensorTransparentH = 1010; // Seuil pour définir quand le disque est transparent
float SeuilSensorTransparentB = 700; // Seuil pour définir quand le disque est transparent
float SeuilSensorTranslucideH = 600; // Seuil pour définir quand le disque est translucide
float SeuilSensorTranslucideB = 200; // Seuil pour définir quand le disque est translucide
float SeuilSensorOpaqueH = 200; // Seuil pour définir quand le disque est opaque
float SeuilSensorOpaqueB = 8; // Seuil pour définir quand le disque est opaque


void setup() {
   pinMode(OpaqueROUGE, OUTPUT);
   pinMode(OpaqueVERT, OUTPUT);
   pinMode(TranslucideROUGE, OUTPUT);
   pinMode(TranslucideVERT, OUTPUT);
   pinMode(TransparentROUGE, OUTPUT);
   pinMode(TransparentVERT, OUTPUT);
   pinMode(triggerCompteAReboursPin, OUTPUT);
   digitalWrite(triggerCompteAReboursPin, LOW);
   pinMode(scorePin, OUTPUT);
   digitalWrite(scorePin, LOW);
   pinMode(resetscorePin, OUTPUT);
   digitalWrite(resetscorePin, LOW);
   pinMode(finPin, INPUT);
   Serial.begin(115200);

}

void loop() {

  // On lit les valeurs des capteurs en continu
  ValeurSensorTransparent = analogRead(sensorTransparent);
  ValeurSensorTranslucide = analogRead(sensorTranslucide);
  ValeurSensorOpaque = analogRead(sensorOpaque);

  digitalWrite(triggerCompteAReboursPin, LOW);
  digitalWrite(scorePin, LOW);
  digitalWrite(resetscorePin, LOW);

  scoreOpaque = 0;
  scoreTranslucide = 0;
  scoreTransparent = 0;

  if (ValeurSensorOpaque < SeuilSensorDemarrage || ValeurSensorTranslucide < SeuilSensorDemarrage || ValeurSensorTransparent  < SeuilSensorDemarrage){
    running = true; // un disque a été inséré, on démarre le jeu
    digitalWrite(triggerCompteAReboursPin, HIGH);
    Serial.println("on demarre le compte à rebours");
    Serial.println(running);
    //Serial.println(compteRebourSeconde);
  }
    digitalWrite(OpaqueVERT, LOW);
    digitalWrite(OpaqueROUGE, LOW);
    digitalWrite(TranslucideVERT, LOW);
    digitalWrite(TranslucideROUGE, LOW);
    digitalWrite(TransparentVERT, LOW);
    digitalWrite(TransparentROUGE, LOW);

  if (running == true){
     
      // Fente OPAQUE
      if (ValeurSensorOpaque < SeuilSensorDemarrage){
          //MoyOpaq.clear();
          for (int i = 0; i < nbreMoy; i++) {
            ValeurSensorOpaque = analogRead(sensorOpaque);
            val[ i ] = ValeurSensorOpaque;
            if (ValeurSensorOpaque < SeuilSensorOpaqueH && ValeurSensorOpaque > SeuilSensorOpaqueB) scoreOpaque = scoreOpaque + 1;
            if (ValeurSensorOpaque < SeuilSensorTranslucideH && ValeurSensorOpaque  > SeuilSensorTranslucideB) scoreTranslucide = scoreTranslucide + 1;
            if (ValeurSensorOpaque > SeuilSensorTransparentB && ValeurSensorOpaque  < SeuilSensorTransparentH) scoreTransparent = scoreTransparent + 1;

            delay(2);
          }
          for ( int j = 0; j < nbreMoy; ++j ) {
            Serial.print (val[j]) ;
            Serial.print (",") ;
          }

          if (scoreTranslucide > TopscoreTranslucide) {
            Serial.println("Translucide !!!");
            digitalWrite(OpaqueROUGE, HIGH);
          }
          //cas de figure où c'est à la fois translucide et transparent quand disque laché très vite  
          if (scoreTransparent > TopscoreTransparent) {
            Serial.println("Transparent !!!");
            digitalWrite(OpaqueROUGE, HIGH);
          }
          if (scoreOpaque > TopscoreOpaque) {
            Serial.println("Opaque !!!");
            digitalWrite(OpaqueVERT, HIGH);
            digitalWrite(scorePin, HIGH);
          }
          delay(delaisEssai);            
      }
      
      // Fente Translucide
      if (ValeurSensorTranslucide < SeuilSensorDemarrage){
          //MoyOpaq.clear();
          for (int i = 0; i < nbreMoy; i++) {
            ValeurSensorTranslucide = analogRead(sensorTranslucide);
            val[ i ] = ValeurSensorTranslucide;
            if (ValeurSensorTranslucide < SeuilSensorOpaqueH && ValeurSensorTranslucide > SeuilSensorOpaqueB) scoreOpaque = scoreOpaque + 1;
            if (ValeurSensorTranslucide  < SeuilSensorTranslucideH && ValeurSensorTranslucide  > SeuilSensorTranslucideB) scoreTranslucide = scoreTranslucide + 1;
            if (ValeurSensorTranslucide > SeuilSensorTransparentB && ValeurSensorTranslucide  < SeuilSensorTransparentH) scoreTransparent = scoreTransparent + 1;

            delay(2);
          }
          for ( int j = 0; j < nbreMoy; ++j ) {
            Serial.print (val[j]) ;
            Serial.print (",") ;
          }

          if (scoreTranslucide > TopscoreTranslucide) {
            Serial.println("Translucide !!!");
            digitalWrite(TranslucideVERT, HIGH);
            digitalWrite(scorePin, HIGH);
          }
          //cas de figure où c'est à la fois translucide et transparent quand disque laché très vite  
          if (scoreTransparent > TopscoreTransparent) {
            Serial.println("Transparent !!!");
            digitalWrite(TranslucideROUGE, HIGH);
          }
          if (scoreOpaque > TopscoreOpaque) {
            Serial.println("Opaque !!!");
            digitalWrite(TranslucideROUGE, HIGH);
          }
          delay(delaisEssai);          
      }

      // Fente Tranparente
      if (ValeurSensorTransparent < SeuilSensorDemarrage){
          //MoyOpaq.clear();
          for (int i = 0; i < nbreMoy; i++) {
            ValeurSensorTransparent = analogRead(sensorTransparent);
            val[ i ] = ValeurSensorTransparent;
            if (ValeurSensorTransparent < SeuilSensorOpaqueH && ValeurSensorTransparent > SeuilSensorOpaqueB) scoreOpaque = scoreOpaque + 1;
            if (ValeurSensorTransparent  < SeuilSensorTranslucideH && ValeurSensorTransparent  > SeuilSensorTranslucideB) scoreTranslucide = scoreTranslucide + 1;
            if (ValeurSensorTransparent > SeuilSensorTransparentB && ValeurSensorTransparent  < SeuilSensorTransparentH) scoreTransparent = scoreTransparent + 1;

            delay(2);
          }
          for ( int j = 0; j < nbreMoy; ++j ) {
            Serial.print (val[j]) ;
            Serial.print (",") ;
          }
          affichelesvaleurs();
          if (scoreTranslucide > TopscoreTranslucide) {
            Serial.println("Translucide !!!");
            digitalWrite(TransparentROUGE, HIGH);
          }
          //cas de figure où c'est à la fois translucide et transparent quand disque laché très vite  
          if (scoreTransparent > TopscoreTransparent) {
            Serial.println("Transparent !!!");
            digitalWrite(TransparentVERT, HIGH);
            digitalWrite(scorePin, HIGH);
          }
          if (scoreOpaque > TopscoreOpaque) {
            Serial.println("Opaque !!!");
            digitalWrite(TransparentROUGE, HIGH);
          }
          delay(delaisEssai);
      }
  delay(200); // délais entre 2 insertion de disque ? à check si possible de baisser
  }  
  if (digitalRead(finPin) == HIGH){
    running = false;
    Serial.println("Fin de partie !!!!!!");
    Serial.println("");
    digitalWrite(OpaqueROUGE, HIGH);
    digitalWrite(TranslucideROUGE, HIGH);
    digitalWrite(TransparentROUGE, HIGH);
    delay(10000);
    digitalWrite(resetscorePin, HIGH);
  }
  delay(2);
  //Serial.println(digitalRead(finPin));
}


void affichelesvaleurs(){

            Serial.println("");
            Serial.println("+++");

            Serial.print("scoreOpaque ");         
            Serial.print(scoreOpaque);
            Serial.print("/tr");
            Serial.print("scoreTranslucide ");          
            Serial.print(scoreTranslucide);
            Serial.print("/tr");
            Serial.print("scoreTransparent ");          
            Serial.print(scoreTransparent);
            Serial.println("");
            // Serial.print("valeur capteur lum ");          
            // Serial.print(ValeurSensorOpaque);
            // Serial.print("");
            // Serial.print(ValeurSensorTranslucide);
            // Serial.print("");
            // Serial.print(ValeurSensorTransparent);
            // Serial.println("");   


}