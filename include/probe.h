/**
 * @file probe.h
 * @brief Gestion des probes
 */
#ifndef __DEF_PROBE
#define __DEF_PROBE

#include <pdu.h>
#include <pdu-filter.h>
/**
 * @ brief definition de la structure d'une sonde
*/
struct probe_t;

/**
 * @brief Définition des types de prob
 * @param exhaustiveProbeType  Conserve tous les échantillons
 * @param meanProbeType  Conserve la moyenne
 * @param timeSliceAverageProbeType  Conserve des moyennes temporelles
 * @param timeSliceThroughputProbeType  Conserve le débit moyen par tranche de temps
 * @param graphBarProbeType  Conserve un histogramme
 * @param EMAProbeType  Exponential Moving Average AFAIRE
 * @param slidingWindowProbeType  Conserve une fenêtre de valeurs AFAIRE
 * @param periodicProbeType  Enregistre périodiquement une valeur
*/
enum probeType_t {
   exhaustiveProbeType,           // Conserve tous les échantillons
   meanProbeType,                 // Conserve la moyenne
   timeSliceAverageProbeType,     // Conserve des moyennes temporelles
   timeSliceThroughputProbeType,  // Conserve le débit moyen par tranche de temps
   graphBarProbeType,             // Conserve un histogramme
   EMAProbeType,                  // Exponential Moving Average AFAIRE
   slidingWindowProbeType,        // Conserve une fenêtre de valeurs AFAIRE
   periodicProbeType              // Enregistre périodiquement une valeur
};


#define probeTypeName(t) \
(t == exhaustiveProbeType)?"exhaustive":(\
(t == meanProbeType)?"mean":(\
(t == timeSliceAverageProbeType)?"timeSliceAverage":(\
(t == timeSliceThroughputProbeType)?"timeSliceThroughput":(\
(t == graphBarProbeType)?"graphBar":(\
(t == EMAProbeType)?"EMA":(\
(t == periodicProbeType)?"periodic":(\
(t == slidingWindowProbeType)?"slidingWindow":"???"))))))) 

/*
 * Pour le moment, c'est forcément des doubles
 */
/**
 * @fun struct probe_t * probe_createExhaustive()
 * @brief Création d'une sonde exhaustive
 * Attention, une telle sonde conserve tous les échantillons
 * @return retourne une sonde de type probe_t 
 */
struct probe_t * probe_createExhaustive();  

/**
 * @fun struct probe_t * probe_slidingWindowCreate(int windowLength)
 * @brief Une telle sonde conserve des échantillons sur une fenêtre
 * @param windowLength nombre d'échantillons conservés
 */
struct probe_t * probe_slidingWindowCreate(int windowLength);

/**
 * @fun struct probe_t * probe_createMean()
 * @brief Ne conserve aucun échantillon, juste la somme et le nombre
 * @return retourne une structure de probe
*/
struct probe_t * probe_createMean();

/**
 * @fun struct probe_t * probe_createTimeSliceAverage(double t)
 * @brief Conserve une moyenne sur chaque tranche temporelle de durée t
 * @param t durée de la tranche temporelle
 * @return une probe_t conservant la moyenne sur chaque tranche temporelle de durée t
*/
struct probe_t * probe_createTimeSliceAverage(double t);

/**
 * @fun struct probe_t * probe_createTimeSliceThroughput(double t)
 * @brief Conserve un débit moyen par tranche temporelle de durée t
 * @param t durée de la tranche temporelle
 * @return une probe_t conservant un débit moyen par tranche temporelle de durée t
*/
struct probe_t * probe_createTimeSliceThroughput(double t);

/** 
 * @brief Création d'une sonde périodique
 * @param t Période de l'échantillonage
 *
 * Une telle sonde conserve un échantillon à la fin de chaque tranche
 * temporelle de durée t
 * @return  Une sonde conservant un échantillon à la fin de chaque tranche
 * temporelle de durée t
 */
struct probe_t * probe_periodicCreate(double t);

/**
 * @fun struct probe_t * probe_EMACreate(double a)
 * @brief Conserve une moyenne mobile M <- a.M + (1-a).sample
 * @param a coefficient de moyennage a.M + (1-a).sample
 * @return une probe conservant une moyenne mobile
*/
struct probe_t * probe_EMACreate(double a);

/**
 * @brief Création d'une sonde qui compte le nombre d'occurences par
 * intervalle
 * @param min Minimal value
 * @param max Maximal value
 * @param nbInt Number of bars
 * @result Created graphBar probe
 */
struct probe_t * probe_createGraphBar(double min,
				      double max,
				      unsigned long nbInt);

