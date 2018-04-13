/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib_print_str.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadiffe <agadiffe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/13 17:29:00 by agadiffe          #+#    #+#             */
/*   Updated: 2018/04/13 17:29:03 by agadiffe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

size_t			ft_strlen(const char *s)
{
	const char	*origin_str;

	origin_str = s;
	while (*s)
		s++;
	return (s - origin_str);
}

void			ft_putchar(char c)
{
	(void)write(1, &c, 1);
}

void			ft_putstr(char const *s)
{
	(void)write(1, s, ft_strlen(s));
}

void			ft_putendl(char const *s)
{
	ft_putstr(s);
	ft_putchar('\n');
}
