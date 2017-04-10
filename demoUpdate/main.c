/*
** main.c for demoLib in /home/aurelien/Documents/demoLib/libmy_extended
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Wed Apr  5 11:44:31 2017 CASTELLARNAU Aurelien
** Last update Mon Apr 10 23:43:53 2017 CASTELLARNAU Aurelien
*/

#include <stdlib.h>
#include "libmy.h"

void    display_help()
{
  my_putstr("\nUse:\n");
  my_putstr("\nMandatory: -hello: display 'Hello World'\n");
  my_putstr("\nMandatory: -ipconfig [address] [port]\n");
  my_putstr("\nOptional: -help: display usage\n");
}

// For an example with a int as expected parameter.
void		fake_ipconfig(t_chain *parameters)
{
  int		port;

  my_putstr("\nThe address is set to: ");
  my_putstr((char*)((t_link*)parameters->dictionnary[0])->content);
  my_putstr("\nThe port is set to: ");

  // On cast le deuxième paramètre en int grâce à getnbr
  port = my_getnbr((char*)((t_link*)parameters->dictionnary[1])->content);
  // et on a un int...
  my_put_nbr(port);
  // de là on peut paramétrer
  my_putchar('\n');
}

void		display_hello(t_chain *parameters)
{
  t_link	*tmp;

  tmp = parameters->first;
  my_putstr("\n");
  while (tmp)
    {
      my_putstr((char*)tmp->content);
      my_putchar(' ');
      tmp = tmp->next;
    }
  /*
  ** Ou bien:
  ** my_putstr((char*)parameters->dictionnary[0]->content)
  ** Parce que l'on sait que le second paramètre doit être un int par exemple:
  ** my_put_nbr((int)parameters->dictionnary[1]->content)
  ** On peut ainsi affiner les paramètres, reste que le typage n'est pas pris en compte
  ** dans le process de parsing et que donc la gestion d'erreur à l'exécution de l'option
  ** doit être léché... si un int est attendu est que la chaine de caractères reçue n'est pas
  ** convertible par my_put_nbr, c'est le drame...
  */
  my_putstr("\n Hello World\n");
}

int     exec(t_option *option)
{
  char  *opt;

  opt = option->name;
  my_putstr("\nExecution de la fonction de callback: ");
  my_putstr(opt);

  if (!my_strcmp(opt, "-h"))
    ((void (*)(void))option->action)();
  if (!my_strcmp(opt, "-hello") || !my_strcmp(opt, "-ipconfig"))
    (*(void (*)(t_chain*))option->action)(option->parameters);
  return (0);
}

/*
** how to use libParse:
*/
int		main(int argc, char *argv[])
{
  /*
  ** LibParse fonctionne en corrélation avec libList
  ** Les fonctions validate_opt et check_mandatories attendent une t_chain en parametre
  ** voir libmy.h
  */
  // Etape 1, déclarer les options de type t_option
  // attendues par le programme au runtime:
  // Chainer ces options dans une t_chain
  t_chain	*chaine_options;
  t_option	*opt_hello;
  t_option	*opt_conf;
  t_option	*opt_help;
  t_link	*ltmp;
  t_option	*otmp;
  // 2 fonctions de creation de liste différentes: create_chain() et create_chainf(void*)
  // avec create_chainf() on passe une fonction chargée de free un contenu particulier
  // dans le liste->link->content. Par exemple, si le content est une structure, comme ici
  // les 2 t_option*s
  if ((chaine_options = create_chainf(free_options_in_chain)) == NULL)
    return (1);
  // La fonction new_option(int, mandatory : l'option est-elle obligatoire? BOOL
  //                        int, mandatory_arguments : combien d'arguments obligatoires attend l'option? INT
  //                        int, allowed_arguments : combien d'arguments sont acceptés par l'option? INT
  //                        char*, opt: nom de l'option attendue "-help" ou "-port" par exemple
  //                        void*, action: la fonction a exécuter si l'option est présente.
  //                           )
  if ((opt_hello = new_option(1, 0, 4, "-hello", display_hello)) == NULL)
    return (1);
 if ((opt_conf = new_option(1, 2, 0, "-ipconfig", fake_ipconfig)) == NULL)
    return (1);
  if ((opt_help = new_option(0, 0, 0, "-help", display_help)) == NULL)
    return (1);
  // On active les chaines de parametres pour les options qui doivent en avoir
  if ((opt_hello->parameters = create_chain()) == NULL)
    return (1);
  if ((opt_conf->parameters = create_chain()) == NULL)
    return (1);
  // on link les options
  if (add_link(&chaine_options, opt_hello))
    return (1);
  if (add_link(&chaine_options, opt_conf))
    return (1);
  if (add_link(&chaine_options, opt_help))
    return (1);

  // Une fois les options définies (typiquement, dans une fonction get_options)
  // on passe argv et argc à une fonction parse() et c'est finis...

  if (parse(argc, argv, &chaine_options))
    display_help();
  else
    {
      ltmp = chaine_options->first;
      while (ltmp)
	{
	  otmp = ((t_option*)ltmp->content);
	  // Exec() est à définir par l'utilisateur
	  if (otmp->to_execute)
	    exec(otmp);
	  ltmp = ltmp->next;
	}
    }

  // pas zapper...
  delete_chainf(&chaine_options);
  // revient à faire:
  // free_options_in_chain(chaine_options);
  // delete_chain(chaine_options);
  return (0);
}
