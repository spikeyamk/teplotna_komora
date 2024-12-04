import json
import matplotlib.pyplot as plt
from collections import deque
import numpy as np

def load_data(): 
    with open('4_min_50_deg_c_heat_pid.json') as file:
        return json.load(file)
    
data = load_data()

time = [item['time'] for item in data]
d_front = [item['d_front'] for item in data]
d_rear = [item['d_rear'] for item in data]

plt.figure()
plt.plot(time, d_front)
plt.plot(time, d_rear)
plt.show()

plt.figure()
plt.plot(time[1:], np.diff(d_front))
plt.plot(time[1:], np.diff(d_rear))
plt.show()

class MedianFilter:
    def __init__(self, window_size = 3):
        self.window_size = window_size
        self.window = deque(maxlen = window_size)

    def filter(self, samples):
        filtered_samples = []
        for sample in samples:
            self.window.append(sample)
            filtered_samples.append(sorted(self.window)[len(self.window) // 2])
        return filtered_samples

def apply_filter(samples, window_size = 30):
    median_filter = MedianFilter(window_size)
    return median_filter.filter(samples)

plt.figure()
plt.plot(time, apply_filter(d_front))
plt.show()