#include <stdio.h>     // printf
#include <stdlib.h>    // Malloc, NULL, exit...
#include <math.h>      // log

#include <motsim.h>
#include <probe.h>
#include <random-generator.h>

struct dateGenerator_t {
   struct randomGenerator_t *randGen;
   struct probe_t * interArrivalProbe;

  //   double (* nextDate)(struct dateGenerator_t * dateGen, double currentTime);
  //   void           * data;
};

/*-------------------------------------------------------------------------*/
/*   Les fonctions générales                                               */
/*-------------------------------------------------------------------------*/
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

/*
 * Ajout d'une sonde sur les inter-arrivees
 */
void dateGenerator_setInterArrivalProbe(struct dateGenerator_t * dateGen, struct probe_t * probe)
{
  printf("Salut les mecs !\n");
   dateGen->interArrivalProbe = probe;
  printf("OK pour moi !\n");

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

/*
 * Création d'une loi avec interarrivé exponentielle
 */
struct dateGenerator_t * dateGenerator_createExp(double lambda)
{
  struct dateGenerator_t * result = (struct dateGenerator_t * )
                  sim_malloc(sizeof(struct dateGenerator_t));
  result->interArrivalProbe = NULL;

  result->randGen = randomGenerator_createDoubleExp(lambda);

  return result;
}

/*
 * Modification du paramètre lambda
 */
void dateGenerator_setLambda(struct dateGenerator_t * dateGen, double lambda)
{
   randomGenerator_setLambda(dateGen->randGen, lambda);
}

/*
 * Création d'une loi avec interarrivé constante
 */
struct dateGenerator_t * dateGenerator_createPeriodic(double period)
{
  double un = 1.0;
  struct dateGenerator_t * result = (struct dateGenerator_t * )
                  sim_malloc(sizeof(struct dateGenerator_t));
  result->interArrivalProbe = NULL;

  result->randGen = randomGenerator_createDoubleDiscreteProba(1, &period, &un);

  return result;
}


/*
 * Prepare for record values in order to replay on each reset
 */
void dateGenerator_recordThenReplay(struct dateGenerator_t *  d){
  randomGenerator_recordThenReplay(d->randGen);
};
