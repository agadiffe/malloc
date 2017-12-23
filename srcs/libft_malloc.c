#include <unistd.h>
#include <stdint.h>
#include <limits.h>

static size_t	ft_strlen(const char *s)
{
	const char	*origin_str;

	origin_str = s;
	while (*s)
		s++;
	return (s - origin_str);
}

static void		ft_putchar(char c)
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

void			*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*s1;
	unsigned char	*s2;

	if (!n || (!dst && !src))
		return (dst);
	s1 = (unsigned char*)dst;
	s2 = (unsigned char*)src;
	while (n--)
		*s1++ = *s2++;
	return (dst);
}

static void		*ft_memset(void *b, int c, size_t len)
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

void			ft_bzero(void *s, size_t n)
{
	ft_memset(s, '\0', n);
}
