import matplotlib.pyplot as plt

class Sim:
    def __init__(self, filename):
        self.N = None
        self.width = None
        self.height = None

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
    Visualises 
    """
    def visualise(self, output_dir):
        for timestep, particles in self.timesteps.items():
            x = [p[0] for p in particles]
            y = [p[1] for p in particles]
            m = [p[2] for p in particles]

            plt.figure()
            plt.scatter(x, y, s=[mass * 10 for mass in m], c=m, cmap='viridis', alpha=0.6)
            plt.colorbar(label='Mass')
            plt.xlabel('X')
            plt.ylabel('Y')
            plt.title(f'Timestep {timestep}')
            plt.xlim(0, 100)
            plt.ylim(0, 100)
            plt.savefig(f'{output_dir}/timestep_{timestep}.png')
            plt.close()

def main():
    filename = "sims/code_out.sim"
    sim = Sim(filename)
    sim.visualise("vis")

if __name__ == '__main__':
    main()