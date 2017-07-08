///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* 
		Version 1: 7 Juillet 2017
		Auteur: Jimmy Poirier
		
		-Le programme du BMS 5 cellules Li-ion as principalement pour objectif de
		superviser la tention de chacune des 5 cellules de la batterie 18.5V.
		-La carte communique avec le BMS de la batterie voisine dans le but que
		chacune d'entre elle puissent détecter une défaillance de l'autre.
		-La carte communique avec un haute à l'aide du protocole UART.
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const int serial_BautRate = 115200;  //baud rate UART matériel

///////////////////////////////////
//	Pin Description:
const int pin_VCell0 = 0;		//Tention de chaque cellule relative à 0V
const int pin_VCell1 = 1;
const int pin_VCell2 = 2;
const int pin_VCell3 = 3;
const int pin_VCell4 = 4;

const int pin_IBat = 5;			//Sonde de courant
const int pin_Temp = 6;			//Sonde de température

const long freqLoopSurv = 100;	//Fréquance de la boucle de surveillance, (hz)
const long periodLoopSurv = 1000000 / freqLoopSurv;   //Période de la boucle de surveillance

const int hartFreq = 2;			//Fréquance de la DEL hart
const int hartTh = 10;			//division du temps allumé,  (ex 10 = 1/10 du temps)
const int hartCycles  = periodLoopSurv / hartFreq;		//periode du hart en cycles
const int hartCyclesH = hartCycles / hartTh;				//cycles du hart allumé

const int buzzCycles = ;

///////////////////////////////////
//	Variables d'états:
int etatHart = 0;
int etatBuzz = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//*******************************  Fonctions Maitresses  **********************************************************
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Initialisation
void setup()
{
	  Serial.begin(serial_BautRate);
	  
}


//Boucle Maitresse
void loop()
{
	//------Init Loop-----------------------//
	long temps1 = micros();						//Enmprunte temporel
	long crono1 = 0;   							//Cronometre (us)


	//------Boucle Rapide, UART-------------//
	byte idRecus = check_ARCOM();				//Vérifier données recus


	//------Boucle lente de surveillance----//
	if(crono1 >= periodLoopSurv)				//Activé apres le délais
	{
		crono1 %= periodLoopSurv;				//Syncronisation: reset du crono + garder le dépassement

		check_VCells();
		
		check_Current();

		check_SecondBMS_State();

		maj_Hart();

		maj_Buzz();
	}


	//------Maj Crono-----------------------//
	crono1 += deltaMicros( temps1 ,micros() );
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//*******************************  Fonctions Récurantes  **********************************************************
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Vérifier la tention de chaque cellule
void check_VCells()
{
	
}

//Vérifier le courent actuel de la batterie
void check_Current()
{

}

//Vérification récurente de l'état de marche de l'autre BMS. Lecture de son HartBeat
void check_SecondBMS_State()
{

}

//Met à jour de l'état de la LED Hart
void maj_Hart()
{
	

	etatHart ++; if(etatHart >= hartCycles){etatHart = 0;}	//Période fini, reset
}

//Vérifier si les alarmes sont toujours valides
void maj_Alarmes()
{

}

//Met à jour de l'état du Buzzer
void maj_Buzz()
{

	etatBuzz ++; if(etatBuzz >= buzzCycles){etatBuzz = 0;}	//Période fini, reset
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//*******************************  Fonctions Évenementielles  *****************************************************
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Le relais ne doit être fermer que ICI (Par Sécurité programique)
void essayerFermerRelais()
{
	
}

//Le relais ne doit être ouvert que ICI (Par Sécurité programique)
void ouvrirRelais()
{
	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//FONCTION BIDON, EN ATTENTE DE LA FONCTION ARCOM
byte check_ARCOM()
{
	return 0;
}