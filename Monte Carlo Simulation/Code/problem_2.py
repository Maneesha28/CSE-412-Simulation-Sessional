import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d

def find_probability_of_success(num_candidates, success_criterion, num_simulations):
    success_rate = np.zeros(num_candidates)
    for _ in range(num_simulations):
        candidates = np.random.randint(1, num_candidates, num_candidates)

        for j in range(num_candidates):
            std_candidate = 0
            chosen_candidate = num_candidates-1
            for k in range(j):
                if(candidates[k] > candidates[std_candidate]):
                    std_candidate = k
            for k in range(j, num_candidates):
                if(candidates[k] >= candidates[std_candidate]):
                    chosen_candidate = k
                    break
            
            sorted_candidates = np.sort(candidates)[::-1]
            if (sorted_candidates[success_criterion-1] <= candidates[chosen_candidate]):
                success_rate[j] += 1

    return (success_rate*100)/num_simulations

def plot_graph(num_candidates, success_rates, s):
    x = np.arange(1, num_candidates+1)
    f = interp1d(x, success_rates, kind='cubic')
    x_smooth = np.linspace(1, num_candidates, 300)
    plt.plot(x_smooth, f(x_smooth), label=f"{s} Success Criteria")
    plt.xlabel('Sample Size (m)')
    plt.ylabel('Success Rate (%)')
    plt.title('Success Rate vs. Sample Size')
    plt.ylim(0, 100)
    plt.legend()
    plt.grid(True)
    plt.savefig('success_criteria' + str(s) + '.png')
    plt.clf()

if __name__ == "__main__":

    # Define parameters
    num_simulations = 1000
    num_candidates = 100
    success_criteria = [1, 3, 5, 10]

    for s in success_criteria:
        success_rates = find_probability_of_success(num_candidates, s, num_simulations)
        plot_graph(num_candidates, success_rates, s)
