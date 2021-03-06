#include "../includes/corewar.h"
#include <fcntl.h>
#include <stdio.h>

t_obj       *initialize_obj(void)
{
    t_obj   *c;
    int i;

    i = 0;
    c = ft_memalloc(sizeof(t_obj));
    c->arena = ft_memalloc(sizeof(unsigned char *) * MEM_SIZE);
    while (i < MEM_SIZE)
    {
        c->arena[i] = 0;
        i++;
    }
    return (c);
}

void        ft_puthex(unsigned char c)
{
    char *tab;

    tab = "0123456789abcdef";
    ft_putchar(tab[c / 16]);
    ft_putchar(tab[c % 16]);
}

void		ft_append(t_players** head, unsigned char *new_data)
{
    t_players	*new_node;
    t_players	*last;
    int         i;
    unsigned int         j;

    i = 4;
    j = 0;
	new_node = ft_memalloc(sizeof(t_players));
	last = *head;
    while (j < 129)
    {
        new_node->info.prog_name[j] = new_data[i];
        i++;
        j++;
    }
    i += 6;
    new_node->info.prog_size = new_data[i];
    i++;
    j = 0;
    while (j < 2049)
    {
        new_node->info.comment[j] = new_data[i];
        i++;
        j++;
    }
    i += 3;
    j = 0;
    new_node->code = &new_data[i];
    new_node->next = NULL;
    if (*head == NULL)
    {
       *head = new_node;
       return ;
    }
    while (last->next != NULL)
        last = last->next;
    last->next = new_node;
    return ;
}

// void    load_players(t_players *pl, t_obj *c)
// {
    
// }

void    ft_puthex2(int c)
{
    char *tab;

    tab = "0123456789abcdef";
    ft_putchar(tab[((c / 16 / 16) / 16)]);
    ft_putchar(tab[((c / 16) / 16)]);
    ft_putchar(tab[(c / 16) % 16]);
    ft_putchar(tab[c % 16]);
}

void    *ft_write_to(void *arena, void *code, size_t start, size_t size)
{
    unsigned char *d;
	unsigned char *s;
    size_t          i;

	d = (unsigned char *)arena;
	s = (unsigned char *)code;
    i = 0;
	while (i < size)
	{
		d[start + i] = s[i];
		i++;
	}
	return (arena);
}

void    ft_print_dump(t_obj *c)
{
    int x;
    int y;
    int i;

    x = 1;
    y = 0;
    i = 0;
    ft_putstr("0x");
    ft_puthex2(i);
    ft_putstr(" : ");
    while (i < MEM_SIZE)
    {
        ft_puthex(c->arena[i]);
        if (i < MEM_SIZE - 1)
            ft_putchar(' ');
        if (x == 64)
        {
            if (i < MEM_SIZE - 1)
            {
                ft_putchar('\n');
                ft_putstr("0x");
                // ft_putnbr(i);
                ft_puthex2(i + 1);
                ft_putstr(" : ");
            }
            x = 0;
        }
        x++;
        i++;
    }
}

void    ft_load_champions(t_obj *c)
{
    int i;
    int offset;
    t_players *pl;

    i = 0;
    pl = c->worriors;
    offset = MEM_SIZE / c->no_of_worriors;
    while (i < c->no_of_worriors)
    {
        pl->pc = offset * i;
        pl->cool_time = 0;
        ft_write_to(c->arena, pl->code, offset * i, pl->info.prog_size);
        pl = pl->next;
        i++;
    }
}

void    ft_read_champions(t_obj *c, int ac, char **av)
{
    int i;
    unsigned char *line;
    int fd;
    int j;

    i = 0;
    fd = 0;
    c->no_of_worriors = 0;
    line = NULL;
    while (i < ac)
    {
        fd = open(av[i], O_RDONLY);
        j = ft_read_memory(fd, &line);
        ft_append(&c->worriors, line);
        c->no_of_worriors++;
        i++;
    }
}

int     main(int ac, char **av)
{
    t_obj *c;

    if (ac < 2)
    {
        ft_putendl("Usage: ./corewar [-dump numbers_of_cycles] [ [-a load_address] champion1.cor] ...");
        return (0);
    }
    c = initialize_obj();
    if (ft_strequ(av[1], "-dump"))
    {
        if (!ft_isnumber(av[2]))
        {
            ft_putendl("Usage: ./corewar [-dump numbers_of_cycles] [ [-a load_address] champion1.cor] ...");
            return (0);
        }
        ft_read_champions(c, ac - 3, &av[3]);
        ft_load_champions(c);
        ft_print_dump(c);
    }
    else
    {
        ft_read_champions(c, ac - 1, &av[1]);
        ft_load_champions(c);
    }
    // ft_putendl(pl->info->prog_name);
    return (1);
}