/**
 * @fun void probe_graphBarNormalize(struct probe_t * pr)
 * @brief Normalization of a graphBar probe
 * @param pr la probe a normaliser
 * @resulter pr la probe est normalisée
 */
void probe_graphBarNormalize(struct probe_t * pr);

/**
 * @fun void probe_delete(struct probe_t * p)
 * @brief Destruction d'une probe
 * @param p probe à détruire
 * @result la probe p est détruite
 */
void probe_delete(struct probe_t * p);

/**
 * @fun struct probe_t * probe_chain(struct probe_t * p1, struct probe_t * p2)
 * @brief Chaînage des probes p1 et p2, dans cet ordre. Tout échantillon sur
 * p1 sera répercuté sur p2. C'est la seule méthode qui soit
 * répercutée en cascade. Les reset, calcul de moyenne, ... doivent
 * être invoquées sur chaque sonde si nécessaire
 * @param p1 probe à chainer
 * @param p2 probe à chaîner
 * @result p1 et p2 sont chaînées
 */
struct probe_t * probe_chain(struct probe_t * p1, struct probe_t * p2);

/**
 * @fun void probe_reset(struct probe_t * probe)
 * @brief Réinitialisation d'une probe (pour permettre de relancer une
 * simulation dans les mêmes conditions). Tout est effacé et doit donc
 * avoir été sauvegardé si besoin.
 * @param probe à  réinitialiser
 * @result la probe est réinitilisée
 */
void probe_reset(struct probe_t * probe);

/**
 * @fun void probe_setPersistent(struct probe_t * p)
 * @brief Define a probe as persistent
 * Une sonde persistante ne sera pas réinitialisée en cas de reset (en
 * fin de simulation)
 * @param p probe à rendre persistant
 * @result la probe est devenue persistante
 */
void probe_setPersistent(struct probe_t * p);

/**
 * @fun void probe_resetAllProbes()
 * @brief réinitialiser toutes les probes
 * @result tout les probes sont réinitialisées
*/
void probe_resetAllProbes();

/**
 * @fun void probe_setName(struct probe_t * p, char * name)
 * @brief Modification du nom, il est copié depuis le paramètre
 * qui peut donc être détruit ensuite
 * @param p probe dont on doit changer le nom
 * @param name nouveau nom de la probe
 * @result la probe a changé de nom
 */
void probe_setName(struct probe_t * p, char * name);

/**
 * @fun char * probe_getName(struct probe_t * p)
 * @brief Lecture du nom. C'est un pointeur sur le nom qui
 * est retourné, il doit donc être copié avent toute
 * modification/destruction.
 * @param p probe dont on veut savoir le nom
 * @return on obtient le nom de la probe
 */
char * probe_getName(struct probe_t * p);

/**
 * @fun void probe_sample(struct probe_t * probe, double value)
 * @brief Echantillonage d'une valeur
 * La probe peut être NULL, auquel cas rien n'est enregistré,
 * naturellement
 * @param probe La sonde dans laquelle on veut enregistrer
 * @param value La valeur à enregistrer
 * @result la valeur est enregistrée dans la sonde
 */
void probe_sample(struct probe_t * probe, double value);

/*****************************************************************************
       Probes and filters
 */

/**
 * @fun void probe_setFilter(struct probe_t * probe, struct PDUFilter_t * filter)
 * @brief Set a filter to a probe
 *
 * If the probe is used through probe_sampleValuePDUFilter, the sample
 * will be done iif the PDU validates the filter
 * @param probe la sonde dont on veut munir d'un filtre
 * @param filter filtre qui sera associé à la sonde
 * @result le filtre a été associé à la sonde
 */
void probe_setFilter(struct probe_t * probe, 
		     struct PDUFilter_t * filter);

/**
 * @brief Echantillonage d'une valeur
 * La probe peut être NULL, auquel cas rien n'est enregistré,
 * naturellement.
 * Attention, l'échantillonage se fera SI ET SEULEMENT SI la PDU passe
 * le fitre.
 * @param probe La sonde dans laquelle on veut enregistrer
 * @param value La valeur à enregistrer
 * @param pdu une PDU sur laquelle le filtre sera appliqué
 * @result la valeur a été enregistrée dans la sonde
 */
void probe_sampleValuePDUFilter(struct probe_t * probe, 
			   double value, struct PDU_t* pdu);

/**
 * @fun void probe_sampleEvent(struct probe_t * probe)
 * @brief Echantillonage de la date d'occurence d'un evenement
 * @param probe dont on veut echantillonner la date d'occurence d'un événement
 * @result la date d'occurence de l'événement a été échantillonné
 */
