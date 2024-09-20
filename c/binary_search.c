#include <stdio.h>
#include <stdlib.h>

int
compar(const void *arg1, const void *arg2)
{
	int a = *((int *)arg1);
	int b = *((int *)arg2);

	if (a == b)
		return 0;
	else if (a > b)
		return 1;
	else
		return -1;
}

// better bsearch for any type array
int
binary_search(const void *key, const void *base0,
              size_t nmemb, size_t size,
              int (*compar)(const void *, const void *))
{
	const char *base = (const char *)base0;

	int lower = 0;
	int upper = nmemb - 1;
	
	int mid;
	int res;

	while (lower <= upper) {
		mid = (lower + upper) / 2;
		res = compar(key, base + mid * size);

		if (0 == res)
			return mid;
		else if (0 < res)
			lower = mid + 1;
		else
			upper = mid - 1;

	}

	return -1;
}

// simple bsearch on int array
int
simple_bsearch(int key, int *arr, size_t nmemb)
{
	int lower = 0;
	int upper = nmemb - 1;
	int mid;

	while (lower <= upper) {
		mid = (upper + lower) / 2;

		if (arr[mid] == key)
			return mid;
		else if (arr[mid] < key)
			lower = mid + 1;
		else
			upper = mid - 1;
	}

	return -1;
}

int
main()
{
	int key;
	scanf("%d", &key);

	int key2;
	scanf("%d", &key2);

	int arr[] = {-1, 0, 3, 5, 9, 12};
	const size_t arr_size = sizeof(arr) / sizeof(*arr);

	int arr2[] = {25, 19, 18, 12, 0, -2};
	const size_t arr2_size = sizeof(arr2) / sizeof(*arr2);
	
	int index = binary_search(&key, arr, arr_size, sizeof(int), compar);
	int index2 = binary_search(&key2, arr2, arr2_size, sizeof(int), compar);

	printf("Index is %d!\n", index);
	printf("Index2 is %d!\n", index2);

	return EXIT_SUCCESS;
}
