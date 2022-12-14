/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatstra <mbatstra@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 13:06:34 by mbatstra          #+#    #+#             */
/*   Updated: 2022/10/25 19:31:30 by mbatstra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include "minishell.h"
#include "libft.h"

void	db_ptlist(t_list **lst)
{
	t_list	*node;

	node = *lst;
	while (node != NULL)
	{
		printf("%s: type: %d\n", \
			((t_token *)node->content)->value, \
			((t_token *)node->content)->type);
		node = node->next;
	}
}

void	db_ptcmd(t_simplecmd **cmd_table)
{
	int	i;

	i = 0;
	while (cmd_table[i] != NULL)
	{
		t_list	*arg = *(cmd_table[i]->arg);
		t_list	*in = *(cmd_table[i]->in);
		t_list	*out = *(cmd_table[i]->out);
		printf("---\n%d\n---\n", i);
		printf("arg: ");
		while (arg != NULL)
		{
			printf("%s\t", arg->content);
			arg = arg->next;
		}
		printf("\n");
		printf("in: ");
		if (in == NULL)
			printf("default");
		else while (in != NULL)
		{
			printf("%s\t", ((t_token *)in->content)->value);
			in = in->next;
		}
		printf("\n");
		printf("out: ");
		if (out == NULL)
			printf("default");
		else while (out != NULL)
		{
			printf("%s\t", ((t_token *)out->content)->value);
			out = out->next;
		}
		printf("\n");
		i++;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_simplecmd	**cmd_table;
	t_list		*tokens;
	t_list		*env;
	char		*input;
	int			error;

	(void) argc;
	(void) argv;
	env_init(envp, &env);
	while (1)
	{
		error = 0;
		input = readline("minishell-$ ");
		if (input == NULL)	// handle this
			printf("error\n");
		add_history(input);
		tokens = NULL;
		error = lexer_tokenize(&tokens, input);
		if (!error)
		{
			cmd_table = parse_cmd_init(tokens);
			error = parse_tokens(cmd_table, &tokens, env);
			db_ptcmd(cmd_table);
			parse_clear_cmd_table(cmd_table);
		}
		ft_lstclear(&tokens, &lexer_clear_token);
		if (error == 1)
			printf("Allocation failure\n");
		free(input);
	}
	return (0);
}
