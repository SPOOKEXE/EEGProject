

import random
import pandas as pd
import matplotlib.pyplot as plt

from itertools import count
from matplotlib.animation import FuncAnimation

plt.style.use('fivethirtyeight')

MAX_ITEMS = 10

x_vals = [ x - MAX_ITEMS for x in list(range(MAX_ITEMS-1)) ]
y_vals_1 = [0] * (MAX_ITEMS-1)
y_vals_2 = [0] * (MAX_ITEMS-1)

def animate(i):

	# remove first values until list length is MAX_ITEMS
	while len(x_vals) > MAX_ITEMS:
		x_vals.pop(0)
		y_vals_1.pop(0)
		y_vals_2.pop(0)

	# append a new value on the end (read from file)
	x_vals.append(i)
	y_vals_1.append( random.randint(-1000, 1000)/1000 )
	y_vals_2.append( random.randint(-1000, 1000)/1000 )

	# plot data
	plt.cla()

	plt.plot(x_vals, y_vals_1, label='DP-1', linewidth=2)
	plt.plot(x_vals, y_vals_2, label='DP-2', linewidth=2)

	plt.ylim( -1.5, 1.5 )
	plt.xlabel("Timestamp")
	plt.ylabel("Delta Voltage")
	plt.legend(loc='upper left')
	plt.tight_layout()

ani = FuncAnimation(plt.gcf(), animate, interval=1000, cache_frame_data=False)

plt.tight_layout()
plt.xlabel("Timestamp")
plt.ylabel("Delta Voltage")
plt.ylim( -1.5, 1.5 )
plt.show()
