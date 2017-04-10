/*
** my_strcat.c for libmy_finale in /home/aurelien/repertoire_rendu/piscine_C/Jour04/castel_a
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Fri Oct 21 12:49:49 2016 CASTELLARNAU Aurelien
** Last update Wed Nov 16 10:58:22 2016 CASTELLARNAU Aurelien
*/

char	*my_strcat(char *dest, char *src)
{
  int	i;
  int	j;

  i = 0;
  j = 0;
  while (*(dest + i) != '\0')
    ++i;
  while (*(src + j) != '\0')
    {
      dest[i] = src[j];
      ++i;
      ++j;
    }
  dest[i] = '\0';
  return (dest);
}
