#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void *Alloc(size_t sz) {
    extraMemoryAllocated += sz;
    size_t *ret = malloc(sizeof(size_t) + sz);
    *ret = sz;
    printf("Extra memory allocated, size: %ld\n", sz);
    return &ret[1];
}

void DeAlloc(void *ptr) {
    size_t *pSz = (size_t *)ptr - 1;
    extraMemoryAllocated -= *pSz;
    printf("Extra memory deallocated, size: %ld\n", *pSz);
    free(pSz);
}

size_t Size(void *ptr) {
    return ((size_t *)ptr)[-1];
}


int parseData(char *inputFileName, int **ppData) {
    FILE *inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    *ppData = NULL;

    if (inFile) {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (int *)Alloc(sizeof(int) * dataSz);
        int i;
        for (i = 0; i < dataSz; i++) {
            fscanf(inFile, "%d", &((*ppData)[i]));
        }
        fclose(inFile);
    }

    return dataSz;
}


void printArray(int pData[], int dataSz) {
    int i, sz = dataSz - 100;
    printf("\tData:\n\t");
    for (i = 0; i < 100; ++i) {
        printf("%d ", pData[i]);
    }
    printf("\n\t");

    for (i = sz; i < dataSz; ++i) {
        printf("%d ", pData[i]);
    }
    printf("\n\n");
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}


void merge(int pData[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));
    extraMemoryAllocated += n1 * sizeof(int) + n2 * sizeof(int);

    for (i = 0; i < n1; i++)
        L[i] = pData[l + i];
    for (j = 0; j < n2; j++)
        R[j] = pData[m + 1 + j];

    i = 0;
    j = 0;
    k = l;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            pData[k] = L[i];
            i++;
        } else {
            pData[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        pData[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        pData[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}


void mergeSort(int pData[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(pData, l, m);
        mergeSort(pData, m + 1, r);
        merge(pData, l, m, r);
    }
}


void insertionSort(int *pData, int n) {
    for (int i = 1; i < n; i++) {
        int key = pData[i];
        int j = i - 1;
        int *temp = (int *)malloc(sizeof(int));
        extraMemoryAllocated += sizeof(int);
        *temp = key;

        while (j >= 0 && pData[j] > *temp) {
            pData[j + 1] = pData[j];
            j--;
        }

        pData[j + 1] = *temp;
        free(temp);
    }
}


void bubbleSort(int *pData, int n) {
    for (int i = 0; i < n - 1; i++) {
        int *flag = (int *)malloc(sizeof(int));
        extraMemoryAllocated += sizeof(int);
        *flag = 0;

        for (int j = 0; j < n - i - 1; j++) {
            if (pData[j] > pData[j + 1]) {
                int temp = pData[j];
                pData[j] = pData[j + 1];
                pData[j + 1] = temp;
                *flag = 1;
            }
        }

        if (*flag == 0)
            break;

        free(flag);
    }
}


void selectionSort(int *pData, int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        int *temp = (int *)malloc(sizeof(int));
        extraMemoryAllocated += sizeof(int);
        *temp = min_idx;

        for (int j = i + 1; j < n; j++) {
            if (pData[j] < pData[*temp])
                *temp = j;
        }

        int t = pData[i];
        pData[i] = pData[*temp];
        pData[*temp] = t;

        free(temp);
    }
}


void heapSort(int pData[], int n) {

    for (int i = n / 2 - 1; i >= 0; i--) {
        int parent = i;
        int child = 2 * parent + 1;

        while (child < n) {
            if (child + 1 < n && pData[child + 1] > pData[child])
                child++;

            if (pData[child] > pData[parent]) {
                swap(&pData[child], &pData[parent]);
                parent = child;
                child = 2 * parent + 1;
            } else {
                break;
            }
        }
    }


    for (int i = n - 1; i > 0; i--) {
        swap(&pData[0], &pData[i]);


        int parent = 0;
        int child = 2 * parent + 1;

        while (child < i) {
            if (child + 1 < i && pData[child + 1] > pData[child])
                child++;

            if (pData[child] > pData[parent]) {
                swap(&pData[child], &
                              pData[parent]);
                                  parent = child;
                                  child = 2 * parent + 1;
                              } else {
                                  break;
                              }
                          }
                      }
                  }
int main(void)
{
  clock_t start, end;
  int i;
    double cpu_time_used;
  char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};

  for (i=0;i<3;++i)
  {
    int *pDataSrc, *pDataCopy;
    int dataSz = parseData(fileNames[i], &pDataSrc);

    if (dataSz <= 0)
      continue;

    pDataCopy = (int *)Alloc(sizeof(int)*dataSz);

    printf("---------------------------\n");
    printf("Dataset Size : %d\n",dataSz);
    printf("---------------------------\n");

    printf("Selection Sort:\n");
    memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
    extraMemoryAllocated = 0;
    start = clock();
    selectionSort(pDataCopy, dataSz);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
    printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
    printArray(pDataCopy, dataSz);

    printf("Insertion Sort:\n");
    memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
    extraMemoryAllocated = 0;
    start = clock();
    insertionSort(pDataCopy, dataSz);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
    printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
    printArray(pDataCopy, dataSz);

    printf("Bubble Sort:\n");
    memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
    extraMemoryAllocated = 0;
    start = clock();
    bubbleSort(pDataCopy, dataSz);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
    printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
    printArray(pDataCopy, dataSz);

    printf("Merge Sort:\n");
    memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
    extraMemoryAllocated = 0;
    start = clock();
    mergeSort(pDataCopy, 0, dataSz - 1);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
    printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
    printArray(pDataCopy, dataSz);

        printf("Heap Sort:\n");
    memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
    extraMemoryAllocated = 0;
    start = clock();
    heapSort(pDataCopy, dataSz);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
    printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
    printArray(pDataCopy, dataSz);

    DeAlloc(pDataCopy);
    DeAlloc(pDataSrc);
  }

  return 0;
}
