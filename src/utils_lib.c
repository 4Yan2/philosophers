#include "../include/philosophers.h"

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_usleep(int time_in_ms, t_data *data)
{
	long long	start_time;

	start_time = get_time();
	while ((get_time() - start_time) < time_in_ms)
	{
		usleep(100);
		pthread_mutex_lock(&data->death_lock);
		if (!data->still_running)
		{
			pthread_mutex_unlock(&data->death_lock);
			break ;
		}
		pthread_mutex_unlock(&data->death_lock);
	}
}

void	safe_print(t_philo *philo, const char *msg)
{
	long long	timestamp;

	pthread_mutex_lock(&philo->data->write_lock);
	timestamp = get_time() - philo->data->start_time;
	printf("%lld %d %s\n", timestamp, philo->id, msg);
	pthread_mutex_unlock(&philo->data->write_lock);
}

void	print_status(t_philo *philo, const char *status)
{
	pthread_mutex_lock(&philo->data->death_lock);
	if (philo->data->still_running)
		safe_print(philo, status);
	pthread_mutex_unlock(&philo->data->death_lock);
}
