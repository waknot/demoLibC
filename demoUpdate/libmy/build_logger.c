
/*
** logger.c for libmy_extended in /home/aurelien/Documents/Lib_etna/castel_a/libmy_extended/libmy
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Tue Feb  7 23:01:39 2017 CASTELLARNAU Aurelien
** Last update Wed Apr  5 16:49:27 2017 CASTELLARNAU Aurelien
*/

#include <stdlib.h>
#include "libmy.h"

void	*log_warning(char *str)
{
  my_putstr("\n<WARN>\n");
  my_putstr(str);
}

void	*log_error(char *str)
{
  my_putstr("\n<ERR>\n");
  my_putstr(str);
}

void_char	*verbose(t_logger *logger, char *line, char *file, char *function)
{
  
}

t_logger_int_void_star	build_logger(int nb_params, t_chain *parameters)
{
  int			level;
  char			*verbose_lvl;
  t_link		*tmp;
  char			*parameter;

  tmp  = parameters->first;
  while (tmp)
    {
      parameter = ((char*)tmp->content);
      my_putstr("\nDeclenchement du parametre: ");
      my_putstr(parameter);
      tmp = tmp->next;
    }
}