void probe_sampleEvent(struct probe_t * probe);

/**
 * @fun double probe_exhaustiveGetSample(struct probe_t * probe, unsigned long n)
 * @brief Lecture d'un echantillon
 * @param probe sonde dont on veut lire un échantillon
 * @param n choisier l'élément n de l'échantillon
 * @return retourne le nieme element de l'échantillon
 */
double probe_exhaustiveGetSample(struct probe_t * probe, unsigned long n);

/*****************************************************************************
       Consultation des métriques offertes par une sonde
 */

/**
 * @fun unsigned long probe_nbSamples(struct probe_t * probe)
 * @brief Nombre d'echantillons
 * @return donne le nombre d'échantillons de la sonde
 */
unsigned long probe_nbSamples(struct probe_t * probe);

/**
 * @fun double probe_max(struct probe_t * probe)
 * @brief Maximal sampled value
 * @return l'échantillon de valeur maximale
 */
double probe_max(struct probe_t * probe);

/**
 * @fun double probe_min(struct probe_t * probe)
 * @brief Minimal sampled value
 * @return l'échantillon de valeur minimal
 */
double probe_min(struct probe_t * probe);

/**
 * @fun double probe_mean(struct probe_t * probe)
 * @brief Valeur moyenne, variance, écart type, ... empriques !
 * @return retourne la valeur moyenne
 */
double probe_mean(struct probe_t * probe);

/**
 * @fun double probe_variance(struct probe_t * probe)
 * @brief Valeur variance
 * @param probe à étudier
 * @return retourne la valeur de la variance
 */
double probe_variance(struct probe_t * probe);

/**
 * @fun double probe_stdDev(struct probe_t * probe)
 * @brief Valeur écart type
 * @param probe à étudier
 * @return retourne la valeur de la déviation standard
 */
double probe_stdDev(struct probe_t * probe);

/**
 * @fun double probe_coefficientOfVariation(struct probe_t * probe)
 * @brief Experimental coefficient of variation
 * @param probe à étudier
 * @return The experimental coefficient of variation
 */
double probe_coefficientOfVariation(struct probe_t * probe);

/**
 * @fun double probe_demiIntervalleConfiance5pc(struct probe_t * p)
 * @brief Demi largeur de l'intervalle de confiance à 5%
 * @param p à étudier
 * @return l'intervalle de confiance à 5% de la sonde
 */
double probe_demiIntervalleConfiance5pc(struct probe_t * p);

/**
 * @fun double probe_demiIntervalleConfiance5pcCoupes(struct probe_t * p)
 * @brief Tentative de calcul de l'IC à 5% par la méthode des coupes. C'est
 * très probablement faux ! Combien de blocs de quelle taille par
 * exemple ?
 * @param p la sonde a étudier
 * @return la valeur de l'intervalle de confiance 5% par la méthode des coupes
 */
double probe_demiIntervalleConfiance5pcCoupes(struct probe_t * p);

/**
 * @fun double probe_IAMean(struct probe_t * probe)
 * @brief Moment d'ordre 1 de la loi d'inter-arrivée des événements de sondage
 * @param probe la sonde à étudier
 * @ return la valeur du moment d'ordre 1 des inter-arrivée des événements de sondage
 */
double probe_IAMean(struct probe_t * probe);

/**
 * @fun double probe_IAVariance(struct probe_t * probe)
 * @brief Moment d'ordre 2 de la loi d'inter-arrivée des événements de sondage
 * @param probe la sonde à étudier
 * @ return la valeur du moment d'ordre 2 des inter-arrivée des événements de sondage
 */
double probe_IAVariance(struct probe_t * probe);

/**
 * @fun double probe_IADev(struct probe_t * probe)
 * @brief Moment d'ordre 2 de la loi d'inter-arrivée des événements de sondage
 * @param probe la sonde à étudier
 * @ return la valeur de la déviation standard des inter-arrivée des événements de sondage
 */
double probe_IAStdDev(struct probe_t * probe);

/**
 * @fun double probe_throughput(struct probe_t * p)
 * @brief Consultation du débit. On considère ici chaque nouvelle valeur
 * comme la taille d'une nouvelle PDU. La fonction suivante permet
 * alors de connaitre le débit qui en  découle. 
 *
 * Il s'agit d'une valeur "instantanée"
 *
 * La méthode de calcul est évidemment dépendante de la nature de la
 * sonde et sa précision est donc variable
 *
 * - Fenêtre glissante : le débit est, à tout moment, le rapport entre
 * la taille cumulée reçue et la durée depuis la première PDU reçue
 * non inclue pour la taille).
 * - Fenêtre temporelle : rapport entre la taille reçue durant la
 * dernière  fenêtre révolue et sa durée
 * @param p sonde à etudier
 * @return on retourne le débit
 */
