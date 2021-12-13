/* -----------------------------------------------------------
Laboratoire : 04 - Gestion de ressources
Fichier     : sharedsection.h
Auteur(s)   : Jeanrenaud Nelson, Mirabile Théo
Date        : 06/12/2021

But  : Implémentation du fonctionnement de la section partagée
----------------------------------------------------------- */

//    ___  _________    ___  ___  ___   __ //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  | / / //
//  / ___/ /__/ /_/ / / __// // / __/ / /  //
// /_/   \___/\____/ /____/\___/____//_/   //
//                                         //

#ifndef SHAREDSECTION_H
#define SHAREDSECTION_H

#include <QDebug>

#include <pcosynchro/pcomutex.h>
#include <pcosynchro/pcosemaphore.h>

#include "locomotive.h"
#include "ctrain_handler.h"
#include "sharedsectioninterface.h"

/**
 * @brief La classe SharedSection implémente l'interface SharedSectionInterface qui
 * propose les méthodes liées à la section partagée.
 */
class SharedSection final : public SharedSectionInterface
{
public:

    /**
     * @brief SharedSection Constructeur de la classe qui représente la section partagée.
     * Initialisez vos éventuels attributs ici, sémaphores etc.
     */
    SharedSection() :requests(QList<Request>()), isAvailable(true), s_synchro(0), m_available(), m_blocked() {}

    /**
     * @brief request Méthode a appeler pour indiquer que la locomotive désire accéder à la
     * section partagée (deux contacts avant la section partagée).
     * @param loco La locomotive qui désire accéder
     * @param locoId L'identidiant de la locomotive qui fait l'appel
     * @param entryPoint Le point d'entree de la locomotive qui fait l'appel
     */
    void request(Locomotive& loco, LocoId locoId, EntryPoint entryPoint) override {
        // On rajoute notre requête à la liste des requêtes passées
        requests.append({locoId, entryPoint});
        afficher_message(qPrintable(QString("The engine no. %1 requested the shared section.").arg(loco.numero())));
    }

    /**
     * @brief getAccess Méthode à appeler pour accéder à la section partagée, doit arrêter la
     * locomotive et mettre son thread en attente si la section est occupée ou va être occupée
     * par une locomotive de plus haute priorité. Si la locomotive et son thread ont été mis en
     * attente, le thread doit être reveillé lorsque la section partagée est à nouveau libre et
     * la locomotive redémarée. (méthode à appeler un contact avant la section partagée).
     * @param loco La locomotive qui essaie accéder à la section partagée
     * @param locoId L'identidiant de la locomotive qui fait l'appel
     */
    void getAccess(Locomotive &loco, LocoId locoId) override {

        bool expected = true;

        // Tant que la section partagée n'est pas disponible
        while (!isAvailable.compare_exchange_strong(expected, false)) {
            afficher_message(qPrintable(QString("The engine no. %1 block shared already used.").arg(loco.numero())));

            // Section utilisée on bloque...
            loco.arreter();
            m_blocked.lock();
            blocked = true;
            m_blocked.unlock();
            s_synchro.acquire();

            // Après relâchement du sémaphore, on relance la locomotive
            loco.demarrer();
            expected = true;

        }

        int ourRequestid = -1;

        // On récupère ensuite notre requête
        for(int id = 0; id < requests.count(); ++id){
            if(requests.at(id).id == locoId){
                ourRequestid = id;
            }
        }

        if (ourRequestid != -1) {

            Request ourRequest = requests.at(ourRequestid);

            // On parcourt toutes les autres requêtes effectuées
            for(int id = 0; id < requests.count(); ++id){

                // Pour chaque requête existante
                Request rq = requests.at(id);

                // On vérifie si elle n'a pas été envoyée par nous
               if (rq.id != locoId) {

                   // Si la requête a été faite par la locomotive LA et que son point d'entrée est différent du nôtre
                    if (locoId == LocoId::LA and rq.entry != ourRequest.entry) {

                        afficher_message(qPrintable(QString("The engine no. %1 block not same entry.").arg(loco.numero())));

                        // On rend la section partagée disponible pour les autres
                        m_available.lock();
                        isAvailable = true;
                        m_available.unlock();

                        // On s'arrête car nous n'avons pas la priorité
                        loco.arreter();
                        m_blocked.lock();
                        blocked = true;
                        m_blocked.unlock();
                        s_synchro.acquire();

                        // Après relâchement du sémaphore, on redémarre et on occupe la section partagée
                        loco.demarrer();
                        m_available.lock();
                        isAvailable = false;
                        m_available.unlock();

                    // Si la requête a été faite par la locomotive LB et que son point d'entrée et le même que le nôtre
                    } else if (locoId == LocoId::LB and rq.entry == ourRequest.entry) {


                        afficher_message(qPrintable(QString("The engine no. %1 block same entry.").arg(loco.numero())));

                        // On rend la section partagée disponible pour les autres
                        m_available.lock();
                        isAvailable = true;
                        m_available.unlock();

                        // On s'arrête car nous n'avons pas la priorité
                        loco.arreter();
                        m_blocked.lock();
                        blocked = true;
                        m_blocked.unlock();
                        s_synchro.acquire();

                        // Après relâchement du sémaphore, on redémarre et on occupe la section partagée
                        loco.demarrer();
                        m_available.lock();
                        isAvailable = false;
                        m_available.unlock();

                    // Sinon, nous sommes seuls, nous pouvons y aller
                    } else {
                        afficher_message(qPrintable(QString("The engine no. %1 no blocks").arg(loco.numero())));
                    }
                }
            }
            // La requête ayant été traitée, nous pouvons la retirer
            requests.removeAt(ourRequestid);
        } else {
            // Notre requête n'a pas été trouvée...
            afficher_message(qPrintable(QString("The engine no. %1 can't find my request").arg(loco.numero())));
        }
        // On devrait être seul ici
        afficher_message(qPrintable(QString("The engine no. %1 accesses the shared section.").arg(loco.numero())));
    }

    /**
     * @brief leave Méthode à appeler pour indiquer que la locomotive est sortie de la section
     * partagée. (reveille les threads des locomotives potentiellement en attente).
     * @param loco La locomotive qui quitte la section partagée
     * @param locoId L'identidiant de la locomotive qui fait l'appel
     */
    void leave(Locomotive& loco, LocoId locoId) override {
        // On rend la section partagée a nouveau disponible
        m_available.lock();
        isAvailable = true;
        m_available.unlock();

        // Si une locomotive était bloquée, on relâche le sémaphore pour la débloquer
        m_blocked.lock();
        if(blocked){
            m_blocked.unlock();
            s_synchro.release();

            m_blocked.lock();
            blocked = false;
            m_blocked.unlock();
        }

        afficher_message(qPrintable(QString("The engine no. %1 leaves the shared section.").arg(loco.numero())));
    }


private:

    // Structure d'une requête
    struct Request{
        LocoId id;          // ID de la locomotive l'ayant passée
        EntryPoint entry;   // Point d'entrée duquel on essaie d'accéder
    };

    // Liste des requêtes d'accès passées
    QList<Request> requests;

    // Détermine si une locomotive tentant d'accéder est bloquée ou non
    bool blocked = false;

    // Détermine si la section partagée est libre ou non
    std::atomic<bool> isAvailable;

    // Sémaphore permettant l'attente et la synchronisation des locomotives
    PcoSemaphore s_synchro;

    // Verrou protégeant la variable partagée isAvailable
    PcoMutex m_available;

    // Verrou protégeant la variable partagée blocked
    PcoMutex m_blocked;
};


#endif // SHAREDSECTION_H
