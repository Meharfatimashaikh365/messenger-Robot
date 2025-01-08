import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.neighbors import KNeighborsClassifier
from sklearn.metrics import classification_report, accuracy_score, ConfusionMatrixDisplay
from sklearn.preprocessing import LabelEncoder

# Load room data
room_data_file = "room_data.csv"  # Update with the correct path to your dataset
all_data = pd.read_csv(room_data_file)

# Ensure numeric data types and clean dataset
all_data["Longitude"] = pd.to_numeric(all_data["Longitude"], errors='coerce')
all_data["Latitude"] = pd.to_numeric(all_data["Latitude"], errors='coerce')
all_data.dropna(subset=["Longitude", "Latitude", "Room"], inplace=True)

# Encode labels to integers
label_encoder = LabelEncoder()
all_data["Room"] = label_encoder.fit_transform(all_data["Room"])

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
report = classification_report(y_test, y_pred, target_names=label_encoder.classes_)
print(f"Accuracy: {accuracy:.2f}")
print("Classification Report:\n", report)

# Save evaluation metrics to a text file
with open("model_evaluation.txt", "w") as f:
    f.write(f"Accuracy: {accuracy:.2f}\n\n")
    f.write("Classification Report:\n")
    f.write(report)

# Visualize the confusion matrix and save it as an image
disp = ConfusionMatrixDisplay.from_estimator(
    knn_model, X_test, y_test, display_labels=label_encoder.classes_, cmap='Blues', xticks_rotation='vertical'
)
plt.title("Confusion Matrix")
plt.savefig("confusion_matrix.png")
plt.show()

# Plot the data with KNN decision boundaries
x_min, x_max = features["Longitude"].min() - 0.01, features["Longitude"].max() + 0.01
y_min, y_max = features["Latitude"].min() - 0.01, features["Latitude"].max() + 0.01
xx, yy = np.meshgrid(np.arange(x_min, x_max, 0.01), np.arange(y_min, y_max, 0.01))

# Predict labels for each point in the mesh grid
grid_points = pd.DataFrame(np.c_[xx.ravel(), yy.ravel()], columns=["Longitude", "Latitude"])
Z = knn_model.predict(grid_points)
Z = Z.reshape(xx.shape)

# Plot decision boundaries
plt.figure(figsize=(10, 8))
plt.contourf(xx, yy, Z, alpha=0.8, cmap=plt.cm.Paired)

# Plot room data points
for room, color in zip(label_encoder.classes_, ["orange", "gray", "blue", "purple"]):
    room_data = all_data[all_data["Room"] == label_encoder.transform([room])[0]]
    plt.scatter(
        room_data["Longitude"], room_data["Latitude"], c=color, label=room, edgecolor='k', alpha=0.7
    )

plt.title("KNN Decision Boundary and Room Data")
plt.xlabel("Longitude")
plt.ylabel("Latitude")
plt.legend()
plt.savefig("decision_boundary.png")
plt.show()

print("Confusion matrix and decision boundary plot have been saved as images.")
