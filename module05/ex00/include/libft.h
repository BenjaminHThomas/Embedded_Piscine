#ifndef LIBFT_H
# define LIBFT_H

# include <avr/io.h>
# include <util/delay.h>
# include <avr/interrupt.h>

# define HEX "0123456789ABCDEF"

void	ft_itoa(int n, char *buff);
void	ft_putnbr_array(char *str, int nb, int *index);
void	ft_bzero(char * buff, uint8_t len);

#endif