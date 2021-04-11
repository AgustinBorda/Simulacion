/* External definitions for single-server queueing system. */

#include <stdio.h>
#include <math.h>
#include "AleatoryNumber.h"
#include "Queue.h"

#define Q_LIMIT 100  /* Limit on queue length. */
#define BUSY      1  /* Mnemonics for server's being busy */
#define IDLE      0  /* and idle. */

/*Variable declaration*/
FILE *config;
float sim_duration;
float sim_time;
int server_status;
int num_in_q;
float time_last_event;
int next_event_type;
float time_next_event[2];
Queue waiting_queue;
int served_clients;
float queue_total_time;
float laundry_total_time;
float server_usage;
float mean_clients_laundry;
float mean_clients_queue;
int clients_in_laundry;

void  initialize(void);
void  timing(void);
void  arrive(void);
void  depart(void);
void  report(void);
void  update_time_avg_stats(void);
float expon(float mean);
float gen_next_interarrive(void);
float  gen_next_depart(void);


int main()  /* Main function. */
{

    /* Initialize the simulation. */

    initialize();

    /* Run the simulation while more delays are still needed. */

    while (sim_time <= sim_duration || num_in_q > 0 || server_status == BUSY) {

        /* Determine the next event. */
        timing();
        /* Update time-average statistical accumulators. */

        update_time_avg_stats();

        /* Invoke the appropriate event function. */

        switch (next_event_type) {
            case 1:
                arrive();
                break;
            case 2:
                depart();
                break;
        }
    }

    /* Invoke the report generator and end the simulation. */

    report();


    return 0;
}


void initialize(void)  /* Initialization function. */
{
    /*Initialize the random number generator*/
    set_seed_random_number();


    /* Initialize the simulation clock. */
    config = fopen("config.txt","r");
    fscanf(config,"%f",&sim_duration);
    sim_time = 0.0;

    /* Initialize the state variables. */
    initialization(waiting_queue);
    server_status   = IDLE;
    num_in_q        = 0;
    time_last_event = 0;


    // Initialize the statistical counters. num_custs_delayed, total_of_delays, area_num_in_q, area_server_status

     served_clients = 0;
     laundry_total_time = 0;
     server_usage = 0;
     queue_total_time = 0;
     mean_clients_laundry = 0;
     mean_clients_queue = 0;
     clients_in_laundry = 0;

    /* Initialize event list.  Since no customers are present, the departure
       (service completion) event is eliminated from consideration. */

    time_next_event[0] = sim_time + gen_next_interarrive() ; //Planificacion del primer arribo
    time_next_event[1] = 1.0e+30; //infinito

}


void timing(void)  /* Timing function. */
{
    int   i;
    float min_time_next_event = 1.0e+29;

    next_event_type = 0; //Al final debe quedar en 1 en caso de que el proximo evento sea un arribo, 2: si es la salida de un servicio

    /* Determine the event type of the next event to occur. */
    next_event_type = 1;
    min_time_next_event = time_next_event[0];
    if(min_time_next_event > time_next_event[1]) {
      min_time_next_event = time_next_event[1];
      next_event_type = 2;
    }

    /* advance the simulation clock. */
    sim_time = min_time_next_event;

}


void arrive(void)  /* Arrival event function. */
{
  clients_in_laundry++;

    /* Schedule next arrival. */
    if(sim_time <= sim_duration) {
        time_next_event[0] = sim_time + gen_next_interarrive();
    }
    else {
      time_next_event[0] = 1.0e+30; //infinito
    }

    /* Guardar el tiempo de arribo de esta entidad para los calculos estadisticos */
    Cliente c;
    c.timeArrived = sim_time;

    /* Check to see whether server is busy. */

    if (server_status == BUSY) {

        /* Server is busy, so increment number of customers in queue. */

	       push(c,&waiting_queue);
         num_in_q++;

        /* Check to see whether an overflow condition exists. */
        if (num_in_q > Q_LIMIT) {

            exit(2);
        }



    }

    else {

        /* Server libre, tener en  cuenta la entidad que pasa directamente al server para los calculos estadisticos */

        server_status = BUSY;

        /*Llevar el numero de clientes que han entrado*/

        served_clients++;

        /* Schedule a departure (service completion). */

        time_next_event[1] = sim_time + gen_next_depart();
    }
}


void depart(void)  /* Departure event function. */
{
    int   i;
    served_clients++;
    clients_in_laundry--;

    /* Check to see whether the queue is empty. */

    if (num_in_q == 0) {
      /* The queue is empty so make the server idle and eliminate the
      departure (service completion) event from consideration. */
      server_status = IDLE;
      time_next_event[1] = 1.0e+30; //infinito

    }

    else {

        /* The queue is nonempty, so decrement the number of customers in
           queue. */
        Cliente c = pop(&waiting_queue);
        num_in_q--;

        /*Schedule departure, and update the statistics acumulators. */
        time_next_event[1] = sim_time + gen_next_depart();
        laundry_total_time += (sim_time - c.timeArrived) + (time_next_event[1]-sim_time);
        queue_total_time += (sim_time - c.timeArrived);
      }

}


void report(void)  /* Report generator function. */
{
    /* Compute and write estimates of desired measures of performance. */
    printf("Tiempo medio de espera en la cola: %f\n",queue_total_time / (served_clients+0.0)); //media de demora en cola

    printf("Tiempo medio de espera en el local: %f\n", laundry_total_time / (served_clients+0.0)); //media de demora en el local

    printf("Cantidad media de gente en la cola: %f\n",mean_clients_queue / time_last_event ); //media de clientes en cola

    printf("Cantidad media de gente en el local: %f\n",mean_clients_laundry / time_last_event ); //media de clientes en local

    printf("Utilizacion de la lavadora: %f\n",server_usage / time_last_event); //Utilizacion de la lavadora


}

void update_time_avg_stats(void)  /* Update area accumulators for time-average
                                     statistics. */
{
    float time_since_last_event;

    /* Compute time since last event, and update last-event-time marker. */

    time_since_last_event = sim_time - time_last_event;
    time_last_event       = sim_time;

    /* Update area under number-in-queue function. */
    mean_clients_queue = mean_clients_queue + ((time_since_last_event)*num_in_q);
    mean_clients_laundry = mean_clients_laundry + ((time_since_last_event)*clients_in_laundry);

    /* Update area under server-busy indicator function. */
    server_usage = server_usage +((time_since_last_event)*server_status);
}

/*Generates the next arrive in base of given probabilities*/
float gen_next_interarrive()
{
  double randomNumber = generate_random_number();
  if (randomNumber <= 0.05) {
    return 5.0;
  }
  else {
    if (randomNumber <= 0.1) {
      return 10.0;
    }
    else {
      if (randomNumber <= 0.2) {
        return 15.0;
      }
      else {
        if (randomNumber <= 0.3) {
          return 20.0;
        }
        else {
          if (randomNumber <= 0.6) {
            return 25.0;
          }
          else {
            if (randomNumber <= 0.8) {
              return 30.0;
            }
            else {
              if (randomNumber <= 0.95) {
                return 35.0;
              }
              else {
                return 40.0;
              }
            }
          }
        }
      }
    }
  }
}
/*Generates the next depart in base of given probabilities*/
float  gen_next_depart()
{
    double randomNumber = generate_random_number();
    if (randomNumber <= 0.15) {
      return 10.0;
    }
    else {
      if (randomNumber <= 0.4) {
        return 20.0;
      }
      else {
        if (randomNumber <= 0.8) {
          return 30.0;
        }
        else {
          return 40.0;
        }
      }
    }
}
