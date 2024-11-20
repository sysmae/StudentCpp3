import random
import numpy as np
import csv
import os

# Function to read users (students and professors) from the CSV
def read_users_from_csv(filename):
    students = []
    professors = []
    with open(filename, mode='r', newline='') as file:
        reader = csv.DictReader(file)
        for row in reader:
            if row["UserType"] == "Student":
                students.append(row["StudentID"])
            elif row["UserType"] == "Professor":
                professors.append(row["ID"])  # Collecting professor IDs
    return students, professors

def read_subjects_from_csv(filename):
    subjects = []
    with open(filename, mode='r', newline='') as file:
        reader = csv.DictReader(file)
        for row in reader:
            subject = {
                "subjectID": int(row["ID"]),
                "subjectName": row["Name"],
                "credit": int(row["Credit"]),
                "type": row["Type"],
                "year": row["Year"],  # 문자열로 변경
                "term": row["Term"],
                "professorID": row["ProfessorID"],  # Associate subject with professor ID
            }
            subjects.append(subject)
    return subjects

def get_student_year(student_id):
    return int(student_id[:4])

def generate_mock_data(num_rows, subjects, student_ids, professor_ids):
    grade_to_letter = {
        4.5: 'A+',
        4.0: 'A',
        3.5: 'B+',
        3.0: 'B',
        2.5: 'C+',
        2.0: 'C',
        1.5: 'D+',
        1.0: 'D',
        0.0: 'F',
        "N/A": "N/A"
    }
    mock_data = []
    student_subject_grades = {student_id: {} for student_id in student_ids}
    student_term_credits = {student_id: {} for student_id in student_ids}

    max_attempts = num_rows * 10
    attempts = 0

    while len(mock_data) < num_rows and attempts < max_attempts:
        attempts += 1

        student_id = random.choice(student_ids)
        student_admission_year = get_student_year(student_id)
        available_subjects = [
            subject for subject in subjects
            if (int(subject["year"]) >= student_admission_year and 
                (subject["subjectName"] not in student_subject_grades[student_id] or 
                 student_subject_grades[student_id][subject["subjectName"]]["letterGrade"] == "F"))
        ]
        
        if not available_subjects:
            continue

        subject = random.choice(available_subjects)
        professor_id = subject["professorID"]

        term_key = f"{subject['year']}-{subject['term']}"
        if term_key not in student_term_credits[student_id]:
            student_term_credits[student_id][term_key] = 0

        if student_term_credits[student_id][term_key] + int(subject["credit"]) > 21:
            continue

        # 2024년 2학기 처리 수정 (문자열로 비교)
        if subject["year"] == "2024" and subject["term"] == "SECOND_TERM":
            grade = 0.0
            letter_grade = "N/A"
            score = 0.0
        else:
            # 학점 부여 방식 선택
            case = random.choice([1, 2])
            if case == 1:
                grades = [4.5, 4.0, 3.5, 3.0, 2.5, 2.0, 1.5, 1.0, 0.0]
                weights = [0.175, 0.175, 0.245, 0.245, 0.1, 0.05, 0.03, 0.02, 0.01]
            else:
                grades = [4.5, 4.0, 3.5, 3.0, 2.5, 2.0, 1.5, 1.0, 0.0]
                weights = [0.25, 0.25, 0.2, 0.2, 0.1, 0.07, 0.05, 0.05, 0.03]

            grade = random.choices(grades, weights=weights, k=1)[0]
            letter_grade = grade_to_letter[grade]

            score = int(np.random.normal(85 if grade >= 3.5 else 70 if grade >= 2.0 else 60, 5))
            score = max(0, min(100, score))

        student_subject_grades[student_id][subject["subjectName"]] = {
            "grade": grade,
            "letterGrade": letter_grade
        }
        student_term_credits[student_id][term_key] += int(subject["credit"])

        mock_data.append([student_id, subject["subjectID"], score, grade, letter_grade, professor_id])

        attempts = 0

    if len(mock_data) < num_rows:
        print(f"Warning: Could only generate {len(mock_data)} rows out of {num_rows} requested.")

    return mock_data

def write_to_csv(filename, data):
    with open(filename, mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(["studentID", "subjectID", "score", "grade", "letterGrade", "ProfessorID"])
        writer.writerows(data)

if __name__ == "__main__":
    # Read the data
    student_ids, professor_ids = read_users_from_csv('users.csv')
    subjects = read_subjects_from_csv('subjects.csv')

    # Generate mock data
    mock_data = generate_mock_data(4500, subjects, student_ids, professor_ids)

    # Ensure the "mock" folder exists
    mock_folder = "mock"
    os.makedirs(mock_folder, exist_ok=True)

    # Write to CSV inside the "mock" folder
    csv_file = os.path.join(mock_folder, 'mock_student_records.csv')
    write_to_csv(csv_file, mock_data)

    print(f"Successfully generated {len(mock_data)} rows of mock data")