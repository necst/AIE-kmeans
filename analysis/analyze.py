import os
import matplotlib.pyplot as plt
import pandas as pd

# List of results files to analyze
# versions = ['v2-floats', 'v2-integers', 'v3-1AIE', 'v3-2AIE', 'v3-4AIE', 'v3-8AIE', 'v3-16AIE', 'v3-32AIE', 'v4-2AIE']
# colors = ['#fee391', '#df65b0', '#a1d99b', '#74c476', '#41ab5d', '#238b45', '#006d2c', '#00441b', '#6baed6'] 
# all versions

# versions = ['v3-2AIE', 'v4-2AIE']
# colors = ['#74c476','#6baed6']
# v3 - v4

# versions = ['v2-floats', 'v2-integers']
# colors = ['#fee391', '#df65b0']
# v2

# versions = ['v2-floats', 'v3-1AIE']
# colors = ['#fee391', '#a1d99b']
# v2 - v3

# versions = ['v4-2AIE', 'v5-2AIE', 'v6-2AIE']
# colors = ['#6baed6', '#fd8d3c', '#9e9ac8']
# v4 - v5 - v6

versions = ['v2-floats', 'v2-integers', 'v3-1AIE', 'v3-2AIE', 'v3-4AIE', 'v3-8AIE', 'v3-16AIE', 'v3-32AIE']
colors = ['#fee391', '#df65b0', '#a1d99b', '#74c476', '#41ab5d', '#238b45', '#006d2c', '#00441b']
# v3

results = ['4_clusters_avg.csv', '8_clusters_avg.csv', '12_clusters_avg.csv', '16_clusters_avg.csv']
names = ['4 Clusters', '8 Clusters', '12 Clusters', '16 Clusters']

# List of colors for plotting


# Plots speedup for multiple versions from a single result file.
def plot_speedup(data, result_file, title):
    plt.figure(figsize=(10, 6))

    for version, color in zip(versions, colors):
        # Plot speedup for the current version
        plt.plot(
            data[version]['num_points'], 
            data[version]['speedup'], 
            marker='o', 
            color=color, 
            label=f'Version {version}'
        )

    # filename = result_file.split('.')[0]
    # filename = result_file.split('.')[0].replace("_avg", "_v3-v4")
    # filename = result_file.split('.')[0].replace("_avg", "_v2")
    # filename = result_file.split('.')[0].replace("_avg", "_v2-v3")
    # filename = result_file.split('.')[0].replace("_avg", "_v4-v5-v6")
    filename = result_file.split('.')[0].replace("_avg", "_v3")

    # Draw an horizontal line at y=1
    plt.axhline(y=1, color='red', linestyle='--', linewidth=0.6)

    # Graph configuration
    plt.xscale('log')
    plt.xlabel('Number of Points (log scale)', fontsize=14)
    plt.ylabel('Speedup (sw_time / hw_time)', fontsize=14)
    plt.title(f'Speedup ({title})', fontsize=16)
    plt.legend(fontsize=12)
    plt.show()
    plt.savefig(f"{filename}.pdf")


def main():
    project_path = os.path.join(os.path.dirname(__file__), '..')

    for result_file in results:
        print(f"Analyzing {result_file} results")
        
        # Dictionary to store data for each version
        data = {}

        for version in versions:
            file_path = os.path.join(project_path, version, 'results', result_file)
            
            if not os.path.exists(file_path):
                print(f"File not found: {file_path}")
                continue

            # Read the CSV data into DataFrame
            df = pd.read_csv(file_path)
            df['speedup'] = df['sw_time'] / df['hw_time']
            data[version] = df

        plot_speedup(data, result_file, names[results.index(result_file)])  



if __name__ == '__main__':
    main()