import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.neighbors import KNeighborsClassifier
from sklearn.metrics import classification_report, accuracy_score

# Read combined room data
room_data_file = "room_data.csv"
all_data = pd.read_csv(room_data_file)

# Extract features and labels
features = all_data[["Longitude", "Latitude"]]
labels = all_data["Room"]

# Split data into training and testing sets
X_train, X_test, y_train, y_test = train_test_split(features, labels, test_size=0.2, random_state=42)

# Initialize and train the KNN model
knn_model = KNeighborsClassifier(n_neighbors=9)
knn_model.fit(X_train, y_train)

# Make predictions
y_pred = knn_model.predict(X_test)

# Evaluate the model
accuracy = accuracy_score(y_test, y_pred)
report = classification_report(y_test, y_pred)
print(f"Accuracy: {accuracy:.2f}")
print("Classification Report:\n", report)

# Plot the data with KNN decision boundaries
colors = {"Room 0": "orange", "Room 1": "gray", "Room 2": "blue", "Room 3": "purple"}
plt.figure(figsize=(10, 8))
for room, color in colors.items():
    room_data = all_data[all_data["Room"] == room]
    plt.scatter(room_data["Longitude"], room_data["Latitude"], c=color, label=room, alpha=0.7, edgecolor='k')

plt.title("Room Data Plot with KNN")
plt.xlabel("Longitude")
plt.ylabel("Latitude")
plt.legend()
plt.show()
