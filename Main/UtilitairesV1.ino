///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* 
		Utilitaires temporels V-1.1

		Version 1: 02-2015
		Auteur: Jimmy Poirier
		
		-deltaMicros calcule le temps passé en en micro seconde
		sans risquer d'être erroné par le retour à 0 du cronometre pincipal 
		-delaisComplex combine la durée de millis et la précision de micros
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//calcule du temps entre deux emprunte en prenant en compte le role over du LONG
unsigned long deltaMicros(unsigned long emprunte1 , unsigned long emprunte2)
{
  unsigned long maximLong = 4294967295uL;
  unsigned long durerSortie = 0;
  
  if(emprunte2 < emprunte1)
  {
    durerSortie = (maximLong - emprunte1) + emprunte2; //compensation du dépassemen du nombre maximum
  }
  else
  {
    durerSortie = emprunte2 - emprunte1;
  }

  return durerSortie;
}

//dÃ©lais complex suprime les limitation de delay et delay micros
//******Il faudrait dÃ©terminer le temps que prend cette mÃ©thode et la soustraire au dÃ©lais
void delaisComplex(unsigned long microseconde) //limitÃ© a environ 70 minutes
{
   long milliSec = microseconde / 1000;
   int microSec = (microseconde / 1000000) - (milliSec * 1000);
   delay(milliSec);
   delayMicroseconds(microSec);
}


