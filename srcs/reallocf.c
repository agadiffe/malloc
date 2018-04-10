#include "malloc.h"

FOR_EXPORT_VOID		*reallocf(void *ptr, size_t size)
{
	void	*new;

	ft_putstr("REALLOCF:\t");
	ft_putnbr_base((uintmax_t)ptr, BASE16);
	ft_putstr("\n");
	ft_putnbr_base(size, BASE16);
	ft_putstr("\n");
	new = realloc(ptr, size);
	if (!new)
		free(ptr);
	return (new);
}
