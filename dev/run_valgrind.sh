#!/bin/bash

## Use this script to perform memory check. Will only work if the tests have been
## compiled.

# Ensure the script exits on errors
set -e

# Path to the tests folder
TESTS_DIR="./bin/tests"

# Path to the Valgrind logs folder
LOGS_DIR="./valgrind"

# Root directory
ROOT_DIR=$(pwd)

# Check if the tests directory exists
if [ ! -d "$TESTS_DIR" ]; then
    echo "Error: Tests directory '$TESTS_DIR' does not exist."
    exit 1
fi

# Create the Valgrind logs directory if it doesn't exist
mkdir -p "$TESTS_DIR/$LOGS_DIR"

# Change to the tests directory
cd "$TESTS_DIR"

# Loop through all executables in the tests directory
for TEST_EXECUTABLE in *; do
    if [ -x "$TEST_EXECUTABLE" ]; then

        # Verbose
        echo "Running Valgrind on $TEST_EXECUTABLE..."
        LOG_FILE="$LOGS_DIR/${TEST_EXECUTABLE}.valgrind.log"

        # Perform memory check
        valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
            --log-file="$LOG_FILE" ./"$TEST_EXECUTABLE"

        # Verbose
        echo "Valgrind log saved to $LOG_FILE"

        # Extract and print the ERROR SUMMARY line
        SUMMARY_LINE=$(tail -n 1 "$LOG_FILE")
        echo "$SUMMARY_LINE"

    else
        echo "Skipping $TEST_EXECUTABLE (not executable)"
    fi
done

# Migrate log to top level
mv "$TESTS_DIR/$LOGS_DIR" "$ROOT_DIR"

echo "All tests completed! Valgrind logs are in the '$LOGS_DIR' directory."