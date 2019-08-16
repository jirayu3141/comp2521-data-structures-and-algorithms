#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void *min (void *vp1, void *vp2, int (*fpCmp) (void *, void *) ) {
	int cmpVal = fpCmp (vp1, vp2);
	
	if (cmpVal <= 0) 
		return vp1;
	else 
		return vp2;
}

int stringCompare (void *pt1, void *pt2) {
	char *s1 = (char *)pt1;
	char *s2 = (char *)pt2;
	
	return strcmp(s1, s2);


}

int intCompare (void *pt1, void *pt2) {
	int one = *(int *)pt1;
	int two = *(int *)pt2;
	return one - two;
}


int main(int argc, char *argv[]) {
	int val1 = 55;
	int val2 = 34;  
   
	int *minInt = (int *)min(&val1, &val2, intCompare);
	printf("min int = %d\n", *minInt);
	
	return 0;
}
