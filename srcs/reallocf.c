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
	void				*new;

	new = realloc(ptr, size);
	if (!new)
		free(ptr);
	return (new);
}
