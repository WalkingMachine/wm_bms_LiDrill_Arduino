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

const long serialBautRate_c = 115200;  //baud rate UART matériel

///////////////////////////////////
//	Pin Description:
const int pin_VCells_c[5] = {0,	1,	2,	3,	4};

const int pin_IBat_c = 5;		//Sonde de courant
const int pin_Temp_c = 6;		//Sonde de température

const int pin_Buzz_c = 10;
const int pin_Hart_c = 13;
const int pin_Relais = 5;

///////////////////////////////////
//  Constante type d'alarme
const int alrm_aucune_c   = 0;		//aucune alarme
const int alrm_batMax_c   = 1;		//alarme batterie trop haute
const int alrm_batLow_c   = 2;		//alarme batterie basse
const int alrm_batCritq_c = 3;		//alarme batterie très basse
const int alrm_batMin_c	  = 4;		//alarme batterie trop basse

///////////////////////////////////
//	Variables Constantes:
const long usPerSec_c = 1000000;						//nombre de us dans 1 seconde

const long freqLoopSurv_c = 100;								//Fréquance de la boucle de surveillance, (hz)
const long periodLoopSurv_c = usPerSec_c / freqLoopSurv_c;		//Période de la boucle de surveillance

const long hartTmpPeriod_c = usPerSec_c;						//Période Hart	   (us = 1sec / hz) ou (us = 1sec * periode)
const long hartTmpActif_c  = hartTmpPeriod_c / 40;				//Temps actif Hart (us = hartTmpPeriod / diviseur)
 
const long buzzTmpPeriod_c = usPerSec_c * 10;					//Période Hart	   (us = 1sec / hz) ou (us = 1sec * periode)
const long buzzTmpActif_c  = buzzTmpPeriod_c / 40;				//Temps actif Hart (us = hartTmpPeriod / diviseur)

const float propMoyVcell_c = 0.1;								//Proportion de la tention des cellule moyenné sur la valeur précédente

const float maxAnal_c = 1024;						//Valeur de 100% d'entre analogique
const float vReff_c = 1.1;						//Référence analogique interne
const float mullVcells_c[5] = {	23.453,	23.308,	23.218,	23.135,	23.082 };		//division du diviseur de tention
								

const float vCellMax_c	 = 4.23;				//tention maximale toléré
const float vCellBas_c	 = 3.5;					//tention avertissement bas
const float vCellTresBas_c = 3.3;					//tention avertissement tre bas
const float vCellMin_c     = 3.0;					//tention minimale toléré

///////////////////////////////////
//	Variables d'états:
long etatHart_g = 0;
long etatBuzz_g = 0;
long crono1_g = 0;   			//Cronometre de loop(), en us
long periodLoopSurvCorr_g = 0;	//période corrigé pour la syncronisation

int etatVCells_g = 0;				//seul l'état 0 n'est pas une alarme

///////////////////////////////////
//	Variables de mesure

float vCellsMoy_g[5] = {0,0,0,0,0};
float vTotalCells = 0.0f;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//*******************************  Fonctions Maitresses  **********************************************************
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Initialisation
void setup()
{
	  startupMusic();								//Son d'indication de démarage

	  Serial.begin(serialBautRate_c);				//Initialisation de la communication Série

	  analogReference(INTERNAL);					//Mise de la référence à l'interne 1.1v

	  check_VCells(true);							//Première lecture des tentions

	  pinMode(pin_Hart_c, OUTPUT);

	  periodLoopSurvCorr_g = periodLoopSurv_c;		//initialisation de la période corrigé
}


