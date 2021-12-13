/* -----------------------------------------------------------
Laboratoire : 04 - Gestion de ressources
Fichier     : locomotivebehavior.h
Auteur(s)   : Jeanrenaud Nelson, Mirabile Théo
Date        : 06/12/2021

But  : Interface du comportement d'une locomotive
----------------------------------------------------------- */

//    ___  _________    ___  ___  ___   __ //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  | / / //
//  / ___/ /__/ /_/ / / __// // / __/ / /  //
// /_/   \___/\____/ /____/\___/____//_/   //
//

#ifndef LOCOMOTIVEBEHAVIOR_H
#define LOCOMOTIVEBEHAVIOR_H

#include "locomotive.h"
#include "launchable.h"
#include "sharedsectioninterface.h"
#include "track.h"

/**
 * @brief La classe LocomotiveBehavior représente le comportement d'une locomotive
 */
class LocomotiveBehavior : public Launchable
{
public:
    /*!
     * \brief locomotiveBehavior Constructeur de la classe
     * \param loco la locomotive dont on représente le comportement
     */
    LocomotiveBehavior(Locomotive& loco, std::shared_ptr<SharedSectionInterface> sharedSection, Track& track, int nbLaps, bool isClockwise
                       , SharedSectionInterface::LocoId id)
        : loco(loco), sharedSection(sharedSection), track(track), MAX_LAPS(nbLaps), isClockwise(isClockwise), id(id){
        // Initialisation des valieurs par défaut
        currentLapCount = 1;
        wokeUp = false;
        requestSent = false;
    }

protected:
    /*!
     * \brief run Fonction lancée par le thread, représente le comportement de la locomotive
     */
    void run() override;

    /*!
     * \brief printStartMess        code/prog2/prog2.pro.userage Message affiché lors du démarrage du thread
     */
    void printStartMessage() override;

    /*!
     * \brief printCompletionMessage Message affiché lorsque le thread a terminé
     */
    void printCompletionMessage() override;

    /**
     * @brief loco La locomotive dont on représente le comportement
     */
    Locomotive& loco;

    /**
     * @brief sharedSection Pointeur sur la section partagée
     */
    std::shared_ptr<SharedSectionInterface> sharedSection;

    /*!
     * \brief Cette fonction détermine les actions à effectuer en fonction du niveau de danger du virage courant
     * \param currentTurn le virage courant pour lequel il faut analyser les dangers
     */
    void manageDangers(Turn* currentTurn);

    /**
     * @brief Tour effectué par la locomotive
     */
    Track track;

    /**
     * @brief Compteur des tours effectués
     */
    int currentLapCount;

    /**
     * @brief Nombre de tours à effectuer avant de changer de sens
     */
    const int MAX_LAPS;

    /**
     * @brief Détermine si la locomotive tourne dans un sens horaire ou non
     */
    bool isClockwise;

    /**
     * @brief Détermine si la locomotive a redémarré après avoir dû attendre
     */
    bool wokeUp;

    /**
     * @brief Détermine si une requête d'accès à la section partagée a été envoyée ou non
     */
    bool requestSent;

    /**
     * @brief Identifiant de la locomotive pour accès à la section partagée
     */
    SharedSectionInterface::LocoId id;
};

#endif // LOCOMOTIVEBEHAVIOR_H
