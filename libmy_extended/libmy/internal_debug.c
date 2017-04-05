/*
** internal_debug.c for libmy_extended in /home/aurelien/Documents/Lib_etna/castel_a/libmy_extended/libmy
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Thu Feb  9 01:25:13 2017 CASTELLARNAU Aurelien
** Last update Thu Feb  9 01:26:51 2017 CASTELLARNAU Aurelien
*/

#include "libmy.h"

void		locate()
{
  static int	call = 0;
  ++call;
  my_putstr("\n\n locate ");
  my_put_nbr(call);
}
