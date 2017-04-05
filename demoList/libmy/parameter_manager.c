/*
** parameters_manager.c for libmy_extended in /home/aurelien/Documents/Lib_etna/castel_a/libmy_extended/libmy
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Thu Feb  9 01:01:55 2017 CASTELLARNAU Aurelien
** Last update Thu Feb 23 23:14:48 2017 CASTELLARNAU Aurelien
*/

#include <stdlib.h>
#include "libmy.h"

t_parameter	*build_parameter(char *opt, char *param, int rights[])
{
  t_parameter	*parameter;
  t_chain	*inputs;
  
  if ((parameter = malloc(sizeof (*parameter))) == NULL)
    return (NULL);
  if ((parameter->option = my_strdup(opt)) == NULL)
    return (NULL);
  if ((parameter->name = my_strdup(param)) == NULL)
    return (NULL);
  if ((inputs = create_chain()) == NULL)
    return (NULL);
  parameter->mandatory = rights[0];
  parameter->mandatory_arguments = rights[1];
  parameter->allowed_arguments = rights[2];
  parameter->inputs = inputs;
  return (parameter);
}


void		free_parameter(t_parameter *parameter)
{
  int		index_inputs;
  t_chain	*inputs;
  t_link	*tmp;

  index_inputs = 0;
  if (parameter->option != NULL)
    free(parameter->option);
  if (parameter->name != NULL)
    free(parameter->name);
  inputs = ((t_chain*)parameter->inputs);
  delete_chain(inputs);
  free(parameter);
}
