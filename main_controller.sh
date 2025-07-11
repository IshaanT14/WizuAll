#!/bin/bash

# Check arguments
if [ "$#" -ne 1 ]; then
    echo "Usage: ./automate.sh <data_file.csv>"
    exit 1
fi

DATA_FILE="$1"
WIZ_FILES=("line.wiz" "box.wiz" "bar.wiz" "density.wiz" "ecdf.wiz" "stacked.wiz") # List your wiz files here

# 1. Build project once
echo "Building project..."
make clean
if ! make; then
    echo "Build failed. Exiting."
    exit 1
fi

# 2. Process each wiz file
for wiz in "${WIZ_FILES[@]}"; do
    PY_SCRIPT="${wiz%.wiz}.py"
    WIZ_PATH="vizscripts/$wiz"
    
    echo -e "\nProcessing $wiz..."

    # Compile wiz file into out.c
    if ! ./vizrunner "$WIZ_PATH" "$DATA_FILE"; then
        echo "Error during vizrunner processing of $wiz"
        continue
    fi

    # Compile out.c into executable
    if ! gcc out.c -o visualizeit; then
        echo "Compilation failed for $wiz"
        continue
    fi

    # Run the executable and capture Python code
    ./visualizeit > "$PY_SCRIPT"

    if [ $? -ne 0 ]; then
        echo "Execution failed for $wiz"
        continue
    fi

    # Execute the generated Python script
    if ! python3 "$PY_SCRIPT"; then
        echo "Python script execution failed: $PY_SCRIPT"
        continue
    fi

    echo "Successfully generated and ran $PY_SCRIPT"
done

echo -e "\nAll processing complete."

