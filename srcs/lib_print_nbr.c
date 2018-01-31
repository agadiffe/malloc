#include "malloc.h"
#include <unistd.h>
#include <stdint.h>
#include <limits.h>

static void		ft_putnbr_base_inner(uintmax_t n, size_t len_base, char *base)
{
	if (n >= len_base)
		ft_putnbr_base_inner(n / len_base, len_base, base);
	ft_putchar(base[n % len_base]);
}

void			ft_putnbr_base(uintmax_t n, char *base)
{
	size_t		len_base;

	len_base = ft_strlen(base);
	if (len_base > 1)
		ft_putnbr_base_inner(n, len_base, base);
}

static void		ft_putnbr_inner(int n)
{
	if (n > 9)
		ft_putnbr_inner(n / 10);
	ft_putchar('0' + (n % 10));
}

void			ft_putnbr(int n)
{
	if (n == INT_MIN)
	{
		ft_putstr("-2147483648");
		return ;
	}
	if (n < 0)
	{
		ft_putchar('-');
		n = -n;
	}
	ft_putnbr_inner(n);
}
