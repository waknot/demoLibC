/*
** create_list.c for libmy_extended in /home/aurelien/Documents/Lib_etna/castel_a/libmy_extended/libmy
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Fri Feb  3 02:13:19 2017 CASTELLARNAU Aurelien
** Last update Thu Feb  9 22:15:01 2017 CASTELLARNAU Aurelien
*/

#include <stdlib.h>
#include "libmy.h"

t_chain		*create_chain()
{
  t_chain	*chain;

  if ((chain = malloc(sizeof(*chain))) == NULL)
    return (NULL);
  chain->first = NULL;
  chain->last = NULL;
  chain->index = 0;
  chain->dictionnary = NULL;
  return (chain);
}

void	init_chain(t_chain **chain, t_link *link)
{
  (*chain)->first = link;
  (*chain)->last = link;
}

int		add_link(t_chain **chain, void *content)
{
  t_link	*link;

  if ((link = malloc(sizeof(*link))) == NULL)
    return (1);
  link->index = (*chain)->index;
  link->content = content;
  link->prev = (*chain)->last;
  if ((*chain)->first == NULL)
    {
      init_chain(chain, link);
      init_index((*chain));
    }
  else
    (*chain)->last->next = link;
  link->next = NULL;
  (*chain)->last = link;
  if (add_to_index((*chain), link))
    return (1);
  (*chain)->index++;
  return (0);
}

int		remove_link(t_chain **chain, t_link *link)
{
  if (remove_from_index((*chain), link))
      return (1);
  if (link == (*chain)->first && link == (*chain)->last)
    {
      free((*chain)->dictionnary);
      free((*chain));
      (*chain) = NULL;
    }
  else if (link == (*chain)->first)
    {
      (*chain)->first->next->prev = NULL;
      (*chain)->first = (*chain)->first->next;
    }
  else if (link == (*chain)->last)
    {
      (*chain)->last->prev->next = NULL;
      (*chain)->last = (*chain)->last->prev;
    }
  else
    {
      link->next->prev = link->prev;
      link->prev->next = link->next;
    }
  if ((*chain) != NULL)
    (*chain)->index--;
  free(link);
  return (0);
}

int	delete_chain(t_chain *chain)
{
  while (chain != NULL)
    {
      if (remove_link(&chain, chain->last))
	return (1);
    }
  return (0);
}
