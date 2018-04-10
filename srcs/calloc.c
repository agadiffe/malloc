#include "malloc.h"

static void			*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*ptr;

	ptr = (unsigned char *)b;
	while (len)
	{
		*ptr++ = (unsigned char)c;
		len--;
	}
	return (b);
}

static void			ft_bzero(void *s, size_t n)
{
	ft_memset(s, '\0', n);
}

FOR_EXPORT_VOID 	*calloc(size_t nmemb, size_t size)
{
	void	*new;

	ft_putstr("CALLOC:\t");
	ft_putnbr(size);
	ft_putstr("\n");
	if (nmemb && size && (new = malloc(nmemb * size)))
	{
		ft_bzero(new, ALIGN4(nmemb * size));
		return (new);
	}
	return (NULL);
}
