"""Example of work with requests, json and pyplot library for data visualisation"""

import requests
import json
import matplotlib.pyplot as plt
import pytz
import datetime as dt
import numpy as np

from matplotlib.widgets import CheckButtons


DOMAIN = ""
PORT = 0
PATH = ""

def utc_from_time_stamp(time_stamp: int, offset_hours: int) -> str:
    """Convert time stamp to UTC+`offset_hours` time"""
    # Create time object
    dt_utc = dt.datetime.utcfromtimestamp(time_stamp)

    # Add temporary UTC time zone
    dt_utc = dt_utc.replace(tzinfo=pytz.utc)
    
    # Convert time to UTC+`offset_hours` time zone
    dt_with_offset = dt_utc + dt.timedelta(hours=offset_hours)

    # return string representation of time in human-readable format
    return dt_with_offset.strftime('%Y-%m-%d %H:%M:%S')

def parse_json(data: json) -> (list, dict, dict):
    """
    Graph structure:
    x: current time
    left  y: from 0  to 40 Celsius degrees
    right y: from 0% to 100%

    Scores:
    T_*    - left y, current temperature
    H_*    - right y, current humidity
    TRGT_* - target temperature for T_*
    PWM_*  - right y
    GRID   - right y
    """

    times = []

    temp_values = dict()
    perc_values = dict()

    # Transform json data to ease of visualisation
    for time_stamp in data:
        # Store all time stamps in times list as UTC+3 time
        times.append(utc_from_time_stamp(int(time_stamp), 3))
       
        for score in data[time_stamp]:
            # Store all temperature values in temp_values dictionary
            if score.startswith("T_") or score.startswith("TRGT_"):
                temp_values[score] = temp_values.get(score, []) + [data[time_stamp][score] / 100]

            # Store all percent values in perc_values dictionary
            elif score.startswith("H_"):
                perc_values[score] = perc_values.get(score, [])  + [data[time_stamp][score] / 100]
            # Store all PWM_* values in percents dictionary
            elif score.startswith("PWM_"):
                perc_values[score] = perc_values.get(score, [])  + [data[time_stamp][score]]
            # Store all GRID values in perc_values dictionary
            elif score == "GRID":
                # Interpret True as 100 and False as 0
                perc_values[score] = perc_values.get(score, []) + [100 if data[time_stamp][score] else 0]

    return times, temp_values, perc_values

def main():
    # # Construct src url
    # url = f"https://{DOMAIN}:{PORT}/{PATH}"
    
    # Get json from constructed url
    # data = requests.post(url=url, headers={"Token": "0"}, verify=False)

    # Mock for a while
    data = ""
    with open("test2.json") as f:
        data = json.load(f)

    # Store main plotting data
    #
    # NOTE:
    # When we will get real data with request, we will get
    # Reponse object, so to get json data json() method
    # need to be executed with given Response object.
    times, temp_values, perc_values = parse_json(data)

    # To use with check boxes
    scores = []
    lines  = []

    # Create plot
    fig, ax1 = plt.subplots()
    fig.canvas.manager.set_window_title("Dacha scores")

    # Resize graph field
    fig.subplots_adjust(left=0.15, right=0.85, top=0.95, bottom=0.05)

    # Setup x axis
    ax1.set_xlabel("Time (current date)")
    ax1.set_xticks([])

    # Setup y (temperature) axis
    temp_color = "red"
    ax1.set_ylabel("Temperature (Celsius)", color=temp_color)
    ax1.tick_params(axis='y', labelcolor=temp_color)
    ax1.set_ylim(-5, 35)

    # Setup humidity y axis
    hum_color = "blue"
    ax2 = ax1.twinx()
    ax2.tick_params(axis='y', labelcolor=hum_color)
    ax2.set_ylabel("Humidity (%)", color=hum_color)
    ax2.set_ylim(-5, 105)
    
    # Plot temperature values
    for score, values in temp_values.items():
        lines.append(ax1.plot(times, values, label=score))
        scores.append(score)
        
    # Plot percent values
    for score, values in perc_values.items():
        lines.append(ax2.plot(times, values, label=score))
        scores.append(score)

    x_min, x_max = ax1.get_xlim()
    y_min, y_max = ax1.get_ylim()

    # Add legend
    fig.legend()

    # Make all lines invisible by default
    for line in lines: line[0].set_visible(False)

    # Add check boxes field
    visibility = [False for _ in scores]
    check_boxes = CheckButtons(plt.axes([0.0, 0.0, 0.1, 0.45]),
                               scores,
                               actives=visibility)

    # Callback function to set visible/unvisible graph lines
    def checkbox_callback(label: str) -> None:
        score_index = scores.index(label)

        visibility[score_index] = not visibility[score_index]

        lines[score_index][0].set_visible(visibility[score_index])

        plt.draw()

    check_boxes.on_clicked(checkbox_callback)

    # Add crosshair
    crosshair_v, = ax1.plot([], [], color="gray", lw=1)
    crosshair_h, = ax2.plot([], [], color="gray", lw=1)

    def crosshair_callback(event) -> None:
        if event.inaxes:
            # Get cursor coordinates
            x_cursor, y_cursor = event.xdata, event.ydata

            # x = np.linspace(-88.3, 1854.3, len(times))
            # y = np.linspace(   -5,     35, len(times))

            # Find nearest point on line of data
            # distances = np.sqrt((x - x_cursor)**2 + (y - y_cursor)**2)
            # min_distance_idx = np.argmin(distances)
            # min_distance = distances[min_distance_idx]

            # print(event)

            # print(f"x_min is {x_min} and x_max is {x_max}")
            # print(f"y_min is {y_min} and y_max is {y_max}")

            crosshair_v.set_data([x_cursor, x_cursor], [y_min, y_max])
            crosshair_h.set_data([x_min, x_max], [y_cursor, y_cursor])
        
            # Redraw graph
            fig.canvas.draw_idle()
        else:
            # If cursor out of graph => hide crosshair
            crosshair_v.set_data([], [])
            crosshair_h.set_data([], [])
            fig.canvas.draw_idle()

    fig.canvas.mpl_connect("motion_notify_event", crosshair_callback)

    plt.show()

if __name__ == "__main__":
    main()
