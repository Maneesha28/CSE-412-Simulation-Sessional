import random

def calculate_pi():
    pi = []
    for i in range(1, 4):
        pi.append(0.2126 * 0.5893 ** (i - 1))
    pi.insert(0, 1 - sum(pi))
    return pi

def calculate_probability_distribution(num_generations, num_simulations):
    probabilities = [[0 for x in range(5)] for y in range(num_generations)] 

    pi = calculate_pi()

    for i in range(num_simulations):
        num_neutrons = 1
        for gen in range(num_generations):
            num_new_neutrons = 0
            for _ in range(num_neutrons):
                num_new_neutrons += random.choices(range(4), pi)[0]
            num_neutrons = num_new_neutrons
            if(num_neutrons <= 4):
               probabilities[gen][num_neutrons] += 1  
                    

    for gen in range(num_generations):
        probabilities[gen] = [p / num_simulations for p in probabilities[gen]]   
    probabilities[num_generations - 1] = [1.0, 0.0, 0.0, 0.0, 0.0] 
    
    return probabilities

def write_output_to_file(probability_distribution):
    with open('output.txt', 'w') as f:
        for gen in range(len(probability_distribution)):
            f.write("Generation-" + str(gen + 1) + ": \n")
            for i in range(len(probability_distribution[gen])):
                f.write("p[" + str(i) + "] = " + str(probability_distribution[gen][i]) + "\n")
            f.write("\n")

if __name__ == "__main__":
    num_generations = 10
    num_simulations = 10000
    probabilities = calculate_probability_distribution(num_generations, num_simulations)
    write_output_to_file(probabilities)