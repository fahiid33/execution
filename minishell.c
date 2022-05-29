/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahd <fahd@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 18:45:32 by fahd              #+#    #+#             */
/*   Updated: 2022/05/29 02:23:36 by fahd             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


char  **crazy_add_string_to_2darray(char **env, char *to_add,int a) //var=15  var=12 || var=
{
	char   **str;
	char  *tmp;
	int   i = 0;
	int	j = 0;
	int   flag = 1;
	int size = array_size(env);
	tmp = ft_substr(to_add, 0, ft_int_strchr(to_add, '='));
	str = (char **)malloc(sizeof (char *) * (size + 1));
	size = strlen(tmp);
	while (env[i])
	{
		if(a == 1 
				&& strncmp(tmp, ft_substr(env[i],ft_int_strchr(env[i],' ') + 4,ft_int_strchr(env[i],'=')), size - 1) == 0)
		{
			str[j] = ft_strdup("declare -x ");
			str[j] = ft_strjoin(str[j],ft_substr(to_add,0,'='));
			str[j] = ft_strjoin(str[j],"\"");
			str[j] = ft_strjoin(str[j],strchr(to_add,'=') + 1);
			str[j] = ft_strjoin(str[j],"\"");
			flag = 0;
		}
		else if(a == 0 
				&& strncmp(tmp, ft_substr(env[i],0,ft_int_strchr(env[i],'=')), size) == 0)
		{
			str[j] = ft_strdup(to_add);
			// printf("%s    \n\n\n",str[j]);
			flag = 0;
		}
		else if(a == 2 
				&& strncmp(to_add, ft_substr(env[i],ft_int_strchr(env[i],' ') + 4,ft_int_strchr(env[i],'=')), strlen(to_add)) == 0)
			return(env);
		else if (strncmp(env[i], tmp, size))
			str[j] = strdup(env[i]);
		else
		{
			str[j] = strdup(to_add);
			flag = 0;
		}
		i++;
		j++;
	}
	if (flag)
	{
		if(a == 2)
		{
			str[j] = ft_strdup("declare -x ");
			str[j] = ft_strjoin(str[j], to_add);
		}
		else if (a == 1)
		{
			str[j] = ft_strdup("declare -x ");
			str[j] = ft_strjoin(str[j],ft_substr(to_add,0,'='));
			str[j] = ft_strjoin(str[j],"\"");
			str[j] = ft_strjoin(str[j],strchr(to_add,'=') + 1);
			str[j] = ft_strjoin(str[j],"\"");
		}
		else
			str[j] = strdup(to_add);
	}
	// printf("str ==-%s\n",str[j - 1]);
	// printf("to_add ===%s\n", to_add);
	str[++j] = NULL;
	return (str);
}

char **get_string(char **env)
{
	char **str;
	int i;
	i = 0;
   while(env[i++])
	str = malloc(sizeof(char *) * i + 1);
   i = 0;
	while(env[i])
	{
		str[i] = ft_strdup(ft_substr(env[i] , 0, '='));
		i++;
	}
	str[i++] = NULL;
	return(str);
}

char** init_export(char **env)
{
	char *s1;
	char **strings;
	int		i;
	int		j;
	char **res;
	j = 0;
	s1 = ft_strdup("");
	strings = get_string(env);
	i = 0;
   
   while(strings[i])
   {
      
      while(env[j] && strncmp(env[j],strings[i], strlen(strings[i])))
      {
         j++;
      }
      s1 = ft_strjoin(s1,"declare -x ");
      s1 = ft_strjoin(s1,strings[i]);
      
      if(strcmp(strings[i] , "OLDPWD"))
      {
         s1 = ft_strjoin(s1,"\"");
         s1 = ft_strjoin(s1,strchr(env[j],'=')+ 1);
         s1 = ft_strjoin(s1,"\"\n");
      }
      else
         s1 = ft_strjoin(s1,"\n");
      j = 0;
      i++;

   }
   
   res = ft_split(s1,'\n');
   return res;
}


void print_list(t_token *lst)
{
   while (lst)
   {
      printf("value == %s --------  type: %d --------  flag: %d \n", lst->val, lst->type, lst->flag);
      lst = lst->next;
   }
}
void print_l(t_parse *lst)
{
   int i;
   while (lst)
   {
      i  =0;
      if (lst->cmd)
      {
         printf("cmd == %s --------  args: ", lst->cmd);
         while(lst->argv && lst->argv[i])
         {
            printf("<<<%s>>",lst->argv[i]);
            i++;
         }
         if (lst->next->next)
            printf("  | \n");
         else
            printf("\n");
      }
      lst = lst->next;
   }
}
char  **my_envir(char **env)
{
   char   **str;
   int   i = 0;
   int	j = 0;
   int size = array_size(env);
   str = (char **)malloc(sizeof (char *) * (size + 1));
	while (env[i])
	{
		str[j] = strdup(env[i]);
		i++; 
		j++; 
	}
	str[j] = NULL;
	return (str); 
}

int main(int ac, char *av[], char **env)
{
   char	*line;
   char *dir;
   char *buf;
   int size;
   char	*l;
   int	i = 0;
   char **my_env;
   char **my_export;
   t_parse *commands;
   t_lexer	*test;
   t_token *test1;
   my_env = my_envir(env);
   my_export = init_export(env);
   // printf_env(my_env);
   while((line = readline("Minishell ")))
   {
      commands = init_command();
      test = malloc(sizeof(t_lexer));
      test = ft_init_lexer(line, line[0]);//initilize the lexer
      test1 = send_lexer_to_tokenize(test);//tokenizing every char in the line
      add_history(line);
      create_commands(test1, &commands);
      // print_l(commands);
      // printf("||%s|||\n", commands->argv[]);
      builtins(commands, &my_env, &my_export);
      // system("leaks minishell");
   }
}