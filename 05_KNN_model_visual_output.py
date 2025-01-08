import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.neighbors import KNeighborsClassifier
from sklearn.metrics import classification_report, accuracy_score
from scipy.spatial import ConvexHull

# Read combined room data
room_data_file = "room_data.csv"
all_data = pd.read_csv(room_data_file)

# Extract features and labels
features = all_data[["Longitude", "Latitude"]]
labels = all_data["Room"]

# Split data into training and testing sets
X_train, X_test, y_train, y_test = train_test_split(features, labels, test_size=0.2, random_state=42)

# Initialize and train the KNN model
knn_model = KNeighborsClassifier(n_neighbors=5)
knn_model.fit(X_train, y_train)

# Make predictions
y_pred = knn_model.predict(X_test)

# Evaluate the model
accuracy = accuracy_score(y_test, y_pred)
report = classification_report(y_test, y_pred)
print(f"Accuracy: {accuracy:.2f}")
print("Classification Report:\n", report)

# Visualize results with irregular shapes
plt.figure(figsize=(10, 8))
colors = {"Room 0": "orange", "Room 1": "gray", "Room 2": "blue", "Room 3": "purple"}

for room, color in colors.items():
    room_data = all_data[all_data["Room"] == room][["Longitude", "Latitude"]].values
    if len(room_data) > 2:  # ConvexHull requires at least 3 points
        hull = ConvexHull(room_data)
        hull_points = room_data[hull.vertices]
        plt.fill(hull_points[:, 0], hull_points[:, 1], color=color, alpha=0.4, label=room)
    plt.scatter(room_data[:, 0], room_data[:, 1], c=color, edgecolor='k', label=f"{room} points")

plt.title("KNN Results with Irregular Room Boundaries")
plt.xlabel("Longitude")
plt.ylabel("Latitude")
plt.legend()
plt.show()
