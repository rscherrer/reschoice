#!/bin/bash

## Use this script to profile the code. The program must have 
## been compiled already.

# Path to the bin folder
BIN_DIR="./bin"

# Path to the gprof folder
GPROF_DIR="./gprof"

# Check if the bin directory exists
if [ ! -d "$BIN_DIR" ]; then
    echo "Error: Bin directory '$BIN_DIR' does not exist. Please compile the program first."
    exit 1
fi

# Create the gprof directory if it doesn't exist
mkdir -p "$GPROF_DIR"

# Change to the bin directory
cd "$BIN_DIR"

# Create the parameters.txt file
PARAM_FILE="parameters.txt"
cat > "$PARAM_FILE" <<EOL
tend 10000
tsave 10
verbose 1
savedat 1
popsize 1000
nrounds 10
savepars 1
EOL
echo "Created parameter file: $PARAM_FILE"

# Start the timer
SECONDS=0

# Run the program with the parameter file to generate gmon.out
./reschoice "$PARAM_FILE"

# Stop the timer and print the elapsed time
ELAPSED_TIME=$SECONDS
echo "Program execution time: $ELAPSED_TIME seconds"

# Move gmon.out to the gprof directory
mv gmon.out "../$GPROF_DIR/"

# Generate the profiling report and save it in the gprof directory
gprof reschoice "../$GPROF_DIR/gmon.out" > "../$GPROF_DIR/report.txt"

# Display the profiling report
less "../$GPROF_DIR/report.txt"

# Print a message indicating where the output files are saved
echo "Profiling completed! Output files have been saved to the '$GPROF_DIR' directory."