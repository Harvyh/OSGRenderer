import numpy as np
import matplotlib.pyplot as plt
from PyRenderer import Renderer

viewport_size_x = 700
viewport_size_y = 700

azimuth = 0
elevation = 0
yaw = 0
distance_ratio = 1
field_of_view = 25

x = Renderer()
x.initialize(['mesh/2012-VW-beetle-turbo.3ds', 'mesh/Honda-Accord.3ds'],
              viewport_size_x, viewport_size_y)

# Render
x.setViewpoint(azimuth, elevation, yaw, distance_ratio, field_of_view)
rendering, depth = x.render()

# Flip dimension
rendering = rendering.transpose((2,1,0))
depth = depth.transpose((1,0))

# image show
plt.imshow(rendering)
plt.show()

# depth show
plt.imshow(1-depth)
plt.show()

x.setModelIndex(1)
x.setViewpoint(azimuth, elevation, yaw, distance_ratio, field_of_view)
rendering, depth = x.render()

# Flip dimension
rendering = rendering.transpose((2,1,0))
depth = depth.transpose((1,0))

# image show
plt.imshow(rendering)
plt.show()

# depth show
plt.imshow(1-depth)
plt.show()

# For the second time, no need to select the model index
x.setViewpoint(azimuth, elevation, yaw, 10, field_of_view)
rendering, depth = x.render()

# Flip dimension
rendering = rendering.transpose((2,1,0))
depth = depth.transpose((1,0))

# image show
plt.imshow(rendering)
plt.show()

# depth show
plt.imshow(1-depth)
plt.show()


