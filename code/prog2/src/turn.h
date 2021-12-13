/* -----------------------------------------------------------
Laboratoire : 04 - Gestion de ressources
Fichier     : turn.h
Auteur(s)   : Jeanrenaud Nelson, Mirabile Théo
Date        : 06/12/2021

But  : Interface du système de virages (ou segments)
----------------------------------------------------------- */

#ifndef TURN_H
#define TURN_H

#include <QList>
#include "sharedsectioninterface.h"
/*!
 * \brief Représente un aiguillage à ajuster pour permettre au train de passer
 */
struct Switch{
    /*!
     * \brief l'angle dans lequel il faut mettre l'aiguillage
     */
    int angle;
    /*!
     * \brief l'identifiant de l'aiguillage
     */
    int number;
};

/*!
 * \brief Virage d'un tour
 */
class Turn
{
public:
    enum Danger{clear, requestEA, requestEB, shared};
private:
    /*!
     * \brief contact d'entrée dans le virage dans le sens horaire
     */
    int contact1;
    /*!
     * \brief contact de sortie du le virage dans le sens horaire
     */
    int contact2;
    /*!
     * \brief Les aiguillages à ajuster pour prendre le virage
     */
    QList<Switch> switches;

    /*!
     * \brief Les éventuels dangers à prendre en compte dans ce virage
     */
    Danger safety;
public:

    /**
     * @brief Définit un nouveau virage
     * @param contact1 contact d'entrée
     * @param contact2 contact de sortie
     * @param switches liste des aiguillages présents dans le virage
     * @param safety niveau de danger du virage
     */
    Turn(int contact1, int contact2, QList<Switch> switches, Danger safety);

    /*!
     * \brief retourne l'identifiant du contact d'entrée du virage
     * \param isClockwise point de vue du virage
     * \return l'identifiant du contact d'entrée
     */
    int getEntry(bool isClockwise);

    /*!
     * \brief retourne l'identifiant du contact de sortie du virage
     * \param isClockwise point de vue du virage
     * \return l'identifiant du contact de sortie
     */
    int getExit(bool isClockwise);

    /*!
     * \brief Modifie la position des aiguillages pour permettre
     * à la locomotive de prendre le virage
     */
    void activate();

    /*!
     * \brief Retourne le point d'entrée de la section critique selon le sens de rotation
     * \param isClockwise le sens de rotation (horaire ou anti-horaire)
     * \return le point d'entrée
     */
    SharedSectionInterface::EntryPoint getEntryPoint(bool isClockwise);

    /*!
     * \brief Retourne le danger éventuel à prendre en compte dans ce virage
     */
    Danger getSafety();
};

#endif // TURN_H
