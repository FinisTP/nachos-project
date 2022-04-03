#include "syscall.h"

#define MAX_ARRAY_SIZE 256

int main()
{
    // Declare all variables
    int n, i, j, arr[MAX_ARRAY_SIZE], temp, sortAscending, num;

    // Get array size
    PrintString("Please input array size (1 <= n <= 256): ");
    n = ReadNum();
    if (n <= 0 || n > MAX_ARRAY_SIZE) {
        PrintString("Inputted size is invalid.");
        Halt();
    }

    // Input each element in the array
    PrintString("Next, please enter numbers one by one: ");
    for (i = 0; i < n; ++i) {
        num = ReadNum();
        arr[i] = num;
    }

    // Give the user an option to pick order of sort
    PrintString("Sort ascendingly or descendingly? (0: ascending, 1: descending)");
    sortAscending = ReadNum();
    if (sortAscending) {
        // Sort ascendingly
        for (i = 0; i < n - 1; ++i) {
            for (j = 0; j < n - i - 1; ++j) {
                if (arr[j] > arr[j+1]) {
                    temp = arr[j];
                    arr[j] = arr[j+1];
                    arr[j+1] = temp;
                }
            }
        }
    } else {
        // Sort descendingly
        for (i = 0; i < n - 1; ++i) {
            for (j = 0; j < n - i - 1; ++j) {
                if (arr[j] < arr[j+1]) {
                    temp = arr[j];
                    arr[j] = arr[j+1];
                    arr[j+1] = temp;
                }
            }
        }
    }

    // Print the sorted array
    PrintString("Sorted array: ");
    for (i = 0; i < n; ++i) {
        PrintNum(arr[i]);
        PrintChar(' ');
    }
    Halt();
}
