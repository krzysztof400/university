#!/usr/bin/env python3
import matplotlib.pyplot as plt


def main():
    x = [
        70,
        75,
        80,
        85,
        90,
        95,
        100,
        105,
        110,
        115,
        120,
        125,
        130,
        135,
        140,
        145,
        150,
        155,
        160,
        165,
        170,
        175,
        180,
        185,
        190,
        195,
        200,
        205,
        210,
        215,
        220,
        225,
        230,
        235,
        240,
        245,
        250,
        255,
    ]

    y = [
        2.1239,
        1.2494,
        1.7491,
        1.8731,
        2.3738,
        2.6237,
        2.3738,
        4.4978,
        29.8601,
        29.6102,
        29.6250,
        34.1079,
        40.2299,
        38.2309,
        38.4808,
        41.6042,
        43.1034,
        43.9780,
        45.4773,
        45.4773,
        45.9770,
        48.3508,
        46.6017,
        47.8750,
        52.2239,
        52.3488,
        50.9745,
        54.9725,
        52.3488,
        55.8471,
        55.0699,
        53.9730,
        56.7216,
        57.5962,
        61.0945,
        61.3443,
        70.6250,
        63.4683,
    ]

    # Create and style the plot
    plt.figure(figsize=(9, 5))
    plt.plot(x, y, marker="o", linewidth=2)
    plt.title("Zaleznosc predkosci od Wheels::setSpeed")
    plt.xlabel("Parametr setSpeed")
    plt.ylabel("Predkosc [cm/s]")
    plt.grid(True, linestyle="--", alpha=0.5)
    plt.tight_layout()

    # Save the output
    output_file = "speed_curve.png"
    plt.savefig(output_file, dpi=160)
    print(f"Zapisano wykres do: {output_file}")


if __name__ == "__main__":
    main()
