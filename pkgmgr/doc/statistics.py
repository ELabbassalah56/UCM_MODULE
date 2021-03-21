import matplotlib.pyplot as plt
import numpy as np

y = np.array([23, 0, 15, 0, 17, 235])
mylabels = ["Errors", "Warning", "StyleWarning", "PortabilityWarning","Performance Warning","information messages"]
mycolors = ["r", "g", "y", "b", "b","c"]
plt.pie(y, labels = mylabels, colors = mycolors)
plt.legend(title = "Static Analysis:",bbox_to_anchor =(0.75, 1.15), ncol = 2)
plt.show() 