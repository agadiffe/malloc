/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reallocf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadiffe <agadiffe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/13 17:29:27 by agadiffe          #+#    #+#             */
/*   Updated: 2018/04/13 17:29:31 by agadiffe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

FOR_EXPORT_VOID		*reallocf(void *ptr, size_t size)
{
	void	*new;

	pthread_mutex_init(&g_mutex, NULL);
	pthread_mutex_lock(&g_mutex);
	new = realloc(ptr, size);
	if (!new)
		free(ptr);
	pthread_mutex_unlock(&g_mutex);
	pthread_mutex_destroy(&g_mutex);
	return (new);
}
