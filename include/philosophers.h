#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>
# include <string.h>

typedef struct s_data	t_data;

typedef struct s_philo
{
	int				id;
	int				num_eaten;
	long long		last_meal;
	pthread_t		thread;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	t_data			*data;
}	t_philo;

typedef struct s_data
{
	int				count;
	int				t_death;
	int				t_eat;
	int				t_sleep;
	int				meal_num;
	int				still_running;
	long long		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	death_lock;
	t_philo			*philos;
}	t_data;


long long		get_time(void);
void			ft_usleep(int time_in_ms, t_data *data);
int				ft_atoi(const char *nptr);
void			print_status(t_philo *philo, const char *status);


int				init_struct(t_data *data, char **argv);
int				init_philo(t_data *data);
void			ft_cleanup(t_data *data);


void			*philosopher_routine(void *arg);
int				start_simulation(t_data *data);


void			eat(t_philo *philo);
void			sleep_and_think(t_philo *philo);
void			*monitor_death(void *arg);
void			*monitor_meals(void *arg);

int				create_philo_threads(t_data *data);
int				death_checker(t_data *data, int i);
int				count_meals_eaten(t_data *data);

#endif