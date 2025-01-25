import os
import matplotlib.pyplot as plt
import pandas as pd

# List of results files to analyze
# versions = ['v3-2AIE', 'v4-2AIE']
versions = ['v2-floats', 'v3-2AIE']
# versions = ['v2-floats']
results = ['4_clusters_avg.csv', '8_clusters_avg.csv', '12_clusters_avg.csv', '16_clusters_avg.csv']
names = ['4 Clusters', '8 Clusters', '12 Clusters', '16 Clusters']

# List of colors for plotting
# colors_all = ['#b2e2e2','#66c2a4','#2ca25f','#006d2c', '#bdd7e7','#6baed6','#3182bd','#08519c']
colors_all = ['#d7b5d8','#df65b0','#ce1256','#980043', '#b2e2e2','#66c2a4','#2ca25f','#006d2c']
# colors_all = ['#d7b5d8','#df65b0','#ce1256','#980043']

def plot_combined_speedup(data, title):
    plt.figure(figsize=(10, 6))
    
    for (version, cluster_name), cluster_data in data.items():
        # color_idx = 0 if version == 'v3-2AIE' else 1
        color_idx = 0 if version == 'v2-floats' else 1
        # color_idx = 0

        # Plot each version-cluster pair
        plt.plot(
            cluster_data['num_points'], 
            cluster_data['speedup'], 
            color=colors_all[color_idx * len(names) + names.index(cluster_name)],
            marker='o', 
            label=f'{version.split("-")[0]} - {cluster_name}'
        )

    # Draw a horizontal line at y=1
    plt.axhline(y=1, color='red', linestyle='--', linewidth=0.8)

    # Graph configuration
    plt.xscale('log')
    plt.xlabel('Number of Points (log scale)', fontsize=14)
    plt.ylabel('Speedup (sw_time / hw_time)', fontsize=14)
    plt.title(f'Speedup Comparison ({title})', fontsize=16)
    plt.legend(fontsize=12)
    plt.savefig(f"combined_speedup.pdf")
    plt.show()


def main():
    project_path = os.path.join(os.path.dirname(__file__), '..')
    combined_data = {}

    for result_file, cluster_name in zip(results, names):
        print(f"Analyzing {result_file} results")
        
        for version in versions:
            file_path = os.path.join(project_path, version, 'results', result_file)
            
            if not os.path.exists(file_path):
                print(f"File not found: {file_path}")
                continue

            # Read the CSV data into DataFrame
            df = pd.read_csv(file_path)
            df['speedup'] = df['sw_time'] / df['hw_time']

            # Store data keyed by version and cluster name
            combined_data[(version, cluster_name)] = df

    # Plot combined data
    plot_combined_speedup(combined_data, 'v2')


if __name__ == '__main__':
    main()