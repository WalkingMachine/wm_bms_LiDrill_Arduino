///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* 
		Version 1: 7 Juillet 2017
		Auteur: Jimmy Poirier
		
		-Le programme du BMS 5 cellules Li-ion as principalement pour objectif de
		superviser la tention de chacune des 5 cellules de la batterie 18.5V.
		-La carte communique avec le BMS de la batterie voisine dans le but que
		chacune d'entre elle puissent d�tecter une d�faillance de l'autre.
		-La carte communique avec un haute � l'aide du protocole UART.
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const int serial_BautRate = 115200;  //baud rate UART mat�riel

///////////////////////////////////
//	Pin Description:
const int pin_VCell0 = 0;		//Tention de chaque cellule relative � 0V
const int pin_VCell1 = 1;
const int pin_VCell2 = 2;
const int pin_VCell3 = 3;
const int pin_VCell4 = 4;

const int pin_IBat = 5;			//Sonde de courant
const int pin_Temp = 6;			//Sonde de temp�rature

const long freqLoopSurv = 100;	//Fr�quance de la boucle de surveillance, (hz)
const long periodLoopSurv = 1000000 / freqLoopSurv;   //P�riode de la boucle de surveillance

const int hartFreq = 2;			//Fr�quance de la DEL hart
const int hartTh = 10;			//division du temps allum�,  (ex 10 = 1/10 du temps)
const int hartCycles  = periodLoopSurv / hartFreq;		//periode du hart en cycles
const int hartCyclesH = hartCycles / hartTh;				//cycles du hart allum�

const int buzzCycles = ;

///////////////////////////////////
//	Variables d'�tats:
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
	byte idRecus = check_ARCOM();				//V�rifier donn�es recus


	//------Boucle lente de surveillance----//
	if(crono1 >= periodLoopSurv)				//Activ� apres le d�lais
	{
		crono1 %= periodLoopSurv;				//Syncronisation: reset du crono + garder le d�passement

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
//*******************************  Fonctions R�curantes  **********************************************************
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//V�rifier la tention de chaque cellule
void check_VCells()
{
	
}

//V�rifier le courent actuel de la batterie
void check_Current()
{

}

//V�rification r�curente de l'�tat de marche de l'autre BMS. Lecture de son HartBeat
void check_SecondBMS_State()
{

}

//Met � jour de l'�tat de la LED Hart
void maj_Hart()
{
	

	etatHart ++; if(etatHart >= hartCycles){etatHart = 0;}	//P�riode fini, reset
}

//V�rifier si les alarmes sont toujours valides
void maj_Alarmes()
{

}

//Met � jour de l'�tat du Buzzer
void maj_Buzz()
{

	etatBuzz ++; if(etatBuzz >= buzzCycles){etatBuzz = 0;}	//P�riode fini, reset
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//*******************************  Fonctions �venementielles  *****************************************************
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Le relais ne doit �tre fermer que ICI (Par S�curit� programique)
void essayerFermerRelais()
{
	
}

//Le relais ne doit �tre ouvert que ICI (Par S�curit� programique)
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