//Boucle Maitresse
void loop()
{
	///------Init loop()--------------------//
	long temps1 = micros();						


	//------Boucle Rapide, UART-------------//
	//byte idRecus = check_ARCOM();				//Vérifier données recus


	//------Boucle lente de surveillance----//
	if(crono1_g >= periodLoopSurvCorr_g)		//Activé apres le délais
	{
		check_VCells(false);
		
		//check_Current();

		//check_SecondBMS_State();

		maj_Hart(crono1_g);

		maj_Buzz(crono1_g, etatVCells_g);

		periodLoopSurvCorr_g = periodLoopSurv_c - (crono1_g % periodLoopSurvCorr_g);	//Prochaine période ajusté à cause du dépassement
		crono1_g = 0;							//Reset du crono
	}


	//------Maj Crono-----------------------//
	delayMicroseconds(100);			//petit délais peut être pas nécessais, pour être sur de la fiabilité du deltaMicros
	crono1_g += deltaMicros( temps1 , micros() );
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//*******************************  Fonctions Récurantes  **********************************************************
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Vérifier la tention de chaque cellule
void check_VCells(boolean firstTime)				
{
	float prop	  = propMoyVcell_c;
	if(firstTime){prop = 1.0;}		//si première lecture, pas de moyenne
	float invProp = 1.0 - prop;		//Filtre passe bas, moyenne temporel sur les tentions
	float vTotalCells = 0.0;

	Serial.print("VCells ");
	for(int cell = 0; cell < 5; cell ++)
	{
		//(ancienVoltage * 1-prop   +  nouveau voltage * prop)  
		vCellsMoy_g[cell] = vCellsMoy_g[cell] * invProp + ((float)(analogRead(pin_VCells_c[cell]) / maxAnal_c) * vReff_c * mullVcells_c[cell]) * prop;

		if	   (vCellsMoy_g[cell] <= vCellMin_c    )  { etatVCells_g = alrm_batMin_c;  Serial.print(" A ");}
		else if(vCellsMoy_g[cell] <= vCellTresBas_c)  { etatVCells_g = alrm_batCritq_c; Serial.print(" B ");}
		else if(vCellsMoy_g[cell] <= vCellBas_c    )  { etatVCells_g = alrm_batLow_c;  Serial.print(" C ");}
		else if(vCellsMoy_g[cell] >= vCellMax_c    )  { etatVCells_g = alrm_batMax_c;  Serial.print(" D ");}
		else {}
		//Serial.print(" : "); Serial.print(vCellsMoy_g[cell],4);
		
		vTotalCells += vCellsMoy_g[cell];		//on additionne les 5 cellules pour obtenir la tention totale
	}
	Serial.println();
}

//Vérifier le courent actuel de la batterie
void check_Current()
{}

//Vérification récurente de l'état de marche de l'autre BMS. Lecture de son HartBeat
void check_SecondBMS_State()
{}

//Vérifier si les alarmes sont toujours valides
void maj_Alarmes()
{
}

//Met à jour de l'état de la LED Hart
void maj_Hart(long cronoIn)
{
	if(etatHart_g < hartTmpActif_c)				//Allumage DEL
	{	digitalWrite(pin_Hart_c, HIGH);	}
	else										//Fermeture DEL
	{	digitalWrite(pin_Hart_c, LOW);	}

	etatHart_g += cronoIn; if(etatHart_g >= hartTmpPeriod_c){etatHart_g = etatHart_g % hartTmpPeriod_c;}	//Période fini, reset
}

//Met à jour de l'état du Buzzer
void maj_Buzz(long cronoIn, int typeIn)
{
	if(typeIn == alrm_batCritq_c || typeIn == alrm_batMin_c || typeIn == alrm_batMax_c)
	{
		if(etatBuzz_g < buzzTmpActif_c / 2) //ton 1
		{	tone(pin_Buzz_c, cronoIn / 12);	}
		else if(etatBuzz_g < buzzTmpActif_c) //ton 2
		{	tone(pin_Buzz_c, cronoIn / 13);	}
		else if(etatBuzz_g < buzzTmpActif_c * 3) //ton 2
		{	tone(pin_Buzz_c, cronoIn / 12);	}
		else
		{	noTone(pin_Buzz_c);				}
	}
	else if(typeIn == alrm_batLow_c)
	{
		if(etatBuzz_g < buzzTmpActif_c / 2) //ton 1
		{	tone(pin_Buzz_c, cronoIn / 10);	}
		else if(etatBuzz_g < buzzTmpActif_c) //ton 2
		{	tone(pin_Buzz_c, cronoIn / 15);	}
		else
		{	noTone(pin_Buzz_c);				}
	}
	else //alrm_aucune
	{
		noTone(pin_Buzz_c);
	}

	etatBuzz_g += cronoIn; if(etatBuzz_g >= buzzTmpPeriod_c){etatBuzz_g = etatBuzz_g % buzzTmpPeriod_c;}	//Période fini, reset et ajustement du dépassement
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//*******************************  Fonctions Évenementielles  *****************************************************
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void startupMusic()
{
	 for(int x = 0; x < 4; x ++)
	 {
		int rnd = random(400,2000) + (x * 100);
		tone(pin_Buzz_c, rnd, 20);  //tone(pin, hz, ms)
		delay(30);
		tone(pin_Buzz_c, rnd, 20);  //tone(pin, hz, ms)
		delay(30);
		tone(pin_Buzz_c, rnd, 20);  //tone(pin, hz, ms)
		delay(30);
	 }
}

//Le relais ne doit être fermer que ICI (Par Sécurité programique)
void essayerFermerRelais()
{
}

//Le relais ne doit être ouvert que ICI (Par Sécurité programique)
void ouvrirRelais()
{	
	if(etatVCells_g == alrm_batMax_c || etatVCells_g == alrm_batMin_c)  //la tention de une cellule et + est inaxeptable
	{
		digitalWrite(pin_Relais, LOW);										//Coupure de la sortie de courant de la batterie
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//*******************************  Utilitaires ********************************************************************
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//FONCTION BIDON, EN ATTENTE DE LA FONCTION ARCOM
byte check_ARCOM()
{
	return 0;
}