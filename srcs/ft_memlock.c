/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadiffe <agadiffe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/13 17:29:05 by agadiffe          #+#    #+#             */
/*   Updated: 2018/04/13 17:29:12 by agadiffe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>

pthread_mutex_t		*ft_memlock(void)
{
	static pthread_mutex_t		lock = PTHREAD_MUTEX_INITIALIZER;

	return (&lock);
}
