//################################################
//Projet Mesurex V. 2.03
//
//Auteurs: Lazare Viennot et PHAN Nu Uyen Thi
//
//Date: 20/03/2019
//
//DESCRIPTION / MANUEL:
//Ce programme � pour but de contr�ler un robot mesurant
//des distances.
//Quand l'utilisateur presse le bouton du haut, le robot
//se d�place tout droit en avant jusqu'� rencontrer un obtacle. S'il presse le bouton du bas, le robot
//se d�place � reculon jusqu'� ce que l'utilisateur presse le bouton du millieu.
//Si l'utilisateur appuye sur le bouton du millieu, le robot avance en parcours sinueux et l'utilisateur peut contr�ler la direction ( gauche ou droit) gr�ce � la manette.
//Il affiche ensuite la distance parcourue sur l'�cran, en centim�tres (cm). Les valeurs mesur�es seront stock�es dans un fichier qu'on a cr�� ( fichier mesures.txt).
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
//D�claration du capteur d'angle

// Fonction de calcul des valeurs mesur�es
float calcul(){
float n;
tHTANG angleSensor;
initSensor(&angleSensor, S1);
readSensor(&angleSensor); //on r�cup�re l'angle total mesur� par le capteur
n=angleSensor.accumlatedAngle;
n=n*(-1); //on inverse la valeur de l'angle (le capteur est � l'envers)
float tours_sensor=n/360;//on calcule le nombre de tour parcourus par les roues
float distance_sensor=((tours_sensor*17.6)- 1.5); //on calcule la distance d'apr�s la circonf�rence des roues (17.6cm)
displayCenteredBigTextLine (3, "Distance: ");
displayCenteredBigTextLine (5, "%f cm", distance_sensor);//on affiche � l'�cran la distance parcourue
// on �crire la valeur mesur�e au fichier avec ";" entre les valeurs
return distance_sensor;
}



// Dans la programme principale, on utilise fonction calcul
task main(){
	setSoundVolume(50);
	playSoundFile("Detected");


	float mesure;
	char valeur[10];
// ouverture le fichier pour garder les valeurs mesur�es
	long fileHandle;
	fileHandle = fileOpenWrite("mesures.txt");
	tHTANG angleSensor;
 	initSensor(&angleSensor, S1);
	SensorType[S2] = sensorEV3_Touch;
	SensorType[S3] = sensorEV3_Touch;
	SensorType[S4] = sensorEV3_Touch;


	#ifdef NXT
  nNxtButtonTask  = -2;
	#endif

	while(getButtonPress(buttonLeft)==false && getButtonPress(buttonRight)==false){
		if(getButtonPress(buttonUp)==true){//Si on appuye sur [avant], on enclenche "marche_avant" du robot
			setLEDColor(ledGreenFlash); // on change la couleur de LED au brique
			resetSensor(&angleSensor);
 			while (SensorValue[S2]==0){
				setMotorSpeed(motorA,10);
				setMotorSpeed(motorD,10); //On d�marre les moteur � vitesse 10 tant que le capteur de contact n'est pas activ�
			}
		stopMotor(motorA);
		stopMotor(motorD);
		mesure=calcul();
		sprintf(valeur,"%f", mesure);
		fileWriteData(fileHandle,valeur,strlen(valeur));
		fileWriteData(fileHandle,";",strlen(";"));
		sleep(1000);
	}

		if(getButtonPress(buttonDown)==true){//Si on appuye sur [arri�re], on enclenche "marche_arri�re" du robot
			setLEDColor(ledOrangeFlash);// on change la couleur de LED au brique
			resetSensor(&angleSensor);
			while (getButtonPress(buttonEnter)==false){
				setMotorSpeed(motorA,-10);
				setMotorSpeed(motorD,-10); //On d�marre les moteur � vitesse 10 tant que le capteur de contact n'est pas activ�
			}
			stopMotor(motorA);
			stopMotor(motorD);
			mesure=calcul();
			sprintf(valeur,"%f", mesure);
			fileWriteData(fileHandle,valeur,strlen(valeur));
			fileWriteData(fileHandle,";",strlen(";"));
			sleep(1000);
		}

		if(getButtonPress(buttonEnter)==true){//Si on appuye sur [centre], on enclenche  "parcours sinueux" du robot
			setLEDColor(ledRedFlash);// on change la couleur de LED au brique
			resetSensor(&angleSensor);
			int vitesseA;
			int vitesseD;
			while (SensorValue[S2]==0){
				vitesseA=10;
				vitesseD=10;
				setMotorSpeed(motorA, vitesseA);
				setMotorSpeed(motorD, vitesseD);
				while (SensorValue[S3]==1){
					vitesseD = 12;
					vitesseA = 5;
					setMotorSpeed(motorD, vitesseD);
					setMotorSpeed(motorA, vitesseA);
				}
				while (SensorValue[S4]==1){
					vitesseA = 12;
					vitesseD = 5;
					setMotorSpeed(motorA, vitesseA);
					setMotorSpeed(motorD, vitesseD);
				}
			}
			stopMotor(motorA);
			stopMotor(motorD);
			mesure=calcul();
			sprintf(valeur,"%f", mesure);
			fileWriteData(fileHandle,valeur,strlen(valeur));
			fileWriteData(fileHandle,";",strlen(";"));
			sleep(1000);
		}

	}

//fermeture le fichier
	fileClose(fileHandle);
}
