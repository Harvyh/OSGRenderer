import numpy as np
import matplotlib.pyplot as plt
from PyRenderer import Renderer

viewport_size_x = 227
viewport_size_y = 227

azimuth = 0
elevation = 0
yaw = 0
distance_ratio = 1
field_of_view = 25
offset_x = 0
offset_y = 0
offset_z = 0

x = Renderer()
x.initialize(['mesh/gray_face.3ds'],
              viewport_size_x, viewport_size_y)

# Render
x.setViewpoint(azimuth, elevation, yaw, distance_ratio, field_of_view, 0, 1e4, 0)
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

x.setViewpoint(azimuth, 30, yaw, 1, field_of_view,3e3,0,0)
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
x.setViewpoint(30, elevation, yaw, distance_ratio, field_of_view,0,3e4,0)
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
x.setViewpoint(0, elevation, 30, distance_ratio, field_of_view,0,0,3e3)
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