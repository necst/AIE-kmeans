import json

def json_to_txt(json_filename, txt_filename):
    try:
        # Read the JSON file
        with open(json_filename, 'r') as json_file:
            data = json.load(json_file)

        # Open the text file for writing
        with open(txt_filename, 'w') as txt_file:
            for point in data:
                if len(point) == 2:  # Ensure the point has exactly two values
                    txt_file.write(f"{point[0]} {point[1]}\n")
                else:
                    print(f"Warning: Skipping invalid point {point}")

        print(f"Transformed {json_filename} to {txt_filename} successfully.")
    
    except FileNotFoundError:
        print(f"Error: {json_filename} not found.")
    except json.JSONDecodeError:
        print(f"Error: Failed to parse {json_filename} as JSON.")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")

# Example usage
json_to_txt("points.json", "points.txt")
