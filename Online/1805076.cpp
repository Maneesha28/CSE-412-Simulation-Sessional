#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include "lcgrand.h"
using namespace std;

#define inf 1e+30
#define QUEUE_LIMIT 10000
enum server_states {BSY=1, IDLE=0};
enum event_types {EMPTY=0, ARRIVAL=1, DEPARTURE=2, SIM_END=3};

double simulation_clock, area_under_qt, area_under_bt, time_of_last_event, mean_interarrival_time, mean_service_time, total_delays;
int total_num_of_delays, num_customer_delayed, num_in_queue, server_status, next_event_type, total_num_of_events; 
int event_count = 0, arrived_customer_count = 0, departed_customer_count = 0;
double event_list[4], arrival_time[QUEUE_LIMIT+1]; // indexing from 1
ifstream in;
ofstream result_file, event_order_file;

//added for online
double simulation_end_time;
int queue_capacity, balked_customers = 0;

double expntl(double x) {
    // Your code here
    return -x*log(lcgrand(4));
}

void initialize() {
    // Your code here
    simulation_clock = 0.0;
    server_status = IDLE;
    num_in_queue = 0;
    time_of_last_event = 0.0;
    num_customer_delayed = 0;
    total_delays = 0.0;
    area_under_qt = 0.0;
    area_under_bt = 0.0;
    event_list[ARRIVAL] = simulation_clock + expntl(mean_interarrival_time);
    event_list[DEPARTURE] = inf;
    event_list[SIM_END] = simulation_end_time;
    total_num_of_events = 3; //arrival and departure and simulation end
}

void timing() {
    // Your code here
    double min_time_next_event = inf;
    next_event_type = EMPTY;
    for (int i = 1; i <= total_num_of_events; i++) {
        if (event_list[i] < min_time_next_event) {
            min_time_next_event = event_list[i];
            next_event_type = i;
        }        
    }
    if (next_event_type == EMPTY) {
        // simulation terminated
        result_file << "Event list empty at time " << simulation_clock << endl;
        exit(1);
    }

    simulation_clock = min_time_next_event;
}

void arrive() {
    // Your code here
    // event_order_file << ++event_count << ". Next event: Customer " << ++arrived_customer_count << " Arrival" << endl << endl;
    event_list[ARRIVAL] = simulation_clock + expntl(mean_interarrival_time);
    if (server_status == BSY) {
        // num_in_queue++;
        // if (num_in_queue > queue_capacity) {
        //     result_file << "Overflow of the array time_arrival at time " << simulation_clock << endl;
        //     exit(2);
        // }
        // arrival_time[num_in_queue] = simulation_clock;

        // if queue is full, customer will balk
        if (num_in_queue == queue_capacity) {
            balked_customers++;
        } else {
            num_in_queue++;
            arrival_time[num_in_queue] = simulation_clock;
        }
    } else {
        //customers_in_service++;
        num_customer_delayed++;
        server_status = BSY;
        event_list[DEPARTURE] = simulation_clock + expntl(mean_service_time);
        // event_order_file << "---------No. of customers delayed: " << num_customer_delayed << "---------" << endl << endl;
    }
    
}

void depart() {
    // Your code here
    // event_order_file << ++event_count << ". Next event: Customer " << ++departed_customer_count << " Departure" << endl << endl;
    if (num_in_queue == 0) {
        server_status = IDLE;
        event_list[2] = inf;
    } else {
        num_in_queue--;
        double delay = simulation_clock - arrival_time[1];
        total_delays += delay;
        num_customer_delayed++;
        event_list[DEPARTURE] = simulation_clock + expntl(mean_service_time);
        // event_order_file << "---------No. of customers delayed: " << num_customer_delayed << "---------" << endl << endl;
        for (int i = 1; i <= num_in_queue; i++) {
            arrival_time[i] = arrival_time[i+1];
        }
    }
}

void update_time_avg_stats() {
    // Your code here
    double time_since_last_event;
    time_since_last_event = simulation_clock - time_of_last_event;
    time_of_last_event = simulation_clock;
    area_under_qt += num_in_queue * time_since_last_event;
    area_under_bt += server_status * time_since_last_event;
}

void read_input_from_file() {
    // Your code here
    string input_filename;
    cout << "Enter input filename: ";
    cin >> input_filename;
    in.open(input_filename.c_str());

    if (!in.is_open()) {
        cout << "Error opening file input.txt" << endl;
        exit(1);
    }
    in >> mean_interarrival_time >> mean_service_time >> simulation_end_time >> queue_capacity;
    in.close();
}

void write_to_result_file() {
    // Your code here
    result_file.open("results.txt");
    if (!result_file.is_open()) {
        cout << "Error opening file results.txt" << endl;
        return;
    }

    result_file << setprecision(6) << fixed;

    result_file << "----Single-Server Queueing System----" << endl << endl;
    result_file << "Mean inter-arrival time: " << mean_interarrival_time << " minutes" << endl;
    result_file << "Mean service time: " << mean_service_time << " minutes" << endl;
    result_file << "Maximum Queue Length: " << queue_capacity << endl;
    result_file << endl;

    result_file << "Avg delay in queue: " << total_delays / num_customer_delayed << " minutes" << endl;
    result_file << "Avg number in queue: " << area_under_qt / simulation_clock << endl;
    result_file << "Server utilization: " << area_under_bt / simulation_clock << endl;
    result_file << "Time simulation ended: " << simulation_clock << " minutes" << endl;
    result_file << endl;

    result_file << "Total number of Balking customers: " << balked_customers << endl;
    result_file << "Total number of customers delayed: " << num_customer_delayed << endl;
    result_file << "Percentage of Balking customers: " << ((balked_customers*1.0)/(balked_customers+num_customer_delayed))*100 << " %" << endl;
    
    result_file.close();
}

void run_simulation()
{
    initialize();
    do {
        timing();
        update_time_avg_stats();
        switch (next_event_type) {
            case ARRIVAL:
                arrive();
                break;
            case DEPARTURE:
                depart();
                break;
            case SIM_END:
                break;
        }
    } while (next_event_type != SIM_END);
}

int main() {
    // Your code here
    read_input_from_file();

    // event_order_file.open("event_orders.txt");

    // if (!event_order_file.is_open()) {
    //     cout << "Error opening file event_orders.txt" << endl;
    //     return 1;
    // }

    run_simulation();

    write_to_result_file();
    // event_order_file.close();

    return 0;
}
