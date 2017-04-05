/*
** indexor.c for libmy_extended in /home/aurelien/Documents/Lib_etna/castel_a/libmy_extended
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Fri Feb  3 03:21:04 2017 CASTELLARNAU Aurelien
** Last update Thu Feb 23 23:13:30 2017 CASTELLARNAU Aurelien
*/

#include <stdlib.h>
#include "libmy.h"

int	init_index(t_chain *chain)
{
  if ((chain->dictionnary = malloc(sizeof(*(chain->first)))) == NULL)
    return (1);
  chain->dictionnary[chain->index] = chain->first;
  chain->first->index = chain->index;
  return (0);
}

int		add_to_index(t_chain *chain, t_link *link)
{
  int		i;
  t_link	**tmp;

  i = chain->first->index;
  if ((tmp = malloc((1 + chain->index) * sizeof(*link))) == NULL)
    return (1);
  while (i < chain->index)
    {
      tmp[i] = chain->dictionnary[i];
      tmp[i]->index = i;
      ++i;
    }
  tmp[chain->index] = link;
  free(chain->dictionnary);
  chain->dictionnary = NULL;
  chain->dictionnary = tmp;
  return (0);
}

int		remove_from_index(t_chain *chain, t_link *link)
{
  int		index;
  t_link	**tmp;

  tmp = NULL;
  if ((tmp = malloc((chain->index + 1) * sizeof(*link))) == NULL)
    return (1);
  index = 0;
  while (index < chain->index)
    {
      tmp[index] = chain->dictionnary[index];
      ++index;
    }
  while ((index + 1) < chain->index)
    {
      tmp[index] = chain->dictionnary[index + 1];
      tmp[index]->index = index;
      ++index;
    }
  free(chain->dictionnary);
  chain->dictionnary = NULL;
  chain->dictionnary = tmp;
  return (0);
}
