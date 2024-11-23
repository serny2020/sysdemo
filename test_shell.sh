#!/bin/bash

# Start Shell (assuming it's built as ./shell)
echo "Starting Shell tests..."

# Test 1: Running a simple command (echo)
echo "Test 1: Simple echo command"
echo "echo hi" | ./shell
echo "----------------------------------------"

# Test 2: Running a simple command with arguments (ls)
echo "Test 2: Running ls"
echo "ls" | ./shell
echo "----------------------------------------"

# Test 3: Running wc on a file (test with a sample text file, e.g., hamlet.txt)
echo "Test 3: wc command on a file (hamlet.txt)"
echo "wc hamlet.txt" | ./shell
echo "----------------------------------------"

# Test 4: Running a command that does not exist (should show an error)
echo "Test 4: Running a non-existent command"
echo "foo" | ./shell
echo "----------------------------------------"

# Test 5: Running command with a pipe (ls | wc)
echo "Test 5: Pipe with ls and wc"
echo "ls | wc" | ./shell
echo "----------------------------------------"

# Test 6: Running multiple commands in a pipeline (ls -l | grep test | wc)
echo "Test 6: Multiple commands in a pipeline"
echo "cat hamlet.txt | grep the | wc -l" | ./shell
echo "----------------------------------------"

# Test 7: Running a simple calculator command using bc
echo "Test 8: 1 + 3"
echo "echo 1 + 3 | bc" | ./shell
echo "----------------------------------------"

# Test 8: Running exit command
echo "Test 7: Exit command"
echo "exit" | ./shell
echo "----------------------------------------"

echo "Shell tests complete."
