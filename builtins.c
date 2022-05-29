/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aainhaja <aainhaja@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 23:24:17 by fstitou           #+#    #+#             */
/*   Updated: 2022/05/29 16:18:05 by aainhaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int array_size(char **str)
{
	int i = 0;
	while (str[i])
		i++;
	// printf("%d\n", i);
	return (i);
}

void cd(t_parse *head, char ***env, char ***my_export)
{
	if (!head->argv)
		chdir(getenv("HOME"));
	else if (head->argv && head->argv[0] && !head->argv[1])
	{
		*env = crazy_add_string_to_2darray(*env, ft_strjoin("OLDPWD=",pwd(head, 0), 0), 0);
		*my_export = crazy_add_string_to_2darray(*my_export, ft_strjoin("OLDPWD=",pwd(head, 0), 0), 1);
		// c = 1;
		if (chdir(head->argv[0]) == -1)
			printf("cd: no such file or directory: %s \n", head->argv[0]);
		else
		{
			*env = crazy_add_string_to_2darray(*env, ft_strjoin("PWD=",pwd(head, 0), 0), 0);
			*my_export = crazy_add_string_to_2darray(*my_export, ft_strjoin("PWD=",pwd(head, 0), 0), 1);
			// c = 1;
		}
	}
	else
		printf("cd: too many arguments\n");
}
void printf_env(char **lenv)
{
	int i;

	i = 0;
	while (lenv[i])
	{
		printf("%s\n", lenv[i]);
		i++;
	}
}

char *pwd(t_parse *head, int k)
{
	char *buf;
	char *dir;
	size_t size;

	size = PATH_MAX;
	buf = (char *)malloc(sizeof(size_t));
	if (!head->argv)
	{
		dir = getcwd(buf, size);
		printf("%s\n", dir);
	}
	else
	{
		if (k)
		{
			printf("pwd: too many arguments\n");
		}
		dir = getcwd(buf, size);
	}
	return(dir);
}
char **add_export(t_parse *head, char ***env, char ***export)
{
	if (head->argv && ft_int_strchr(head->argv[0], '=') != -1)
	{
		*env = crazy_add_string_to_2darray(*env, head->argv[0], 0);
		*export = crazy_add_string_to_2darray(*export, head->argv[0], 1);
	}
	else if (head->argv && ft_int_strchr(head->argv[0], '=') == -1)
	{
		*export = crazy_add_string_to_2darray(*export, head->argv[0], 2);
	}
	else
		printf_env(*export);
	return (*export);
}

void	check_numb(char *str)
{
	int	i;

	i = 0;	
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
		{
			printf("exit\n");
			printf("bash: exit: %s: numeric argument required\n", str);			
			exit(0);
		}
		i++;
	}
}

void my_exit(t_parse *cmd)
{
	int j;

	j = 0;
	if (cmd->argv)
	{
		if (!strcmp(cmd->argv[0], "0"))
		{
			printf("exit\n");
			exit(0);
		}
		check_numb(cmd->argv[0]);
		if (cmd->argv[1])
		{
			printf("exit\n");
			printf("bash: exit: too many arguments\n");
		}
		else
		{
			printf("exit\n");
			exit(1);
		}
	}
	else
	{
		printf("exit\n");
		exit(0);
	}
}
void echo(t_parse *cmd)
{
	int i;

	i = 1;
	// if (!cmd->argv && !cmd->argv[0])
	// 	write(1, "\n", 1);
	if (cmd->argv)
	{
		if (strcmp(cmd->argv[0], "-n") == 0)
		{
			while (cmd->argv[i])
			{
				write(cmd->fd_out, cmd->argv[i], strlen(cmd->argv[i]));
				i++;
			}
		}
		else
		{
			i = 0;
			while (cmd->argv[i])
			{
				write(1, cmd->argv[i], strlen(cmd->argv[i]));
				i++;
			}
			write(1, "\n", 1);
		}
	}
	else
		write(1, "\n", 1);
}

void	del_2d_array(char *arg, char **my_export, char **env)
{
	int	i;
	int	j;
	

	i = 0;
	j = 0;
	while (env[i] && strncmp(arg, env[i], strlen(arg)))
		i++;
	while (env[i])
	{
		if (env[i + 1])
			env[i] = strdup(env[i + 1]);
		else
			env[i] = NULL;
		i++;
	}
	while (my_export[j] && strncmp(arg, ft_substr(my_export[j],ft_int_strchr(my_export[j],' ') + 4,ft_int_strchr(my_export[j],'=')),strlen(arg)))
		j++;	
	while (my_export[j])
	{
		if (my_export[j + 1])
			my_export[j] = strdup(my_export[j + 1]);
		else
			my_export[j] = NULL;
		j++;
	}
}
int	my_getenv(char *str, char ***my_env)
{
	int	i;

	i = 0;
	while (my_env[i])
	{
		if (strncmp(str, *my_env[i], strlen(str)))
			return (1);
		i++;
	}
	return (0);
}

void	unset(t_parse *cmd, char ***my_export, char ***env)
{
	int i ;

	i = 0;
	// printf("var===%s\n",cmd->argv[i]);
	if (!cmd->argv)
		return ;
	while (cmd->argv[i])
	{
		if (my_getenv(cmd->argv[i], env))
			del_2d_array(cmd->argv[i], *my_export, *env);
		i++;
	}
}

void builtins(t_parse *commands, char ***env, char ***my_export)
{
	t_parse *head;

	head = commands;
	if (head)
	{
		// printf("--%s--" ,head->cmd);
		if (strcmp(head->cmd, "cd") == 0)
			cd(head, env, my_export);
		else if (strcmp(head->cmd, "env") == 0)
			printf_env(*env);
		else if (strcmp(head->cmd, "export") == 0)
			*my_export = add_export(head, env, my_export);
		else if (strcmp(head->cmd, "pwd") == 0)
			pwd(head, 1);
		else if (strcmp(head->cmd, "exit") == 0)
			my_exit(head);
		else if (strcmp(head->cmd, "echo") == 0)
			echo(head);
		else if (strcmp(head->cmd, "unset") == 0)
		    unset(head, my_export, env);
	}
}