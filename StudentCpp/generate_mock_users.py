import csv
import random
import os

# Provided initial data
initial_data = [
    ["prof1", "prof1234", "DR.Jin", "010-1010-2020", "qwer@mql.com", "Professor", ""],
    ["prof2", "prof456", "Dr. Choi", "010-8888-9999", "choi@prof.com", "Professor", ""],
    ["admin1", "admin123", "Admin Lee", "010-9999-0000", "lee@admin.com", "Administrator", ""],
    ["admin2", "admin456", "Admin Kim", "010-0000-1111", "kim@admin.com", "Administrator", ""],
]

# User mock data generation function
def generate_mock_data(num_users):
    first_names = [
        "Yoo", "Lee", "Park", "Choi", "Jung", "Han", "Oh",
        "Yoon", "Song", "Kang", "Lim", "Seo", "Kwon", "Shin",
        "Kim", "Lee", "Jang", "Yoo", "Bae", "Ha"
    ]
    last_names = [
        "Jain", "JiEun", "SooHyun", "JaeHyun", "MiYoung", "YeJin",
        "MinJoon", "HaNa", "JiWoo", "DongHyun", "SooJin", "JiHo",
        "HyeJin", "YoungJae", "DaBin", "SunMi", "WooJin", "SeoYeon",
        "MinWoo", "JiMin"
    ]
    
    # Year distribution with weights (recent years preferred)
    years = list(range(2017, 2025))
    weights = [0.05, 0.05, 0.1, 0.1, 0.15, 0.15, 0.2, 0.2]

    data = []
    for i in range(1, num_users + 1):
        user_id = f"student{i}"
        password = f"pass{i * 123}"
        name = f"{random.choice(first_names)} {random.choice(last_names)}"
        phone_number = f"010-{random.randint(1000, 9999)}-{random.randint(1000, 9999)}"
        email = f"{user_id}@student.com"
        user_type = "Student"
        
        # Randomly assign a year based on weights
        year = random.choices(years, weights=weights, k=1)[0]
        student_id = f"{year}{str(i).zfill(4)}"
        
        data.append([user_id, password, name, phone_number, email, user_type, student_id])
    return data

# Generate student mock data
student_mock_data = generate_mock_data(150)

# Combine initial and generated data
all_data = initial_data + student_mock_data

# Ensure the "mock" folder exists
mock_folder = "mock"
os.makedirs(mock_folder, exist_ok=True)

# Save data to CSV inside the "mock" folder
csv_file = os.path.join(mock_folder, "mock_users.csv")
with open(csv_file, mode="w", newline="", encoding="utf-8") as file:
    writer = csv.writer(file)
    writer.writerow(["ID", "Password", "Name", "PhoneNumber", "Email", "UserType", "StudentID"])
    writer.writerows(all_data)

print(f"Mock data saved to {csv_file}")
