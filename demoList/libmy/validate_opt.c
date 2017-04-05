/*
** validate_opt.c for libmy_extended in /home/aurelien/Documents/Lib_etna/castel_a/libmy_extended
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Thu Feb  2 23:56:36 2017 CASTELLARNAU Aurelien
** Last update Fri Feb 24 13:52:59 2017 CASTELLARNAU Aurelien
*/

#include <stdlib.h>
#include "libmy.h"

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

int	format_parameter_option(int index_argv, int argvs, char *argv[], t_parameter **parameter)
{
  int	available_inputs;
  int	iterator;

  my_putstr("\nBEGIN OFF FORMAT PARAMETER OPTION\n");
  iterator = 0;
  my_putstr("\nNumber of parameters for this option: ");
  my_put_nbr(argvs);
  my_putstr("\nFrom index: ");
  my_put_nbr(index_argv);
  my_putstr("\nOption: ");
  my_putstr(argv[index_argv]);
  while (iterator < argvs)
    {
      my_putstr("\nInput: ");
      my_putstr(argv[index_argv]);
      ++iterator;
    }
  my_putstr("\nEND OF FORMAT PARAMETER OPTION\n");
  return (0);
}

int		check_parameter_option(int index_argv, int argc, char *argv[], t_option *option)
{
  int		not_conform;
  int		available_inputs;
  int		offset;
  int		argvs;
  int		pos;
  t_link	*tmp;
  t_parameter	*parameter;

  my_putstr("\nBEGIN OF CEHCK PARAMETER OPTION\n");
  not_conform = 1;
  pos = index_argv;
  ++index_argv;
  available_inputs = argc - 1 - index_argv;
  tmp = ((t_chain*)option->parameters)->first;
  while (tmp)
    {
      parameter = ((t_parameter*)tmp->content);
      if (!my_strcmp(parameter->option, option->option)
	  && !my_strcmp(parameter->name, argv[index_argv]))
	{
	  if (parameter->mandatory_arguments > available_inputs)
	    return (dis_mandatory_param_err(parameter->option, parameter->name));
	  else
	    {
	      while (index_argv < (index_argv + parameter->mandatory_arguments))
		{
		  if (argv[index_argv][0] == 45)
		    return (dis_arg_err(parameter->name, argv[index_argv]));
		  ++index_argv;
		}
	      offset = index_argv;
	      while (offset < (index_argv + parameter->allowed_arguments)
		     && index_argv < argc)
		{
		  if (argv[index_argv][0] == 45)
		    offset = index_argv + parameter->allowed_arguments;
		  ++index_argv;
		}
	    }
	  argvs = index_argv - 1 - pos;
	  my_putstr("\n\nWORK IN PROGRESS HERE!\n");
	  parameter->mandatory = format_parameter_option(pos, argvs, argv, &parameter);
	}
      tmp = tmp->next;
    }
  my_putstr("\nEND OF CHECK PARAMETER OPTION\n");
  return (0);
}

int		check_arguments(int index_argv, int argc, t_option *option, char *argv[])
{
  int		pos;
  int		offset;
  int		parameters;

  pos = index_argv;
  parameters = 0;
  my_putstr("\nInto check_arguments()\n");
  if (!option->mandatory_arguments)
    return (0);
  else
    {
      if ((index_argv + option->mandatory_arguments) >= argc)
	return (dis_miss_arg_err(option->option));
      else
	{
	  while (index_argv < (pos + option->mandatory_arguments))
	    {
	      ++index_argv;
	      if (argv[index_argv][0] == 45 && argv[index_argv][1] != 45)
		return (dis_arg_err(argv[pos], argv[index_argv]));
	      if (argv[index_argv][0] == 45 && argv[index_argv][1] == 45)
		{
		  if (check_parameter_option(index_argv, argc, argv, option))
		    return (1);
		}
	      ++parameters;
	    }
	  offset = 0;
	  while (index_argv < (pos + option->allowed_arguments)
		 && index_argv + 1 < argc
		 && offset != argc)
	    {
	      offset = index_argv;
	      ++index_argv;
	      locate();
	      my_putstr("\nValeur de l'index: \n");
	      my_put_nbr(index_argv);
	      my_putstr("\nValeur de l'argc: \n");
	      my_put_nbr(argc);
	      if (argv[index_argv][0] == 45)
		{
		  offset = argc;
		  locate();
		}
	      else
		{
		  ++parameters;
		  locate();
		}
	      locate();
	    }
	  locate();
	  if (index_argv + 1 < argc && argv[index_argv + 1][0] != 45)
	    return (dis_too_much_argv(argv[pos], argv[index_argv + 1]));
	  my_putstr("\nindex value: ");
	  my_put_nbr(index_argv);
	  my_putstr("\npos value: ");
	  my_put_nbr(pos);
	  my_putstr("\nparameters value: ");
	  my_put_nbr(parameters);
	  my_putstr("\nBefore format process\n");
	  if (format_parameters(pos, parameters, argv, &option))
	    return (dis_memory_fail("Try to add parameter in option->parameter\n function check_argument from libParse"));
	  else
	    my_putstr("\nARTIFICIAL ELSE OF CHECK ARGUMENTS SUCCESS!\n");
	  return (0);
	}
    }
}

/*
** Valid or not the presence of manadatories elements
*/
int		check_mandatories(int argc, char *argv[], t_chain **options)
{
  t_link	*tmp;
  t_option	*option;
  t_link	*param_tmp;
  t_parameter	*parameter;
  
  tmp = (*options)->first;
  while (tmp)
    {
      option = ((t_option*)tmp->content);
      if (option->mandatory == 1)
	{
	  my_putstr("\nCheck if stay mandatory for option: ");
	  my_putstr(option->option);
	  return (dis_mandatory_err(option->option));
	}
      if (((t_chain*)option->parameters) != NULL)
	{
	  my_putstr("\nGo inside if in check mandatories for option: ");
	  my_putstr(option->option);
	  param_tmp = ((t_link*)((t_chain*)option->parameters)->first);
	  while (param_tmp)
	    {
	      my_putstr("\nInto the while with: ");
	      parameter = ((t_parameter*)param_tmp->content);
	      my_putstr(parameter->name);
	      if (parameter->mandatory)
		return (dis_mandatory_param_err(option->option, parameter->name));
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
	      if (!my_strcmp(option->option, argv[index_argv]))
		{
		  if (check_arguments(index_argv, argc, option, argv) && option->mandatory)
		    return (1);
		  if (option->mandatory == 1)
		    {
		      option->mandatory = 0;
		    }
		  option->to_execute = 1;  
		}
	      tmp = tmp->next;
	    }
	}
      ++index_argv;
    }
  return (check_mandatories(argc, argv, options));
}
