#include "../include/philosophers.h"

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 5 && argc != 6)
	{
		printf("Usage: ./philo count t_death");
		printf("t_eat t_sleep [meal_num]\n");
		return (1);
	}
	if (init_struct(&data, argv) || init_philo(&data))
	{
		if (data.forks && data.philos)
			ft_cleanup(&data);
		return (1);
	}
	if (start_simulation(&data))
	{
		ft_cleanup(&data);
		return (1);
	}
	ft_cleanup(&data);
	return (0);
}
