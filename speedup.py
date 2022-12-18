import subprocess
import sys

# Path to executables
path_to_a = sys.argv[1]
path_to_b = sys.argv[2]

# List of CPU counts to test
cpu_counts = [1, 2, 3, 4, 5, 6, 7, 8]

# Initialize table
table = [['N runs'] + [str(x) + ' CPU' for x in cpu_counts]]

# Iterate over n_runs values
for n_runs in sys.argv[3:]:
    n_runs = int(n_runs)
    row = [str(n_runs)]
    for cpu_count in cpu_counts:
        # Initialize total time for A and B
        total_time_a = 0
        total_time_b = 0
        
        # Run codes and sum up times
        for i in range(n_runs):
            # Run parallel code
            output_a = subprocess.run([path_to_a, str(cpu_count)], stderr=subprocess.PIPE, universal_newlines=True)
            time_a = float(output_a.stderr.strip())
            total_time_a += time_a
            
            # Run serial code
            output_b = subprocess.run([path_to_b], stderr=subprocess.PIPE, universal_newlines=True)
            time_b = float(output_b.stderr.strip())
            total_time_b += time_b
        
        # Calculate average time for A and B
        avg_time_a = total_time_a / n_runs
        avg_time_b = total_time_b / n_runs
        
        # Calculate average speedup
        avg_speedup = avg_time_b / avg_time_a
        row.append(str(avg_speedup))
    table.append(row)

# Print table
for row in table:
    print(' | '.join(row))
