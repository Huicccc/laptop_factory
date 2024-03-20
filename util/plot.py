# plot the data

import sys
import matplotlib.pyplot as plt


def plot(x, y, xlabel, ylabel, title, filename):
    plt.xticks(x)
    plt.plot(x, y, marker="o")
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.title(title)
    plt.savefig(filename)


if __name__ == "__main__":
    x = [1, 2, 4, 8, 16, 32, 64, 128, 256]
    y1 = [
        78303.10,
        78072.12,
        77974.45,
        77939.75,
        77895.68,
        77668.25,
        77599.35,
        77532.16,
        77405.98,
    ]
    plot(
        x,
        y1,
        "Number of Customers",
        "Average latency (us)",
        "Experiment 1",
        "experiment1_avg_letency.png",
    )
    plt.clf()

    y2 = [12.76, 25.59, 51.21, 101.64, 204.70, 409.93, 810.01, 1611.70, 3162.22]
    plot(
        x,
        y2,
        "Number of Customers",
        "Throughput (orders/sec)",
        "Experiment 1",
        "experiment1_throughput.png",
    )
    plt.clf()

    y3 = [
        86683.83,
        86924.80,
        87393.56,
        87624.38,
        128949.98,
        256645.75,
        509219.95,
        1016637.92,
        2031682.12,
    ]
    plot(
        x,
        y3,
        "Number of Customers",
        "Average latency (us)",
        "Experiment 2",
        "experiment2_avg_letency.png",
    )
    plt.clf()

    y4 = [11.52, 22.92, 45.37, 89.63, 121.09, 121.60, 122.51, 122.68, 122.72]
    plot(
        x,
        y4,
        "Number of Customers",
        "Throughput (orders/sec)",
        "Experiment 2",
        "experiment2_throughput.png",
    )
    plt.clf()

    y5 = [
        86746.51,
        86661.50,
        86734.66,
        86563.11,
        86414.18,
        86704.96,
        87256.92,
        127571.47,
        252575.05,
    ]
    plot(
        x,
        y5,
        "Number of Customers",
        "Average latency (us)",
        "Experiment 3",
        "experiment3_avg_letency.png",
    )
    plt.clf()

    y6 = [11.52, 23.05, 46.08, 92.29, 183.13, 363.97, 544.13, 971.73, 979.73]
    plot(
        x,
        y6,
        "Number of Customers",
        "Throughput (orders/sec)",
        "Experiment 3",
        "experiment3_throughput.png",
    )
    plt.clf()

    y7 = [
        88856.58,
        88714.32,
        88768.49,
        88648.06,
        88465.17,
        88283.49,
        88255.42,
        87723.41,
        87244.86,
    ]
    plot(
        x,
        y7,
        "Number of Customers",
        "Average latency (us)",
        "Experiment 4",
        "experiment4_avg_letency.png",
    )
    plt.clf()

    y8 = [11.24, 22.53, 45.01, 90.10, 180.62, 360.57, 722.38, 1331.25, 2875.53]
    plot(
        x, y8, "Number of Customers", "Throughput (orders/sec)", "Experiment 4", "experiment4_throughput.png")
    plt.clf()
