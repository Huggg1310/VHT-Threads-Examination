import pandas as pd
import matplotlib.pyplot as plt
import os

script_dir = os.path.dirname(os.path.abspath(__file__))
log_file = os.path.join(script_dir, '../logs/time_and_interval.txt')
output_img = os.path.join(script_dir, '../logs/interval_evaluation.png')

if not os.path.exists(log_file):
    print(f"Không tìm thấy file {log_file}")
    exit()

df = pd.read_csv(log_file, sep=' ', header=None, names=['T', 'Interval', 'X'])
df = df.iloc[1:] 

x_values = [1000000, 100000, 10000, 1000, 100]

plt.figure(figsize=(15, 10))

for i, x_val in enumerate(x_values):
    subset = df[df['X'] == x_val]
    
    plt.subplot(3, 2, i + 1)
    plt.plot(range(len(subset)), subset['Interval'], marker='o', linestyle='-', markersize=2, alpha=0.5)
    plt.axhline(y=x_val, color='r', linestyle='--', label=f'Ideal X = {x_val} ns')
    
    plt.title(f'Khảo sát Interval với X = {x_val} ns')
    plt.xlabel('Sample Index')
    plt.ylabel('Interval (ns)')
    plt.legend()
    plt.grid(True)

plt.tight_layout()
plt.savefig(output_img)
print(f"Đã vẽ đồ thị và lưu tại: {output_img}")
