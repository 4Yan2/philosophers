#include "../include/philosophers.h"

int	create_philo_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->count)
	{
		if (pthread_create(&data->philos[i].thread, NULL,
				philosopher_routine, &data->philos[i]))
			return (printf("Thread creation failed\n"), 1);
		usleep(100);
		i++;
	}
	return (0);
}

int	count_meals_eaten(t_data *data)
{
	int	i;
	int	finished_meals;

	finished_meals = 0;
	i = 0;
	while (i < data->count)
	{
		pthread_mutex_lock(&data->death_lock);
		if (data->philos[i].num_eaten >= data->meal_num
			&& data->meal_num > 0)
			finished_meals++;
		pthread_mutex_unlock(&data->death_lock);
		i++;
	}
	return (finished_meals);
}