double probe_throughput(struct probe_t * p);

/**
 * @fun double probe_exhaustiveGetSampleN(struct probe_t * probe, int n)
 * @brief Reading the nth sample in an exhaustive probe
 * @param probe sonde à étudier
 * @param n nieme élément de l'échantillon de la probe exhaustive
 * @return le nieme élément de la probe exhaustive
 */
double probe_exhaustiveGetSampleN(struct probe_t * probe, int n);

/**
 * @fun void probe_exhaustiveToGraphBar(struct probe_t * ep, struct probe_t * gbp)
 * @brief Conversion d'une sonde exhaustive en une graphBar
 * @param ep sonde exhaustive
 * @param gbp graphbar
 * @result la sonde est convertie en graphBar
 */
void probe_exhaustiveToGraphBar(struct probe_t * ep, struct probe_t * gbp);

/**
 * @fun void probe_exhaustiveToBlockMean(struct probe_t * ep, struct probe_t * bmp, unsigned long blockSize)
 * @brief Réduction du nombre d'échantillons d'une sonde exhaustive en
 * remplaçant blockSize échantillons consécutifs par leur moyenne
 * @param ep sonde exhaustive etudié
 * @param bmp sonde qui sera modifié
 * @result on a réduit le nombre d'échantillons d'une sonde exhaustive en
 * remplaçant blockSize échantillons consécutifs par leur moyenne
 */
void probe_exhaustiveToBlockMean(struct probe_t * ep, struct probe_t * bmp, unsigned long blockSize);

#define dumpGnuplotFormat 1

/**
 * @fun void probe_dumpFd(struct probe_t * probe, int fd, int format);
 * @brief Dump d'une sonde dans un fichier
 * @param probe sonde à etudier
 * @param fd paramètre du fichier
 * @param format paramètre du fichier
 * @result La sonde est dumpée dans un fichier
 */
void probe_dumpFd(struct probe_t * probe, int fd, int format);

/**
 * @fun void probe_graphBarDumpFd(struct probe_t * probe, int fd, int format);
 * @brief Dump d'une graphBar dans un fichier
 * @param probe sonde à etudier
 * @param fd paramètre du fichier
 * @param format paramètre du fichier
 * @result Le graphBar est dumpé dans un fichier
 */
void probe_graphBarDumpFd(struct probe_t * probe, int fd, int format);

/**
 * @fun int probe_graphBarGetMinValue(struct probe_t * probe)
 * @brief obtenir la valeur minimale de la graphBar
 * @param probe la sonde à étudier
 * @return la valeur minimale de la graphBar est renvoyée
*/
int probe_graphBarGetMinValue(struct probe_t * probe);

/**
 * @fun int probe_graphBarGetMaxValue(struct probe_t * probe)
 * @brief obtenir la valeur maximale de la graphBar
 * @param probe la sonde à étudier
 * @return la valeur maximale de la graphBar est renvoyée
*/
int probe_graphBarGetMaxValue(struct probe_t * probe);

/**
 * @fun int probe_graphBarGetValue(struct probe_t * probe)
 * @brief obtenir la valeur n de la graphBar
 * @param n la valeur n de la graphBar à renvoyer
 * @param probe la sonde à étudier
 * @return la valeur n de la graphBar est renvoyée
*/
int probe_graphBarGetValue(struct probe_t * probe, int n);

/*
 * Chaîner la nouvelle probe p2 dans une liste débutant par p1 qui
 * peut être nul 
 * 
 *   p1 <- p2 suivi de p1
 */
#define addProbe(p1, p2) {assert(p2 != NULL) ; p2->nextProbe = p1;p1 = p2;}

/*
 * Les méta sondes !!
 * 
 * La sonde p2 observe une valeur de la sonde 1. p2 sera typiquement
 * une sonde périodique et p1 une sonde non exhaustive !
 */
// Une sonde systématique. Ceci peut être utile lorsque p2 collecte
// les échantillons de plusieurs sondes
void probe_addSampleProbe(struct probe_t * p1, struct probe_t * p2);

// Une sonde sur la moyenne
void probe_addMeanProbe(struct probe_t * p1, struct probe_t * p2);

// Une sonde sur le débit
void probe_addThroughputProbe(struct probe_t * p1, struct probe_t * p2);

/*
 * Nombre maximal d'echantillons dans un set. Ca n'a pas lieu d'être
 * public a priori, mais c'est pratique pour certains tests de debogage
 */
#define PROBE_NB_SAMPLES_MAX 32768


#endif

