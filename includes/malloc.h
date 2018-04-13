/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadiffe <agadiffe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/13 17:27:06 by agadiffe          #+#    #+#             */
/*   Updated: 2018/04/13 17:27:46 by agadiffe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# define TINY			(size_t)getpagesize() * 2
# define SMALL			(size_t)getpagesize() * 16
# define TINY_ZONE		(size_t)TINY * 102
# define SMALL_ZONE		(size_t)SMALL * 102

# define HEADER_SIZE	(size_t)(sizeof(t_header))

# define ALIGN4(x)	(((((x) - 1) >> 2) << 2) + 4)

# define PROT		PROT_READ | PROT_WRITE
# define MAP		MAP_ANON | MAP_PRIVATE

# define BASE16		"0123456789ABCDEF"

# define FOR_EXPORT_VOID	__attribute__ ((visibility("default"))) void

# include <unistd.h>
# include <pthread.h>
# include <stdint.h>

typedef struct		s_header
{
	size_t			size;
	struct s_header	*prev;
	struct s_header	*next;
	int				is_free;
	void			*mem;
}					t_header;

typedef struct		s_data
{
	struct s_header	*tiny;
	struct s_header	*small;
	struct s_header	*large;
}					t_data;

t_data				g_data;
pthread_mutex_t		g_mutex;

/*
**	malloc.c
*/
void				*malloc(size_t size);

/*
**	free.c
*/
void				free(void *ptr);

/*
**	realloc.c
*/
void				*realloc(void *ptr, size_t size);

/*
**	lib_print_string.c
*/
size_t				ft_strlen(const char *s);
void				ft_putchar(char c);
void				ft_putstr(char const *s);
void				ft_putendl(char const *s);

/*
**	lib_print_nbr.c
*/
void				ft_putnbr_base(uintmax_t n, char *base);
void				ft_putnbr(int n);

/*
**	chunk_create.c
*/
void				join_next_chunk(t_header *block);
void				split_chunk(t_header *block, size_t size);
t_header			*create_chunk(size_t size);
void				insert_chunk_ascending(t_header **data, t_header *ptr);

/*
**	chunk_find.c
*/
t_header			*find_free_chunk(t_header **lst, size_t size);
t_header			*find_chunk(void *ptr, int *zone);

/*
**	print.c
*/
void				show_alloc_mem(void);

#endif
