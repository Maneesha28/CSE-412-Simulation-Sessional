#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include "lcgrand.h"
using namespace std;

#define inf 1e+30
#define TOTAL_NUM_OF_EVENTS 4

ifstream in;
ofstream out;

enum event_types {EMPTY=0, ORDER_ARRIVAL=1, DEMAND=2, SIMULATION_END=3, INVENTORY_CHECK=4};

int initial_inventory_level, num_of_months, num_of_policies, num_of_demand, small_s, cap_s;
int next_event_type, order_amount;
double mean_inter_demand_time, setup_cost, per_unit_incremental_cost, holding_cost, shortage_cost, min_lag, max_lag;
double simulation_clock, current_inventory_level, last_event_time, total_order_cost, area_holding, area_shortage;
double *demand_size_probabilities;
double event_list[5];

//added for online
int total_num_of_trials;
double exp_setup_cost, exp_per_unit_incremental_cost, exp_min_lag, exp_max_lag;
double total_cost, exp_order_count;

double expntl(double x) {
    // Your code here
    return -x*log(lcgrand(1));
}

double uniform(double a, double b) {
    // Your code here
    return a + (b-a)*lcgrand(1);
}

int random_integer(double demand_size_probabilities[]) {
    // Your code here
    double u = lcgrand(1);
    int index;

    // for (i = 1; u >= demand_size_probabilities[i]; i++);
    for (int i = 1; i <= num_of_demand; i++) {
        if (u < demand_size_probabilities[i]) {
            index = i;
            break;
        }
    }

    return index;
}

void initialize() {
    // Your code here
    simulation_clock = 0.0;
    current_inventory_level = initial_inventory_level;
    last_event_time = 0.0;
    total_order_cost = 0.0;
    area_holding = 0.0;
    area_shortage = 0.0;
    event_list[ORDER_ARRIVAL] = inf;
    event_list[DEMAND] = simulation_clock + expntl(mean_inter_demand_time);
    event_list[SIMULATION_END] = num_of_months;
    event_list[INVENTORY_CHECK] = 0.0;
}

void timing() {
    double min_time = inf;
    next_event_type = EMPTY;

    // Determine the event with the minimum time
    for (int i = 1; i <= TOTAL_NUM_OF_EVENTS; i++) {
        if (event_list[i] < min_time) {
            min_time = event_list[i];
            next_event_type = i;
        }
    }
    simulation_clock = min_time;
}


void update_time_avg_stats() {
    double time_since_last_event = simulation_clock - last_event_time;
    last_event_time = simulation_clock;

    if(current_inventory_level < 0)
        area_shortage -= current_inventory_level * time_since_last_event;
    else if(current_inventory_level > 0)
        area_holding += current_inventory_level * time_since_last_event;

}

void order_arrival() {
    current_inventory_level += order_amount;
    event_list[ORDER_ARRIVAL] = inf;

}

void demand() {
    current_inventory_level -= random_integer(demand_size_probabilities);
    event_list[DEMAND] = simulation_clock + expntl(mean_inter_demand_time);
}

void inventory_check(bool is_exp=false) {
    if(is_exp) {
        if(current_inventory_level < 0) {
            order_amount = - current_inventory_level;
            total_order_cost += exp_setup_cost + exp_per_unit_incremental_cost * order_amount;
            event_list[ORDER_ARRIVAL] = simulation_clock + uniform(exp_min_lag, exp_max_lag);
            exp_order_count++;
        }
        else {
            if(current_inventory_level < small_s) {
                order_amount = cap_s - current_inventory_level;
                total_order_cost += setup_cost + per_unit_incremental_cost * order_amount;
                event_list[ORDER_ARRIVAL] = simulation_clock + uniform(min_lag, max_lag);
            }
        }

    }
    else {
        if(current_inventory_level < small_s) {
            order_amount = cap_s - current_inventory_level;
            total_order_cost += setup_cost + per_unit_incremental_cost * order_amount;
            event_list[ORDER_ARRIVAL] = simulation_clock + uniform(min_lag, max_lag);
        }
    }
    event_list[INVENTORY_CHECK] = simulation_clock + 1.0;
}

void report() {
    double avg_holding_cost = holding_cost * area_holding / num_of_months;
    double avg_shortage_cost = shortage_cost * area_shortage / num_of_months;
    double avg_order_cost = total_order_cost / num_of_months;

    total_cost += avg_order_cost+avg_holding_cost+avg_shortage_cost;

    // out << "(" << small_s << ", " << cap_s << ")" << "            " 
    // << avg_order_cost+avg_holding_cost+avg_shortage_cost << "             " << endl << endl;
}

