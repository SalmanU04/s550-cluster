import csv
import random

# Config
duration_minutes = 10
interval_ms = 100  # Every 50 ms
total_steps = (duration_minutes * 60 * 1000) // interval_ms

# Output
filename = "drive.csv"

prev_speed = 0  # for smoothing

def simulate_speed_rpm(t):
    global prev_speed
    max_speed = 260  # km/h
    # Acceleration/deceleration
    base_speed = 160 * (1 - abs(((t % 1000) / 500) - 1))
    noise = base_speed * random.uniform(-0.02, 0.02)
    target_speed = max(0, min(max_speed, base_speed + noise))
    #Smoothing
    speed = 0.3 * prev_speed + 0.7 * target_speed
    prev_speed = speed
    # RPM follows speed
    rpm = 800 + speed * 32 + random.uniform(-5, 5)
    return speed, min(rpm, 7000)

def simulate_turn_signals(t):
    left = int((t // 200) % 20 == 0) 
    right = int((t // 200) % 30 == 0)
    return left, right

def simulate_high_beam(t):
    return int((t // 3000) % 2 == 0)

def simulate_fuel(t):
    return max(0, 100 - (t // 3000))

def simulate_temp(t):
    return min(105, 70 + (t // 1000) % 35)

def simulate_warnings(t):
    cel = int((t // 50000) % 2 == 1)
    brake = int((t // 15000) % 10 == 0)
    door = int((t // 17000) % 15 == 0)
    return cel, brake, door

# Write CSV
with open(filename, mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(["Time(ms)", "Speed(kmh)", "RPM", "LeftSignal", "RightSignal","HighBeam", "FuelLevel(%)", "EngineTemp(C)", "CheckEngine", "BrakeWarn", "DoorAjar"])

    for i in range(int(total_steps)):
        t = i * interval_ms

        speed, rpm = simulate_speed_rpm(i)
        left, right = simulate_turn_signals(i)
        high = simulate_high_beam(i)
        fuel = simulate_fuel(i)
        temp = simulate_temp(i)
        cel, brake, door = simulate_warnings(i)

        writer.writerow([t, round(speed, 1), int(rpm), left, right, high,
                         fuel, temp, cel, brake, door])

print(f"CSV file '{filename}' generated with {total_steps} rows.")
