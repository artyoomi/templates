#include <stdio.h>
#include <stdlib.h>

int* twoSum(int* nums, int numsSize, int target, int* returnSize) {
    int* result = (int*)malloc(2 * sizeof(int));
    *returnSize = 2;

    for (int i = 0; i < numsSize; i++) {
        for (int j = 0; j < numsSize; j++) {
            if (i != j && nums[i] + nums[j] == target) {
                result[0] = i;
                result[1] = j;
                break;
            }
        }
    }

    return result;
}

int main()
{
    int returnSize;
    int nums[5] = {4, 6, 0, -1, 3};
    
    int* pair = twoSum(nums, 5, 2, &returnSize);
    
    printf("[%d, %d]\n", pair[0], pair[1]);

    free(pair);
    return 0;
}
