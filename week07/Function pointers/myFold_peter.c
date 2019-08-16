#include <stdlib.h>
#include <stdio.h>


int myFold (int a[], int n, int (*fp)(int, int), int startValue) {
    if (n == 0) {
        return startValue;
    } else {
        int currValue = 0;
        currValue = myFold (a, n-1, fp, startValue);
        printf("currValue is : %d\n", currValue);
        int nodeVal = a[n-1];
    }
}

int mySum (int x, int y) {
}


int main (int argc, char *argv[]) {
    int marksA[] = {67, 134, 81, 44, 91};

    int ans = myFold (marksA, 5, mySum, 0);
    printf("mySum: ans is: %d\n", ans);

}