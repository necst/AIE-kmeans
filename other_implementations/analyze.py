import csv, os
import json
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
import pandas as pd

versions = ["4", "8", "12", "16", "32"]

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


def average_time(results):
    # Read the results CSV file into a DataFrame
    df = pd.read_csv(results)

    # Group by number of clusters and number of points and calculate the average times
    avg_results = df.groupby(["num_clusters", "num_points"]).mean().reset_index()

    # Melt the DataFrame to have a single column for the time values
    avg_results_melted = avg_results.melt(id_vars=["num_clusters", "num_points"],
                                            value_vars=["faiss_time_cpu", "faiss_time_gpu", "sklearn_time"],
                                            var_name="Type", value_name="Time (us)")
        
    # Rename "Type" values for clearer labels
    avg_results_melted["Type"] = avg_results_melted["Type"].replace({"faiss_time_cpu": "Faiss Time (CPU)", 
                                                                     "faiss_time_gpu": "Faiss Time (GPU)", 
                                                                     "sklearn_time": "Sklearn Time"})
    
    for cluster in avg_results_melted["num_clusters"].unique():
        # Create a CSV file with the average results
        avg_results[avg_results["num_clusters"] == cluster].to_csv(f"{cluster}_clusters_avg.csv", index=False)


def concat_files(file1, file2):
    df1 = pd.read_csv(file1)
    df2 = pd.read_csv(file2)
    df = pd.concat([df1, df2], axis=1)
    df.to_csv(f"{file1.split('_')[0]}_clusters_avg_combined.csv", index=False)


# Plot average results
def plot_results(avg_results, num_clusters):
    # Read the average results from the CSV file
    df = pd.read_csv(avg_results)
    df_melted = df.melt(id_vars=["num_clusters", "num_points"],
                        value_vars=["faiss_time_cpu", "faiss_time_gpu", "sklearn_time", "sw_time", "hw_time"],
                        var_name="Type", value_name="Time (μs)")
    
    # Rename "Type" values for clearer labels
    df_melted["Type"] = df_melted["Type"].replace({"faiss_time_cpu": "Faiss Time (CPU)", "faiss_time_gpu": "Faiss Time (GPU)", "sklearn_time": "Sklearn Time",
                                                    "sw_time": "Software time", "hw_time": "Hardware time"})

    plt.figure(figsize=(10, 6))
    sns.lineplot(data=df_melted, x="num_points", y="Time (μs)", style="Type", markers=True, hue="Type")

    plt.xscale("log")
    plt.yscale("log")
    plt.xlabel("Number of Points (log 2 scale)", fontsize=12)
    plt.ylabel("Time (μs, log scale)", fontsize=12)
    plt.title(f"{num_clusters} Clusters", fontsize=14)
    plt.legend()

    plt.savefig(f"{num_clusters}_clusters.png")
    plt.savefig(f"{num_clusters}_clusters.pdf")

    plt.show()
    plt.close()
        

def average_aie_time(files):
    combined_df = pd.concat([read_csv(file) for file in files])
    combined_df = combined_df.groupby(["num_clusters", "num_points"]).mean().reset_index()
    combined_df_melted = combined_df.melt(id_vars=["num_clusters", "num_points"],
                                          value_vars=["sw_time", "hw_time"],
                                          var_name="Type", value_name="Time (μs)")

    # Rename "Type" values for clearer labels
    combined_df_melted["Type"] = combined_df_melted["Type"].replace({"sw_time": "Software time", "hw_time": "Hardware time"})
    
    for cluster in combined_df_melted["num_clusters"].unique():
        combined_df[combined_df["num_clusters"] == cluster].to_csv(f"{cluster}_clusters_avg_aie.csv", index=False)


def combine_results(output_file, csv_file):
    with open(output_file, 'r') as f:
        output_lines = f.readlines()

    with open(csv_file, 'r') as f:
        csv_lines = f.readlines()

    data = []
    for i, line in enumerate(output_lines):
        values = line.strip().split('\t')
        num_clusters = int(values[0])
        num_points = int(values[1])
        sw_result = [float(values[2]), float(values[3])]
        hw_result = [float(values[4]), float(values[5])]

        # Append the cluster data from the csv file
        csv_data = csv_lines[i].strip().split(',')
        if int(csv_data[0]) == num_clusters and int(csv_data[1]) == num_points:
            faiss_x, faiss_y = csv_data[2].replace('[', '').replace(']', '').split()
            scikit_x, scikit_y = csv_data[4].replace('[', '').replace(']', '').split()

            faiss_cpu = [float(faiss_x), float(faiss_y)]
            scikit = [float(scikit_x), float(scikit_y)]

            data.append([num_clusters, num_points, sw_result, hw_result, faiss_cpu, scikit])


    # Create DataFrame with column names
    df = pd.DataFrame(data, columns=['num_clusters', 'num_points', 'sw', 'hw', 'faiss', 'scikit'])
    df.to_csv("combined_results.csv", index=False)