void get_inventory_details() {

    in >> total_num_of_trials;

    in >> initial_inventory_level >> num_of_months >> num_of_policies;
    in >> num_of_demand >> mean_inter_demand_time;
    in >> setup_cost >> per_unit_incremental_cost >> holding_cost >> shortage_cost;
    
    in >> exp_setup_cost >> exp_per_unit_incremental_cost;

    in >> min_lag >> max_lag;

    in >> exp_min_lag >> exp_max_lag;

    demand_size_probabilities = new double[num_of_demand+1];
    for (int i = 1; i <= num_of_demand; i++) {
        in >> demand_size_probabilities[i];
    }
}

void print_inventory_details() {
    out << "------Single-Product Inventory System------" << endl << endl;
    out << "Initial inventory level: " << initial_inventory_level << " items" << endl << endl;
    out << "Number of demand sizes: " << num_of_demand << endl << endl;
    out << "Distribution function of demand sizes: ";
    for (int i = 1; i <= num_of_demand; i++) {
        out << demand_size_probabilities[i] << " ";
    }
    out << endl << endl;
    out << "Mean inter-demand time: " << mean_inter_demand_time << " months" << endl << endl;
    out << "Delivery lag range: " << min_lag << " to " << max_lag << " months" << endl << endl;

    out << "Express Order lag range: " << exp_min_lag << " to " << exp_max_lag << " months" << endl << endl;

    out << "Length of simulation: " << num_of_months << " months" << endl << endl;
    out << "Costs: " << endl;
    out << "K = " << setup_cost << endl;
    out << "i = " << per_unit_incremental_cost << endl;
    out << "h = " << holding_cost << endl;
    out << "pi = " << shortage_cost << endl;

    out << "E_K = " << exp_setup_cost << endl;
    out << "E_i = " << exp_per_unit_incremental_cost << endl;

    out << endl;

    out << "Number of policies: " << num_of_policies << endl;

    out << "Number of Trials: " << total_num_of_trials << endl << endl;

    out << "Policies: " << endl;
    out << "--------------------------------------------------------------------------------------------------" << endl;
    out << " Policy        " << "Avg_before_express     " << "Avg_after_express      " << "Expected_express_count" << endl;
    out << "--------------------------------------------------------------------------------------------------" << endl;
    out << endl;

}

void run_simulation() {
    // Your code here
    in >> small_s >> cap_s;

    total_cost = 0;

    for(int i = 1; i <= total_num_of_trials; i++) {
        initialize();
        
        do {
            timing();
            update_time_avg_stats();
            switch (next_event_type) {
                case ORDER_ARRIVAL:
                    order_arrival();
                    break;
                case DEMAND:
                    demand();
                    break;
                case SIMULATION_END:
                    report();
                    break;
                case INVENTORY_CHECK:
                    inventory_check();
                    break;
            }
        } while (next_event_type != SIMULATION_END);
    }

    double total_cost_across_trials = total_cost / total_num_of_trials;
        out << "(" << small_s << ", " << cap_s << ")" << "\t\t\t\t" 
    << total_cost_across_trials << "\t\t\t\t";


    total_cost = 0;
    exp_order_count = 0.0;

    for(int i = 1; i <= total_num_of_trials; i++) {
        initialize();
        
        do {
            timing();
            update_time_avg_stats();
            switch (next_event_type) {
                case ORDER_ARRIVAL:
                    order_arrival();
                    break;
                case DEMAND:
                    demand();
                    break;
                case SIMULATION_END:
                    report();
                    break;
                case INVENTORY_CHECK:
                    inventory_check(true);
                    break;
            }
        } while (next_event_type != SIMULATION_END);
    }

    total_cost_across_trials = total_cost / total_num_of_trials;
    exp_order_count /= total_num_of_trials*num_of_months;
    out << total_cost_across_trials << "\t\t\t\t" << exp_order_count << endl << endl;

        
}


int main() {
    // Your code here
    string input_filename;
    cout << "Enter input filename: ";
    cin >> input_filename;
    in.open(input_filename.c_str());
    // in.open("in2.txt");
    out.open("out.txt");

    if (!in.is_open()) {
        cout << "Error opening file input.txt" << endl;
        exit(1);
    }
    if(!out.is_open()) {
        cout << "Error opening file out.txt" << endl;
        exit(1);
    }

    get_inventory_details();

    out << fixed << setprecision(2);

    print_inventory_details();

    for (int i = 1; i <= num_of_policies; i++) {
        run_simulation();
    }

    // out << "--------------------------------------------------------------------------------------------------" << endl;

    in.close();
    out.close();

    return 0;
}
