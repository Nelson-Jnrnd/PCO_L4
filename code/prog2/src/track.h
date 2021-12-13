/* -----------------------------------------------------------
Laboratoire : 04 - Gestion de ressources
Fichier     : track.h
Auteur(s)   : Jeanrenaud Nelson, Mirabile Théo
Date        : 06/12/2021

But  : Interface du système de tours (tracks)
----------------------------------------------------------- */

#ifndef TRACK_H
#define TRACK_H

#include <QList>
#include "turn.h"

/**
 * @brief Représente un tour complet de la locomotive
 */
class Track
{
    /*!
     * \brief L'ensemble des virages que la locomotive doit prendre
     * pour faire un tour complet
     */
    QList<Turn*> turns;
    /*!
     * \brief iterateur sur le virage où est la locomotive
     */
    QListIterator<Turn*> currentTurn;

    /*!
     * \brief Détermine si le tour courant est initialisé ou non
     */
    bool isInit;
public:
    Track();
    ~Track();

    /*!
     * \brief Ajoute un nouveau virage au tour courant
     * \param contact1 contact d'entrée
     * \param contact2 contact de sortie
     * \param switches liste des aiguillages contenus dans le virage
     * \param safety niveau de danger du virage
     */
    void addTurn(int contact1, int contact2, QList<Switch> switches, Turn::Danger safety);
    /*!
     * \brief retourne le virage suivant à négocier pour la locomotive
     * \param isClockwise sens de rotation du parcours
     * \return
     */
    Turn* nextTurn(bool isClockwise);

    /*!
     * \brief Retourne les informations du virage suivant sans pour autant avancer dans le parcours
     * \param isClockwise sens de rotation du parcours
     * \return
     */
    Turn* peekNextTurn(bool isClockwise);

    /*!
     * \brief Retourne les informations du virage suivant si l'on effectue un demi-tour
     * \param isClockwise sens de rotation du parcours
     * \return
     */
    Turn* peekNextTurnLoopBack(bool isClockwise);
    /*!
     * \brief Doit être appelé après avoir terminé un tour, prépare le tour suivant
     * \param isClockwise
     */
    void newLap(bool isClockwise);

    /*!
     * \brief Initialise un nouveau tour
     * \param isClockwise définit si le tour est dans le sens horaire ou non
     */
    void initTrack(bool isClockwise);

    /*!
     * \brief Retourne le numéro du virage dans le parcours
     * \param turn le virage en question
     * \return
     */
    int getTurnNo(Turn* turn);

    /*!
     * \brief Retourne le nombre de virages dans le tour
     * \return
     */
    int getNbTurns();
};

#endif // TRACK_H
