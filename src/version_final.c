//################################################
//Projet Mesurex V. 2.03
//
//Auteurs: Lazare Viennot et PHAN Nu Uyen Thi
//
//Date: 20/03/2019
//
//DESCRIPTION / MANUEL:
//Ce programme à pour but de contrôler un robot mesurant
//des distances.
//Quand l'utilisateur presse le bouton du haut, le robot
//se déplace tout droit en avant jusqu'à rencontrer un obtacle. S'il presse le bouton du bas, le robot
//se déplace à reculon jusqu'à ce que l'utilisateur presse le bouton du millieu.
//Si l'utilisateur appuye sur le bouton du millieu, le robot avance en parcours sinueux et l'utilisateur peut contrôler la direction ( gauche ou droit) grâce à la manette.
//Il affiche ensuite la distance parcourue sur l'écran, en centimètres (cm). Les valeurs mesurées seront stockées dans un fichier qu'on a créé ( fichier mesures.txt).
//
//MONTAGE DU ROBOT:
//Moteur gauche sur A
//Moteur Droit sur D
//Capteur angulaire sur 1
//Capteur de contact avant sur 2
//Capteur de contact gauche sur 3
//Capteur de contact droit sur 4
//################################################
#pragma config(Sensor, S1,,sensorEV3_GenericI2C)
#include "hitechnic-angle.h"
//Déclaration du capteur d'angle

// Fonction de calcul des valeurs mesurées
float calcul(){
float n;
tHTANG angleSensor;
initSensor(&angleSensor, S1);
readSensor(&angleSensor); //on récupère l'angle total mesuré par le capteur
n=angleSensor.accumlatedAngle;
n=n*(-1); //on inverse la valeur de l'angle (le capteur est à l'envers)
float tours_sensor=n/360;//on calcule le nombre de tour parcourus par les roues
float distance_sensor=((tours_sensor*17.78)); //on calcule la distance d'après la circonférence des roues (17.6cm)
displayCenteredBigTextLine (3, "Distance: ");
displayCenteredBigTextLine (5, "%f cm", distance_sensor);//on affiche à l'écran la distance parcourue
// on écrire la valeur mesurée au fichier avec ";" entre les valeurs
return distance_sensor;
}



// Dans la programme principale, on utilise fonction calcul
task main(){
	setSoundVolume(50);
	playSoundFile("Detected");


	float mesure;
	char valeur[10];
// ouverture le fichier pour garder les valeurs mesurées
	long fileHandle;
	fileHandle = fileOpenWrite("mesures.txt");
	tHTANG angleSensor;
 	initSensor(&angleSensor, S1);
	SensorType[S2] = sensorEV3_Touch;
	SensorType[S3] = sensorEV3_Touch;
	SensorType[S4] = sensorEV3_Touch;



	while(getButtonPress(buttonLeft)==false && getButtonPress(buttonRight)==false){
		if(getButtonPress(buttonUp)==true){//Si on appuye sur [avant], on enclenche "marche_avant" du robot
			setLEDColor(ledGreenFlash); // on change la couleur de LED au brique
			resetSensor(&angleSensor);
 			while (SensorValue[S2]==0){
				setMotorSpeed(motorA,10);
				setMotorSpeed(motorD,10); //On démarre les moteur à vitesse 10 tant que le capteur de contact n'est pas activé
			}
		stopMotor(motorA);
		stopMotor(motorD);
		mesure=calcul();//on utilise la fonction de calcul
		sprintf(valeur,"%f", mesure);//On affiche la valeur à l'écran
		fileWriteData(fileHandle,valeur,strlen(valeur));
		fileWriteData(fileHandle,";",strlen(";"));//on envoie la valeur dans le fichier "mesures.txt"
		sleep(1000);
	}

		if(getButtonPress(buttonDown)==true){//Si on appuye sur [arrière], on enclenche "marche_arrière" du robot
			setLEDColor(ledOrangeFlash);// on change la couleur de LED au brique
			resetSensor(&angleSensor);
			while (getButtonPress(buttonEnter)==false){
				setMotorSpeed(motorA,-10);
				setMotorSpeed(motorD,-10); //On démarre les moteur à vitesse 10 tant que le capteur de contact n'est pas activé
			}
			stopMotor(motorA);
			stopMotor(motorD);
			mesure=calcul();//on utilise la fonction de calcul
			sprintf(valeur,"%f", mesure);//On affiche la valeur à l'écran
			fileWriteData(fileHandle,valeur,strlen(valeur));
			fileWriteData(fileHandle,";",strlen(";"));//on envoie la valeur dans le fichier "mesures.txt"
			sleep(1000);
		}

		if(getButtonPress(buttonEnter)==true){//Si on appuye sur [centre], on enclenche  "parcours sinueux" du robot
			setLEDColor(ledRedFlash);// on change la couleur de LED au brique
			resetSensor(&angleSensor);
			int vitesseA;
			int vitesseD;
			while (SensorValue[S2]==0){ //Avance tout droit à vitesse 10 tant que le senseur avant n'est pas activé
				vitesseA=10;
				vitesseD=10;
				setMotorSpeed(motorA, vitesseA);
				setMotorSpeed(motorD, vitesseD);
				while (SensorValue[S3]==1){ //Quand le senseur gauche (3) est activé, la vitesse de la roue droite augmente et celle de  diminue. Le robot tourne a gauche.
					vitesseD = 12;
					vitesseA = 5;
					setMotorSpeed(motorD, vitesseD);
					setMotorSpeed(motorA, vitesseA);
				}
				while (SensorValue[S4]==1){//Quand le senseur droit (4)  est activé, la vitesse de la roue gauche augmente et celle de droite diminue. Le robot tourne a droite.
					vitesseA = 12;
					vitesseD = 5;
					setMotorSpeed(motorA, vitesseA);
					setMotorSpeed(motorD, vitesseD);
				}
			}
			stopMotor(motorA);
			stopMotor(motorD); //arrêt des moteurs quand le capteur avant est pressé
			mesure=calcul(); //on utilise la fonction de calcul
			sprintf(valeur,"%f", mesure);//On affiche la valeur à l'écran
			fileWriteData(fileHandle,valeur,strlen(valeur));
			fileWriteData(fileHandle,";",strlen(";")); //on envoie la valeur dans le fichier "mesures.txt"
			sleep(1000);
		}

	}

//fermeture du fichier
	fileClose(fileHandle);
}
