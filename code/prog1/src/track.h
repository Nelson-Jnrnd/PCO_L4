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
public:
    Track();
    /*!
     * \brief Ajoute un virage au tour
     */
    void AddTurn(int contact1, int contact2, QList<Switch> switches, Turn::Danger safety);
    /*!
     * \brief retourne le virage suivant à négocier pour la locomotive
     * \param isClockwise
     * \return
     */
    Turn* nextTurn(bool isClockwise);

    Turn* peekNextTurn(bool isClockwise);
    /*!
     * \brief Doit être appelé après avoir terminé un tour, prépare le tour suivant
     * \param isClockwise
     */
    void newLap(bool isClockwise);
};

#endif // TRACK_H
