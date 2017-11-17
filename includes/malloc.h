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

# define MMAP(x)		mmap(0, x, PROT, MAP, -1, 0)
# define MUNMAP(x, y)	munmap(x, y)

# include <unistd.h>
# include <sys/mman.h>
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