def check_results(file):
    df = pd.read_csv(file)
    
    sw = np.array([np.fromstring(x.strip('[]'), sep=',') for x in df["sw"]])
    hw = np.array([np.fromstring(x.strip('[]'), sep=',') for x in df["hw"]])
    faiss = np.array([np.fromstring(x.strip('[]'), sep=',') for x in df["faiss"]])
    scikit = np.array([np.fromstring(x.strip('[]'), sep=',') for x in df["scikit"]])

    # Calculate the percentage difference between the software and faiss_cpu results
    diff_sw_faiss = np.abs(sw - faiss) / faiss * 100
    mean_sw_faiss = np.mean(diff_sw_faiss, axis=1)

    # Calculate the percentage difference between the hardware and faiss_cpu results
    diff_hw_faiss = np.abs(hw - faiss) / faiss * 100
    mean_hw_faiss = np.mean(diff_hw_faiss, axis=1)

    # Calculate the percentage difference between the software and scikit results
    diff_sw_scikit = np.abs(sw - scikit) / scikit * 100
    mean_sw_scikit = np.mean(diff_sw_scikit, axis=1)

    # Calculate the percentage difference between the hardware and scikit results
    diff_hw_scikit = np.abs(hw - scikit) / scikit * 100
    mean_hw_scikit = np.mean(diff_hw_scikit, axis=1)

    # Calculate the percentage difference between the faiss_cpu and scikit results
    diff_faiss_scikit = np.abs(faiss - scikit) / scikit * 100
    mean_faiss_scikit = np.mean(diff_faiss_scikit, axis=1)

    # Find the indices of the maximum differences
    max_sw_faiss_index = np.argmax(mean_sw_faiss)
    max_hw_faiss_index = np.argmax(mean_hw_faiss)
    max_hw_scikit_index = np.argmax(mean_hw_scikit)
    max_sw_scikit_index = np.argmax(mean_sw_scikit)
    max_faiss_scikit_index = np.argmax(mean_faiss_scikit)

    # Retrieve the rows corresponding to the maximum errors
    max_sw_faiss_row = df.iloc[max_sw_faiss_index]
    max_hw_faiss_row = df.iloc[max_hw_faiss_index]
    max_hw_scikit_row = df.iloc[max_hw_scikit_index]
    max_sw_scikit_row = df.iloc[max_sw_scikit_index]
    max_faiss_scikit_row = df.iloc[max_faiss_scikit_index]

    # Print results
    print(f"Average percentage difference (software vs. faiss_cpu): {np.mean(mean_sw_faiss):.2f}%")
    print(f"Average percentage difference (hardware vs. faiss_cpu): {np.mean(mean_hw_faiss):.2f}%")
    print(f"Average percentage difference (hardware vs. scikit): {np.mean(mean_hw_scikit):.2f}%")
    print(f"Average percentage difference (software vs. scikit): {np.mean(mean_sw_scikit):.2f}%")
    print(f"Average percentage difference (faiss_cpu vs. scikit): {np.mean(mean_faiss_scikit):.2f}%")

    print(f"Maximum percentage difference (software vs. faiss_cpu): {mean_sw_faiss[max_sw_faiss_index]:.2f}%")
    print(f"Maximum percentage difference (hardware vs. faiss_cpu): {mean_hw_faiss[max_hw_faiss_index]:.2f}%")
    print(f"Maximum percentage difference (hardware vs. scikit): {mean_hw_scikit[max_hw_scikit_index]:.2f}%")
    print(f"Maximum percentage difference (software vs. scikit): {mean_sw_scikit[max_sw_scikit_index]:.2f}%")
    print(f"Maximum percentage difference (faiss_cpu vs. scikit): {mean_faiss_scikit[max_faiss_scikit_index]:.2f}%")

    print("\nConfiguration with maximum software error (faiss):")
    print(max_sw_faiss_row)
    print("\nConfiguration with maximum hardware error (faiss):")
    print(max_hw_faiss_row)
    print("\nConfiguration with maximum hardware error (scikit):")
    print(max_hw_scikit_row)
    print("\nConfiguration with maximum software error (scikit):")
    print(max_sw_scikit_row)
    print("\nConfiguration with maximum faiss_cpu error (scikit):")
    print(max_faiss_scikit_row)

    with open("max_errors.txt", "w") as f:
        f.write(f"Average percentage difference (software vs. faiss_cpu): {np.mean(mean_sw_faiss):.2f}%\n")
        f.write(f"Average percentage difference (hardware vs. faiss_cpu): {np.mean(mean_hw_faiss):.2f}%\n")
        f.write(f"Average percentage difference (hardware vs. scikit): {np.mean(mean_hw_scikit):.2f}%\n")
        f.write(f"Average percentage difference (software vs. scikit): {np.mean(mean_sw_scikit):.2f}%\n")
        f.write(f"Average percentage difference (faiss_cpu vs. scikit): {np.mean(mean_faiss_scikit):.2f}%\n\n")

        f.write(f"Configuration with maximum software error (faiss):\n{max_sw_faiss_row}\n\n")
        f.write(f"Configuration with maximum hardware error (faiss):\n{max_hw_faiss_row}\n\n")
        f.write(f"Configuration with maximum hardware error (scikit):\n{max_hw_scikit_row}\n\n")
        f.write(f"Configuration with maximum software error (scikit):\n{max_sw_scikit_row}\n\n")
        f.write(f"Configuration with maximum faiss_cpu error (scikit):\n{max_faiss_scikit_row}\n\n")

def main():
    # Read results of other implementations from file and compute average time for each number of clusters
    results = "results.csv"
    average_time(results)

    # Read AIE results from file and compute average time for each number of clusters
    aie_files = ["results_aie_1.csv", "results_aie_2.csv"]
    average_aie_time(aie_files)

    # Plot average results
    for cluster in versions:
        concat_files(f"{cluster}_clusters_avg.csv", f"{cluster}_clusters_avg_aie.csv")
        plot_results(f"{cluster}_clusters_avg_combined.csv", cluster)

        os.remove(f"{cluster}_clusters_avg.csv")
        os.remove(f"{cluster}_clusters_avg_aie.csv")
    
    # Read the output file and combine it with the json file
    output_file = "output.txt"
    csv_file = "clusters.csv"
    combine_results(output_file, csv_file)

    # Calculate the difference between the software, hardware and faiss_cpu results
    check_results("combined_results.csv")


if __name__ == "__main__":
    main()