#include "../include/philosophers.h"

void	assign_forks(t_philo *philo, pthread_mutex_t **first,
	pthread_mutex_t **second)
{
	if (philo->id % 2 == 0)
	{
		*first = philo->r_fork;
		*second = philo->l_fork;
	}
	else
	{
		*first = philo->l_fork;
		*second = philo->r_fork;
	}
}

static void	one_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->l_fork);
	print_status(philo, "has taken a fork");
	ft_usleep(philo->data->t_death, philo->data);
	print_status(philo, "died");
	pthread_mutex_lock(&philo->data->death_lock);
	philo->data->still_running = 0;
	pthread_mutex_unlock(&philo->data->death_lock);
	pthread_mutex_unlock(philo->l_fork);
}

void	eat(t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	if (philo->data->count == 1)
	{
		one_philo(philo);
		return ;
	}
	assign_forks(philo, &first_fork, &second_fork);
	pthread_mutex_lock(first_fork);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(second_fork);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(&philo->data->death_lock);
	philo->last_meal = get_time();
	philo->num_eaten++;
	pthread_mutex_unlock(&philo->data->death_lock);
	print_status(philo, "is eating");
	ft_usleep(philo->data->t_eat, philo->data);
	pthread_mutex_unlock(second_fork);
	pthread_mutex_unlock(first_fork);
}

void	sleep_and_think(t_philo *philo)
{
	print_status(philo, "is sleeping");
	ft_usleep(philo->data->t_sleep, philo->data);
	pthread_mutex_lock(&philo->data->death_lock);
	if (!philo->data->still_running)
	{
		pthread_mutex_unlock(&philo->data->death_lock);
		return ;
	}
	pthread_mutex_unlock(&philo->data->death_lock);
	print_status(philo, "is thinking");
}
