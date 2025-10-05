#include "../include/philosophers.h"

static int	ft_isdigit(const char *nptr)
{
	int	i;

	i = 0;
	while (nptr[i] >= '0' && nptr[i] <= '9')
		i++;
	if (nptr[i] != '\0')
		return (0);
	return (1);
}

int	ft_atoi(const char *nptr)
{
	int					i;
	int					sign;
	unsigned long long	ans;

	i = 0;
	sign = 1;
	ans = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == ' ')
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
		if (nptr[i++] == '-')
			sign *= -1;
	if (!(nptr[i] >= '0' && nptr[i] <= '9') || !ft_isdigit(&nptr[i]))
		return (-1);
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		ans = ans * 10 + (nptr[i++] - '0');
		if (ans > __LONG_LONG_MAX__)
			return (-1);
	}
	return (((int) ans) * sign);
}

void	ft_cleanup(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->count)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->write_lock);
	pthread_mutex_destroy(&data->death_lock);
	free(data->forks);
	free(data->philos);
}
