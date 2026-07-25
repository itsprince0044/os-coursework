# ST5004CEM - Operating Systems and Security Coursework

Author: Prince Parajuli
Student ID: [240522]

This repository contains four tasks implemented in C on Ubuntu Linux.

## Requirements
- Ubuntu Linux
- gcc compiler (install with: sudo apt install build-essential)

## Task 1 - Process Management and Threading
    gcc task1/threads.c      -o task1/threads      -lpthread
    gcc task1/mutex_sync.c   -o task1/mutex_sync   -lpthread
    gcc task1/round_robin.c  -o task1/round_robin
    gcc task1/deadlock.c     -o task1/deadlock     -lpthread
Run: ./task1/threads   ./task1/mutex_sync   ./task1/round_robin   ./task1/deadlock good

## Task 2 - Memory Management Simulation
    gcc task2/paging.c -o task2/paging
    gcc task2/fifo.c   -o task2/fifo
    gcc task2/lru.c    -o task2/lru
Run: ./task2/paging 256   ./task2/fifo   ./task2/lru
Test script: chmod +x task2/run_tests.sh && ./task2/run_tests.sh

## Task 3 - File System Operations and Security
    gcc task3/fileops.c     -o task3/fileops
    gcc task3/auth.c        -o task3/auth
    gcc task3/permissions.c -o task3/permissions
    gcc task3/encrypt.c     -o task3/encrypt
    gcc task3/audit.c       -o task3/audit
Run each: ./task3/fileops   ./task3/auth   ./task3/permissions   ./task3/encrypt   ./task3/audit

## Task 4 - Network Programming and IPC
    gcc task4/server.c -o task4/server -lpthread
    gcc task4/client.c -o task4/client
Run: start the server in one terminal (./task4/server), then the client in another (./task4/client)
