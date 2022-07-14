#include <stdio.h>
#define N 3

void task1()
{
    int m[N][N];

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            m[i][j] = i * N + j + 1;
            printf("%d ", m[i][j]);
        }
        printf("\n");
    }
}

void task2()
{
    int ar[N];

    printf("Vvod -> ");
    for (int i = 0; i < N; i++)
        scanf("%d", &ar[i]);
    
    printf("Vivod -> ");
    for (int i = N - 1 ; i >= 0; i--)
        printf("%d ", ar[i]);
    printf("\n");
}

void task3()
{
    int m[N][N];

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i + j < N - 1)
                m[i][j] = 0;
            else
                m[i][j] = 1;
            printf("%d ", m[i][j]);
        }
        printf("\n");
    }
}

void task4()
{
    int matrix[N][N];
    int i, k = 0, l = 0;
	int m = N;
	int n = N;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = i * N + j + 1;
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

	while (k < m && l < n) {
		for (i = l; i < n; ++i) {
            printf("%d ", matrix[k][i]);
		}
		k++;

		for (i = k; i < m; ++i) {
            printf("%d ", matrix[i][n - 1]);
		}
		n--;

		if (k < m) {
			for (i = n - 1; i >= l; --i) {
                printf("%d ", matrix[m - 1][i]);
			}
			m--;
		}

		if (l < n) {
			for (i = m - 1; i >= k; --i) {
                printf("%d ", matrix[i][l]);
			}
			l++;
		}
	}
    printf("\n");
}

int main(int argc, char *argv[])
{
    int key;
    
    if (argc == 2)
        key = argv[1][0] - 48;
    else {
        printf("wrong number of arguments\n");
        return 1;
    }

    switch (key)
    {
    case 1:
        task1();
        break;
    
    case 2:
        task2();
        break;

    case 3:
        task3();
        break;

    case 4:
        task4();
        break;

    default:
        printf("key not found\n");
    }

    return 0;
}