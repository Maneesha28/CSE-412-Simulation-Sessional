# CSE-412-Simulation-Sessional

This repository contains the assignments of the course CSE 412: Simulation and Modeling Sessional.

## Assignment 1: Simulation of a Single Server Queueing System

In this assignment, we will simulate a Single-Server Queueing System to calculate its various measures of performance: **Average Delay in queue, the Average Number of Customers in the queue and the Average Server Utilization**, given *different values of mean inter-arrival time, the mean service time and the total number of delays*. 

## Assignment 2: Simulation of a Product Inventory System

In this assignment, we will simulate an Inventory System to compare different (s, S) policies for given unit costs. 

In the output file, you would first show all the necessary details of the Inventory System. Afterwards, you need to show the **Average Ordering, Holding, Shortage and Total Costs** for each given policy.

## Assignment 3: Project Scheduling with Triangular Distribution

**PERT (Program Evaluation and Review Technique) Chart Analysis** is a widely used project management tool for scheduling, organizing, and coordinating tasks within a certain project. The PERT Chart provides a graphical representation of a project’s timeline, enabling project managers to break down each individual task within the project for analysis.

In this assignment, we will analyze two simulated projects using information from their corresponding PERT charts. We will use following 3 (three) probability distributions in this assignment to introduce stochasticity in the simulation.

1. Triangular Triang(a, b, m) distribution
2. Right-triangular RT(a, b) distribution
3. Left-triangular LT(a, b) distribution

Here, a is location parameter, b-a is scaling parameter, and m is shape parameter. These distributions will be used to determine the duration of each task within a project.


## Assignment 4: Ouput Data Analysis

A spreadsheet simulation for single-server queueing system is to be done for output data analysis.

1. Given *rho* and *interarrival_time_mean*, simulate the attributes: **Interarrival Time (s), Arrival Time, Service Time(s), Time Service Begins, Time Service Ends, Waiting Time in Queue (Delay)** - with at least m = 500 customers in the system.

2. Simulate *Delay* value for m = 500 customers in the system, for at least n = 1000 replications, using **what-if analysis**.

3. The simulation described in step 1 and 2 should be repeated for different starting conditions (the number of customers, s = 0, 5, 10, 12, 15 at time 0).

4. For each starting condition, show the **plot of convergence of the transient mean, E(Di)** for n = 250, 500, 750, 1000 replications in a single graph considering m = 500 customers.

5. Finally, show the plot of E(Di) for n = 1000 replications and m = 500 customers for different starting conditions s in a single graph.

## Assignment 5: Monte Carlo Simulation

**Problem-1**

In a nuclear chain reaction, a neutron is bombarded to hit a nucleus of an atom (Uranium-235 or Plutonium-239). When an atom undergoes nuclear fission, a few neutrons (the expected number depends on several factors, usually between 2.5 and 3.0) are ejected from the reaction. These free neutrons will then interact with the surrounding material, and if more fissile fuel is present, some may be absorbed and cause more fissions. Thus, the cycle repeats to give a reaction that is self-sustaining.

Suppose *pi* is the probability that a neutron will result in a fission that produces *i* new
neutrons. Our task is to **calculate the probability distribution of the number of neutrons produced in the nth generation of a chain reaction**.

**Problem-2**

In this task, we will simulate the [Secretary Problem / Optimal Marriage Problem](https://www.npr.org/sections/krulwich/2014/05/15/312537965/how-to-marry-the-right-girl-a-mathematical-solution)
