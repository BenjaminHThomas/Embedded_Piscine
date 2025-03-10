#include "libft.h"

void	ft_putnbr_array(char *str, int nb, int *index)
{
	long int	number;

	number = nb;
	if (number < 0)
	{
		str[0] = '-';
		number *= -1;
		*index = *index + 1;
	}
	if (number > 9)
	{
		ft_putnbr_array(str, number / 10, index);
		str[*index] = number % 10 + '0';
		*index = *index + 1;
		return ;
	}
	str[*index] = number + '0';
	*index = *index + 1;
	return ;
}

void ft_itoa(int n, char *buff)
{
	int		digits;
	int		digits_count;
	int		i;

	digits = n;
	digits_count = 0;
	i = 0;
	if (n == 0)
		digits_count = 1;
	while (digits != 0)
	{
		digits /= 10;
		digits_count++;
	}
	if (n < 0)
		digits_count++;
	ft_putnbr_array(buff, n, &i);
	buff[i] = '\0';
}

void ft_bzero(char * buff, uint8_t len) {
	for (uint8_t i = 0; i < len; i++) {
		buff[i] = 0;
	}
}
