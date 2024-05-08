#!/bin/bash

# Define the folder containing the .txt files
folder="Interchange/txt"

# Define the output file
output_file="merged.txt"

# Loop through each .txt file in the folder
for file in "$folder"/*.txt; do
    # Check if the file is a regular file
    if [ -f "$file" ]; then
        # Append the file contents to the output file and add a line feed before EOF
        cat "$file" >> "$output_file"
        echo "" >> "$output_file"  # Add a line feed
    fi
done

echo "Merging completed."
