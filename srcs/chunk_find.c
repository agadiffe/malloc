/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_find.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadiffe <agadiffe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/13 17:28:43 by agadiffe          #+#    #+#             */
/*   Updated: 2018/04/13 17:28:45 by agadiffe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

t_header		*find_free_chunk(t_header **lst, size_t size)
{
	t_header	*tmp;

	tmp = *lst;
	while (tmp && !(tmp->is_free && tmp->size >= size))
		tmp = tmp->next;
	return (tmp);
}

t_header		*find_chunk(void *ptr, int *zone)
{
	t_header	**data[4];
	t_header	*tmp;
	int			i;

	i = 0;
	data[0] = &g_data.tiny;
	data[1] = &g_data.small;
	data[2] = &g_data.large;
	data[3] = NULL;
	while (data[i])
	{
		if (zone)
			*zone = i;
		tmp = *data[i];
		while (tmp)
		{
			if (tmp->mem == ptr)
				return (tmp);
			tmp = tmp->next;
		}
		i++;
	}
	return (NULL);
}
