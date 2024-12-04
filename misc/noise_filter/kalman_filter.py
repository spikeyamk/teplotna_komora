import json
import matplotlib.pyplot as plt
import numpy as np

def load_data(): 
    with open('4_min_50_deg_c_heat_pid.json') as file:
        return json.load(file)
    
data = load_data()

time = [item['time'] for item in data]
d_front = [item['d_front'] for item in data]
d_rear = [item['d_rear'] for item in data]

plt.figure()
plt.plot(time, d_front, label="d_front")
plt.plot(time, d_rear, label="d_rear")
plt.legend()
plt.show()

plt.figure()
plt.plot(time[1:], np.diff(d_front), label="diff(d_front)")
plt.plot(time[1:], np.diff(d_rear), label="diff(d_rear)")
plt.legend()
plt.show()

class KalmanFilter:
    def __init__(self, process_variance, measurement_variance, initial_estimate=0, initial_error=1):
        self.process_variance = process_variance  # Q: Process noise covariance
        self.measurement_variance = measurement_variance  # R: Measurement noise covariance
        self.estimate = initial_estimate  # Initial estimate
        self.error = initial_error  # Initial estimation error
    
    def filter(self, samples):
        filtered_samples = []
        for sample in samples:
            # Prediction Step
            self.error += self.process_variance
            
            # Update Step
            kalman_gain = self.error / (self.error + self.measurement_variance)
            self.estimate += kalman_gain * (sample - self.estimate)
            self.error *= (1 - kalman_gain)
            
            filtered_samples.append(self.estimate)
        return filtered_samples

def apply_filter(samples, process_variance = 1e-2, measurement_variance = 1e-1):
    kalman_filter = KalmanFilter(process_variance, measurement_variance)
    return kalman_filter.filter(samples)

filtered_d_front = apply_filter(d_front, process_variance = 1e-2, measurement_variance=1)

plt.figure()
plt.plot(time, d_front, label="Original d_front", alpha = 0.5)
plt.plot(time, filtered_d_front, label="Filtered d_front (Kalman)", linewidth=2)
plt.legend()
plt.show()