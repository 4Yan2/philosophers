#include "../include/philosophers.h"

static int	parse_arguments(t_data *data, char **argv)
{
	data->meal_num = -1;
	data->count = ft_atoi(argv[1]);
	data->t_death = ft_atoi(argv[2]);
	data->t_eat = ft_atoi(argv[3]);
	data->t_sleep = ft_atoi(argv[4]);
	if (argv[5])
	{
		data->meal_num = ft_atoi(argv[5]);
		if (data->meal_num <= 0)
			return (printf("Invalid arguments\n"), 1);
	}
	if (data->count <= 0 || data->t_death <= 0
		|| data->t_eat <= 0 || data->t_sleep <= 0)
		return (printf("Invalid arguments\n"), 1);
	return (0);
}

int	init_struct(t_data *data, char **argv)
{
	int	i;

	memset(data, 0, sizeof(t_data));
	if (parse_arguments(data, argv))
		return (1);
	data->still_running = 1;
	data->start_time = get_time();
	data->forks = malloc(sizeof(pthread_mutex_t) * data->count);
	data->philos = malloc(sizeof(t_philo) * data->count);
	if (!data->forks || !data->philos)
		return (printf("Memory allocation failed\n"), 1);
	i = -1;
	while (++i < data->count)
		pthread_mutex_init(&data->forks[i], NULL);
	pthread_mutex_init(&data->write_lock, NULL);
	pthread_mutex_init(&data->death_lock, NULL);
	return (0);
}

int	init_philo(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->count)
	{
		memset(&data->philos[i], 0, sizeof(t_philo));
		data->philos[i].id = i + 1;
		data->philos[i].num_eaten = 0;
		data->philos[i].last_meal = data->start_time;
		data->philos[i].l_fork = &data->forks[i];
		data->philos[i].r_fork = &data->forks[(i + 1) % data->count];
		data->philos[i].data = data;
		i++;
	}
	return (0);
}
