#include "../include/philosophers.h"

int		start_simulation(t_data *data)
{
	pthread_t	death_checkerer;
	pthread_t	meals_checker;
	int			i;

	if (create_philo_threads(data))
		return (1);
	if (pthread_create(&death_checkerer, NULL, monitor_death, data))
		return (printf("Death monitor thread failed\n"), 1);
	if (data->meal_num > 0)
	{
		if (pthread_create(&meals_checker, NULL, monitor_meals, data))
			return (printf("Meals monitor thread failed\n"), 1);
		pthread_join(meals_checker, NULL);
	}
	i = -1;
	while (++i < data->count)
	{
		if (pthread_join(data->philos[i].thread, NULL))
		{
			printf("❌ Erreur en rejoignant le thread du philosophe %d\n", i);
			return (1);
		}
	}
	pthread_join(death_checkerer, NULL);
	return (0);
}

void	*monitor_death(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (1)
	{
		pthread_mutex_lock(&data->death_lock);
		if (!data->still_running)
		{
			pthread_mutex_unlock(&data->death_lock);
			return (NULL);
		}
		pthread_mutex_unlock(&data->death_lock);
		i = 0;
		while (i < data->count)
		{
			if (death_checker(data, i))
				return (NULL);
			i++;
		}
		usleep(500);
	}
	return (NULL);
}

void *monitor_meals(void *arg)
{
    t_data  *data = (t_data *)arg;
    int     finished_meals;

    while (1)
    {
    	pthread_mutex_lock(&data->death_lock);
		if (!data->still_running)
		{
			pthread_mutex_unlock(&data->death_lock);
			break;
		}
		pthread_mutex_unlock(&data->death_lock);
        finished_meals = count_meals_eaten(data);
        if (finished_meals == data->count)
        {
            printf("All philosophers hargve eaten at least %d meals. "
                   "Ending simulation.\n", data->meal_num);
            pthread_mutex_lock(&data->death_lock);
            data->still_running = 0;
            pthread_mutex_unlock(&data->death_lock);
            return (NULL);
        }
        usleep(1000);
    }
    return (NULL);
}

void *philosopher_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;

    if (philo->id % 2 == 0)
        usleep(1000);

    while (1)
    {
        pthread_mutex_lock(&philo->data->death_lock);
        if (!philo->data->still_running)
		{
			pthread_mutex_unlock(&philo->data->death_lock);
            return (NULL);
		}
		pthread_mutex_unlock(&philo->data->death_lock);
        eat(philo);

        pthread_mutex_lock(&philo->data->death_lock);
        if (!philo->data->still_running)
		{
			pthread_mutex_unlock(&philo->data->death_lock);
            return (NULL);
		}
		pthread_mutex_unlock(&philo->data->death_lock);
        sleep_and_think(philo);
    }
}

int	death_checker(t_data *data, int i)
{
	long long	time_since_meal;

	pthread_mutex_lock(&data->death_lock);
	time_since_meal = get_time() - data->philos[i].last_meal;
	if (time_since_meal > data->t_death && data->still_running)
	{
		data->still_running = 0;
		pthread_mutex_unlock(&data->death_lock);
		pthread_mutex_lock(&data->write_lock);
		printf("%lld %d died\n",
			get_time() - data->start_time, data->philos[i].id);
		pthread_mutex_unlock(&data->write_lock);
		return (1);
	}
	pthread_mutex_unlock(&data->death_lock);
	return (0);
}
