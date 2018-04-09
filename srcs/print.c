#include "malloc.h"

void		print_zone(int zone)
{
	if (zone == 0)
		ft_putstr("TINY : ");
	else if (zone == 1)
		ft_putstr("SMALL : ");
	else
		ft_putstr("LARGE : ");
}

size_t		print_chunk(t_header **block, int zone)
{
	t_header	*tmp;
	size_t		zone_size;

	zone_size = 0;
	tmp = *block;
	ft_putstr("0x");
	print_zone(zone);
	while (tmp)
	{
		zone_size += tmp->size;
		ft_putstr("0x");
		ft_putnbr_base((uintmax_t)tmp->mem, BASE16);
		ft_putstr(" - ");
		ft_putstr("0x");
		ft_putnbr_base((uintmax_t)tmp->mem + tmp->size, BASE16);
		ft_putstr(" : ");
		ft_putnbr(tmp->size);
		ft_putstr(" octets ");
		if (tmp->is_free)
			ft_putendl("FREE");
		else
			ft_putendl("USED");
		tmp = tmp->next;
	}
	return (zone_size);
}

void		show_alloc_mem(void)
{
	t_header	**data[4];
	t_header	*tmp;
	int			i;
	size_t		total_size;

	i = 0;
	total_size = 0;
	data[0] = &g_data.tiny;
	data[1] = &g_data.small;
	data[2] = &g_data.large;
	data[3] = NULL;
	while (data[i])
	{
		tmp = *data[i];
		if (tmp)
			total_size += print_chunk(&tmp, i);
		i++;
	}
	ft_putstr("Total : ");
	ft_putnbr(total_size);
	ft_putendl(" octets");
}
