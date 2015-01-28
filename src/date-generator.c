/** @file date-generator.c
 *  @brief Implantation des générateurs de dates
 *
 */
#include <stdio.h>     // printf
#include <stdlib.h>    // Malloc, NULL, exit...
#include <math.h>      // log

#include <motsim.h>
#include <probe.h>
#include <random-generator.h>  

/** 
 * @brief Implantation des générateurs de dates.
 *
 * Largement fondé sur les générateurs de nombres aléatoires.
 * Toutes les dates sont toujours exprimées en secondes depuis le début de
 * la simulation.
 * @param randGen Le generateur aleatoire sur lequel on se fonde.
 * C'est concrètement lui qui va générer les dates successives.
 * C'est une structure de type randomGenerator_t
 * @param interArrivalProbe Une sonde sur les inter arrivees.
 * Elle permettra par exemple de vérifier qu'on est conforme à ce que l'on souhaite.
 */
struct dateGenerator_t {
   /** Le generateur aleatoire sur lequel on se fonde. C'est
       concrètement lui qui va générer les dates successives.*/
   struct randomGenerator_t *randGen; 

   /** Une sonde sur les inter arrivees. Elle permettra par exemple
       de vérifier qu'on est conforme à ce que l'on souhaite. */
   struct probe_t * interArrivalProbe;

  //   double (* nextDate)(struct dateGenerator_t * dateGen, double currentTime);  //   void           * data;
};

/*-------------------------------------------------------------------------*/
/*   Les constructeurs                                                     */
/*-------------------------------------------------------------------------*/
/**
 * @fn void dateGenerator_setRandomGenerator(struct gateGenerator_t * dateGen, struct randomGenerator_t * randGen)
 * @brief Creation of a date generator
 * @param dateGen un generateur de date associé au générateur aléatoire sera crée
 * @param randGen générateur aléatoire qui servira de base au générateur de date (aléatoire)
 * @result a struct dateGenerator_t * 
 *
 * The created dateGenerator is unusable for now. It needs to be
 * associated to a random generator
 * @return retourne void
*/
void dateGenerator_setRandomGenerator(struct dateGenerator_t * dateGen,
				      struct randomGenerator_t * randGen);

struct dateGenerator_t * dateGenerator_create()
{
  struct dateGenerator_t * result = (struct dateGenerator_t * )
                  sim_malloc(sizeof(struct dateGenerator_t));
  result->interArrivalProbe = NULL;

  result->randGen = NULL;

  return result;
}

/**
 * @fn struct dateGenerator_t * dateGenerator_createExp(double lambda)
 * @brief Création d'une loi avec interarrivé exponentielle
 * @param lambda parametre de la loi exponentielle
 * @result un générateur de date aléatoire de type exponentielle
 * @retrun retourne un générateur de date
 */
struct dateGenerator_t * dateGenerator_createExp(double lambda)
{
  struct dateGenerator_t * result = dateGenerator_create();

  dateGenerator_setRandomGenerator(result, randomGenerator_createDoubleExp(lambda));

  return result;
}

/**
 * @fn struct dateGenerator_t * dateGenerator_createPeriodic(double period)
 * @brief Création d'une loi avec interarrivé constante
 * @param period paramétre de la loi périodique
 * @result un générateur de date aléatoire de type periodique
 * @retrun retourne un générateur de date
 */
struct dateGenerator_t * dateGenerator_createPeriodic(double period)
{
  struct dateGenerator_t * result = dateGenerator_create();
  double un = 1.0;

  dateGenerator_setRandomGenerator(result, randomGenerator_createDoubleDiscreteProba(1, &period, &un));

  return result;
}

/*-------------------------------------------------------------------------*/
/*   Les fonctions générales                                               */
/*-------------------------------------------------------------------------*/
/**
 * @fn void dateGenerator_setRandomGenerator(struct dateGenerator_t * dateGen,
				      struct randomGenerator_t * randGen)
 * @brief Choix du générateur aléatoire des durées entre dates
 *
 * @param dateGen le générateur à modifier
 * @param randGen le générateur de date à affecter
 * @result le générateur aléatoire est modifié
 */
void dateGenerator_setRandomGenerator(struct dateGenerator_t * dateGen,
				      struct randomGenerator_t * randGen)
{
   dateGen->randGen = randGen;
}


/**
 * @fn  double dateGenerator_nextDate(struct dateGenerator_t * dateGen, double currentTime)
 * @brief Obtention de la prochaine date
 *
 *  @param dateGen le générateur à utiliser
 *  @param currentTime la date actuelle
 * @result On obtient la prochaine date du générateur de date aléatoire
 * @return retourne un double prochaine date
 */
double dateGenerator_nextDate(struct dateGenerator_t * dateGen, double currentTime)
{
   double result =  randomGenerator_getNextDouble(dateGen->randGen);

   if (dateGen->interArrivalProbe){
      probe_sample(dateGen->interArrivalProbe, result);
      printf_debug(DEBUG_GENE, " Mean = %6.3f\n", probe_mean(dateGen->interArrivalProbe));
   }

   return currentTime + result ;
  //   return dateGen->nextDate(dateGen, currentTime);
}

/**
 * @fn void dateGenerator_addInterArrivalProbe(struct dateGenerator_t * dateGen, struct probe_t * probe)
 * @brief Insertion d'une sonde sur les inter-arrivees.
 * 
 * @param dateGen le générateur de date sur lequel greffer la sonde
 * @param probe la sonde à y appliquer
 * @result la sonde est inserée
 */
void dateGenerator_addInterArrivalProbe(struct dateGenerator_t * dateGen, struct probe_t * probe)
{
   dateGen->interArrivalProbe = probe_chain(probe, dateGen->interArrivalProbe);
}

/*-------------------------------------------------------------------------*/
/*    Source de dates avec une interarrivee exponentielle.                 */
/*-------------------------------------------------------------------------*/
/*
double loi_expo(struct dateGenerator_t * dateGen, double currentTime)
{
   double alea, result, * data;

   alea = random() / (RAND_MAX + 1.0);

   data = (double*)dateGen->data;

   //   printf("alea = %f, data = %p, data[0] = %f\n", alea, data, data[0]);
   //printf("log(alea) = %f\n", log(alea));
    result = - log(alea) / data[0];

   printf_debug(DEBUG_GENE, " alea = %6.3f, lambda = %6.3f, result = %6.3f\n", alea, ((double*)dateGen->data)[0], result);
   if (dateGen->interArrivalProbe){
      probe_sample(dateGen->interArrivalProbe, result);
      printf_debug(DEBUG_GENE, " Mean = %6.3f\n", probe_mean(dateGen->interArrivalProbe));
   }

   return result + currentTime;
}
*/

/**
 * @fn void dateGenerator_setLambda(struct dateGenerator_t * dateGen, double lambda)
 * @brief Modification du paramètre lambda
 * @param dateGen le générateur de date dont on doit modifier le paramètre lambda
 * @param lambda le nouveau paramètre lambda à définir
 * @result le paramètre lambda est modifié
 */
void dateGenerator_setLambda(struct dateGenerator_t * dateGen, double lambda)
{
   randomGenerator_setLambda(dateGen->randGen, lambda);
}

/**
 * @fn void dateGenerator_recordThenReplay(struct dateGenerator_t *  d)
 * @brief Prepare for record values in order to replay on each reset
 * @param d le générateur de date dont on doit enregistrer est rejouer les actions
 * @result le générateur de date est enregistré
 */
void dateGenerator_recordThenReplay(struct dateGenerator_t *  d){
  randomGenerator_recordThenReplay(d->randGen);
};
