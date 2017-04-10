/*
** validate_opt.c for libmy_extended in /home/aurelien/Documents/Lib_etna/castel_a/libmy_extended
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Thu Feb  2 23:56:36 2017 CASTELLARNAU Aurelien
** Last update Mon Apr 10 21:05:26 2017 CASTELLARNAU Aurelien
*/

#include <stdlib.h>
#include "libmy.h"

/*
** HERE NEED TO FULLFILL PARAMETERS T_CHAIN
*/
int		format_parameters(int pos,
				  int parameters,
				  char *argv[],
				  t_option **option)
{
  int		boundary;
  int		index_parameter;
  t_chain	params_chain;

  boundary = pos + parameters;
  index_parameter = 0;
  (*option)->mandatory_arguments = index_parameter;
  return (0);
}

int		check_arguments(int index_argv, int argc, t_option *option, char *argv[])
{
  int		pos;
  int		offset;
  int		parameters;

  pos = index_argv;
  parameters = 0;
  if (!option->mandatory_arguments)
    return (0);
  else
    {
      if ((index_argv + option->mandatory_arguments) >= argc)
	return (dis_miss_arg_err(option->name));
      else
	{
	  while (index_argv < (pos + option->mandatory_arguments))
	    {
	      ++index_argv;
	      if (argv[index_argv][0] == 45 && argv[index_argv][1] != 45)
		return (dis_arg_err(argv[pos], argv[index_argv]));
	      ++parameters;
	    }
	  offset = 0;
	  while (index_argv < (pos + option->allowed_arguments)
		 && index_argv + 1 < argc
		 && offset != argc)
	    {
	      offset = index_argv;
	      ++index_argv;
	      if (argv[index_argv][0] == 45)
		  offset = argc;
	      else
		  ++parameters;
	    }
	  if (index_argv + 1 < argc && argv[index_argv + 1][0] != 45)
	    return (dis_too_much_argv(argv[pos], argv[index_argv + 1]));
	  if (format_parameters(pos, parameters, argv, &option))
	    return (dis_memory_fail("Try to add parameter in option->parameter\n function check_argument from libParse"));
	  return (0);
	}
    }
}

/*
** Valid or not the presence of manadatories elements
*/
int		check_mandatories(int argc, char *argv[], t_chain **options)
{
  t_option	*option;
  t_link	*tmp;
  t_link	*param_tmp;
  char		*parameter;
  
  tmp = (*options)->first;
  while (tmp)
    {
      option = ((t_option*)tmp->content);
      if (option->mandatory == 1)
	{
	  return (dis_mandatory_err(option->name));
	}
      if (((t_chain*)option->parameters) != NULL)
	{
	  param_tmp = ((t_link*)((t_chain*)option->parameters)->first);
	  while (param_tmp)
	    {
	      parameter = ((char*)param_tmp->content);
	      param_tmp = param_tmp->next;
	    }
	}
      tmp = tmp->next;
    }
  return (0);
}

/*
** Check if there is arguments
** and if the input option
** is coherent with the options
** defines by the lib's user
*/
int             validate_opt(int argc,
			     char *argv[],
			     t_chain **options)
{
  int		index_argv;
  t_link	*tmp;
  t_option	*option;
  
  index_argv = 1;
  while (index_argv < argc)
    {
      if (argv[index_argv][0] == 45)
	{
	  tmp = (*options)->first;
	  while (tmp)
	    {
	      option = ((t_option*)tmp->content);
	      option->index = tmp->index;
	      if (!my_strcmp(option->name, argv[index_argv]))
		{
		  if (check_arguments(index_argv, argc, option, argv) && option->mandatory)
		    return (1);
		  if (option->mandatory == 1)
		      option->mandatory = 0;
		  option->to_execute = 1;  
		}
	      tmp = tmp->next;
	    }
	}
      ++index_argv;
    }
  return (check_mandatories(argc, argv, options));
}
