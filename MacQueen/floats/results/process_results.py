import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns

# Read the results from the csv file
def read_csv(file_path):
    with open(file_path, 'r') as f:
        lines = f.readlines()

    data = []
    for line in lines:
        values = line.strip().split(',')
        
        # Skip lines without valid data
        if values[0] == "Number of clusters" or not values[0].isdigit():
            continue

        # Convert "2^x" notation to integer number of points
        # num_points = int(values[1].split('^')[0]) ** int(values[1].split('^')[1])
        data.append([int(values[0]), int(values[1]), int(values[2]), int(values[3])])

    # Create DataFrame with column names
    df = pd.DataFrame(data, columns=['num_clusters', 'num_points', 'sw_time', 'hw_time'])
    return df

# Create a scatter plot of the results and save it to a jpeg file
def plot_results(df_melted, num_clusters, figsize, num):
    # Ensure seaborn's color palette distinguishes between Hardware and Software
    palette = {"Hardware time": "blue", "Software time": "orange"}
    
    # Plot using seaborn
    plt.figure(figsize=figsize)
    sns.lineplot(data=df_melted, x="num_points", y="Time (μs)", style="Type", markers=True, hue="Type", palette=palette)
    
    plt.xscale("log")
    plt.yscale("log")
    plt.xlabel("Number of Points (log 2 scale)", fontsize=12)
    plt.ylabel("Time (μs, log scale)", fontsize=12)

    # Set x-axis ticks to powers of 2
    # x_ticks = [2**i for i in range(2, 27, 4)]
    # plt.xticks(x_ticks, [f"$2^{{{i}}}$" for i in range(2, 27, 4)])

    plt.title(f"{num_clusters} Clusters", fontsize=14)
    plt.legend()

    plt.grid(True, which="both", linestyle="--", lw=0.5)
    plt.savefig(f"{num_clusters}_clusters{num}.png")
    plt.show()
    plt.close()


def main():
    files = ['time1.csv', 'time2.csv', 'time3.csv', 'time4.csv', 'time5.csv', 'time6.csv']

    # Make the average plot and concatenate all the csv files
    combined_df = pd.concat([read_csv(file) for file in files])
    combined_df = combined_df.groupby(["num_clusters", "num_points"]).mean().reset_index()
    combined_df_melted = combined_df.melt(id_vars=["num_clusters", "num_points"],
                                          value_vars=["sw_time", "hw_time"],
                                          var_name="Type", value_name="Time (μs)")

    # Rename "Type" values for clearer labels
    combined_df_melted["Type"] = combined_df_melted["Type"].replace({"sw_time": "Software time", "hw_time": "Hardware time"})
    
    for cluster in combined_df_melted["num_clusters"].unique():
        plot_results(combined_df_melted[combined_df_melted["num_clusters"] == cluster], cluster, figsize=(10, 6), num="avg")

        # Create a CSV file with the average results
        combined_df[combined_df["num_clusters"] == cluster].to_csv(f"{cluster}_clusters_avg.csv", index=False)

if __name__ == '__main__':
    main()
