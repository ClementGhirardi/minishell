/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:30:17 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/08 13:50:12 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_data(t_data *data)
{
	if (data->env)
		free_array(data->env);
	if (*data->input)
	{
		free(*data->input);
	}
	if (data->other_lines)
		free(data->other_lines);
	if (data->ast)
		ast_free(data->ast);
	data->env = NULL;
	*data->input = NULL;
	data->other_lines = NULL;
	data->ast = NULL;
}
