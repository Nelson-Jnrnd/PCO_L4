#ifndef TURN_H
#define TURN_H

#include <QList>
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
    enum Danger{clear, slow, shared};
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

    Danger safety;
public:

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
    void Activate();

    Danger getSafety();
};

#endif // TURN_H
