#ifndef MALLOC_H
# define MALLOC_H

# define TINY	(size_t)996
# define SMALL	(size_t)4096

# define HEADER_SIZE	(size_t)(sizeof(t_header))

# define ALIGN4(x)	(((((x) - 1) >> 2) << 2) + 4)

# include <stddef.h>
# include <pthread.h>

typedef struct		s_header
{
	void			*mem;
	size_t			size;
	int				is_free;
	struct s_header	*prev;
	struct s_header	*next;
}					t_header;

typedef struct		s_data
{
	struct s_header	*tiny;
	struct s_header	*small;
	struct s_header	*large;
}					t_data;

t_data				g_data;
pthread_mutex_t		g_mutex;

#endif
