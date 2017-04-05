/*
** main.c for demoLib in /home/aurelien/Documents/demoLib/libmy_extended
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Wed Apr  5 11:44:31 2017 CASTELLARNAU Aurelien
** Last update Wed Apr  5 13:03:41 2017 CASTELLARNAU Aurelien
*/

#include <stdlib.h>
#include "libmy.h"

/*
** how to use liblist:
*/
int		main()
{
  // Pour créer la liste, il faut déclarer une variable de type t_chain* (cf libmy.h):
  t_chain	*liste_chainee;

  // Pour initialiser la liste:
  // ! gestion d'erreur, create_chain renvoit un pointeur...
  if ((liste_chainee = create_chain()) == NULL)
    return (1);

  // Pour ajouter un maillon:
  // int add_link(t_chain **chain, void *content)
  // gestion d'erreur: add_link renvoit 1 si fail
  if(add_link(&liste_chainee, "élément 1"))
    return (1);
  if(add_link(&liste_chainee, "élément 2"))
    return (1);
  if(add_link(&liste_chainee, "élément 3"))
    return (1);

  // Pour accéder, 2 possibilités:
  // Iteration avec un tmp, utilisation du type t_link*
  t_link	*tmp;
  // on prépare une variable du bon type pour récupérer le content:
  char		*element;
  
  tmp = liste_chainee->first;
  while(tmp)
    {
      // Comme le content est un void*, besoin de l'extraire et de le caster:
      element = ((char *)tmp->content);
      my_putstr("\n A l'index: ");
      my_put_nbr(tmp->index);
      my_putstr(" est l' ");
      my_putstr(element);
      my_putstr("\n");
      tmp = tmp->next;
    }
  // La liste est automatiquement indexée pour permettre un accès en mode Array:
  my_putstr((char*)liste_chainee->dictionnary[0]->content);
  my_putstr("\n");
  my_putstr((char*)liste_chainee->dictionnary[1]->content);
  my_putstr("\n");
  my_putstr((char*)liste_chainee->dictionnary[2]->content);
  my_putstr("\n");

  // Pour supprimer un element:
  // int remove_link(t_chain**, t_link*)
  if (remove_link(&liste_chainee, liste_chainee->dictionnary[1]))
    return (1);
  
  tmp = liste_chainee->first;
  while(tmp)
    {
      // Comme le content est un void*, besoin de l'extraire et de le caster:
      element = ((char *)tmp->content);
      my_putstr("\n A l'index: ");
      my_put_nbr(tmp->index);
      my_putstr(" est l' ");
      my_putstr(element);
      my_putstr("\n");
      tmp = tmp->next;
    }
  // Pour libérer la totalité de la liste:
  // int delete_chain(t_chain*)
  if (delete_chain(liste_chainee))
    return (1);
  return (0);
}
