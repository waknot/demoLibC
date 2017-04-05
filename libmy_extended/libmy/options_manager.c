/*
** set_opt.c for libmy_extended in /home/aurelien/Documents/Lib_etna/castel_a/libmy_extended/libs/libParse
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Wed Feb  1 16:32:30 2017 CASTELLARNAU Aurelien
** Last update Thu Feb 23 23:16:06 2017 CASTELLARNAU Aurelien
*/

#include <stdlib.h>
#include "libmy.h"

/*
** Allow user to add an option to the program
*/
t_option	*new_option(int mandatory,
			    int mandatory_arguments,
			    int allowed_arguments,
			    char *opt,
			    void *action)
{
  t_option	*option;

  if ((option = malloc(sizeof (*option))) == NULL)
    return (NULL);
  option->mandatory = mandatory;
  option->mandatory_arguments = mandatory_arguments;
  option->allowed_arguments = allowed_arguments;
  option->to_execute = 0;
  option->option = my_strdup(opt);
  option->action = action;
  option->parameters = NULL;
  return (option);
}

/*
** Allow user to free options contains in a t_chain struct 
*/
void		free_options_in_chain(t_chain **chain)
{
  t_link	*tmp;
  t_option	*option;
  int		index_parameter;
  t_chain	*params;
  t_link	*link_param;
  t_parameter	*param;
  t_link	*input;

  tmp = (*chain)->first;
  index_parameter = 0;
  while (tmp)
    {
      option = ((t_option*)tmp->content);
      my_putstr("\nOption in free process: ");
      my_putstr(option->option);
      if (tmp->content != NULL)
	{
	  if (option->option != NULL)
	    free(option->option);
	  if (option->parameters != NULL)
	    {
	      params = ((t_chain*)option->parameters);
	      link_param  = ((t_link*)params->first);
	      while (link_param)
		{
		  param = ((t_parameter*)link_param->content);
		  free_parameter(param);
		  link_param = link_param->next;
		}
	      delete_chain(option->parameters);
	    }
	  free(option);
	}
      tmp = tmp->next;
    }
  my_putstr("\n\nFIN FREE OPTION IN CHAIN\n\n");
}
