import matplotlib.pyplot as plt # type: ignore

MIN_MARK = 30
MAX_MARK = 150

class Sim:
    def __init__(self, filename):
        self.N = None
        self.width = None
        self.height = None
        self.min_mass = None
        self.max_mass = None

        """
        Dictionary of the form:
            timesteps[i] == [(x1, y1, m1), (x2, y2, m2) ...]
        """
        self.timesteps = {}

        self.read_sim_file(filename)

    """
    Reads given .sim file to populate our sim's timestep data and metadata
    """
    def read_sim_file(self, filename):
        f = open(filename, 'r')

        ## read metadata
        self.N = int(f.readline().strip())
        self.width = int(f.readline().strip())
        self.height = int(f.readline().strip())
        self.min_mass = int(f.readline().strip())
        self.max_mass = int(f.readline().strip())

        line = f.readline() ## blank

        ## read timesteps
        while line:
            ts = int(f.readline().strip())
            self.timesteps[ts] = []
            while True:
                line = f.readline()
                if not line or line == '\n':
                    break
                x, y, m = line.strip().split(" ")
                x = float(x)
                y = float(y)
                m = float(m)
                self.timesteps[ts].append((x, y, m))
        
    """
    Visualises simulation
    """
    def visualise(self, output_dir):
        for timestep, particles in self.timesteps.items():
            x = [p[0] for p in particles]
            y = [p[1] for p in particles]
            m = [p[2] for p in particles]

            ## compute marker sizes, normalised within range [MIN_MARK, MAX_MARK] for best display
            s = [self.change_range(mass, self.min_mass, self.max_mass, MIN_MARK, MAX_MARK) for mass in m]

            plt.figure()
            plt.scatter(x, y, s=s, c=m, cmap='viridis', alpha=0.6)
            plt.colorbar(label='Mass')
            plt.xlabel('X')
            plt.ylabel('Y')
            plt.title(f'Timestep {timestep}')
            plt.xlim(0, self.width+1)
            plt.ylim(0, self.height+1)
            plt.savefig(f'{output_dir}/timestep_{timestep}.png')
            plt.close()
    
    """
    Takes x from range [prev_min, prev_max] to range [after_min, after_max]
    """
    def change_range(self, x, prev_min, prev_max, after_min, after_max):
        return (x - prev_min) / (prev_max - prev_min) * (after_max - after_min) + after_min
        

def main():
    filename = "sims/code_out.sim"
    sim = Sim(filename)
    sim.visualise("vis")
    x, a, b, c, d = 500, 1, 1000, 30, 150
    res = sim.change_range(x, a, b, c, d)
    print(res)

if __name__ == '__main__':
    main()