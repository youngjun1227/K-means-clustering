#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define INPUT_FILE_NAME "test_set_1.txt"
#define OUTPUT_FILE_NAME "test_set_1_sol.txt"

struct save_point* sp;

struct save_point {
	int data_number;
	int cluster;
	double* data;
};

struct Cluster* c;

struct Cluster {
	int data_number;
	double* data;
};

int* check;

// choice함수 = data number를 겹치지 않도록 뽑는 함수
int choice(int d, int n, int k) {
	int flag = 0;
	int q = rand() % n;
	for (int i = 0; i < k; i++) {
		if (q == check[i]) {
			flag = -1;
		}
	}
	if (flag != -1) {
		return q;
	}
	else {
		return -1;
	}
}

// data에서 랜덤하게 하나를 뽑는 함수
double* put(int d,int q) {
	double* pick = (double*)malloc(sizeof(double) * d);
	for (int i = 0; i < d; i++) {
		pick[i] = sp[q].data[i];
	}
	return pick;
}

// data들을 cluster를 기준으로 분류하는 작업
int Data_classification(int number, int d, int k) {

	double target = 435600000000;
	int n = 0;
	int min_n = 0;
	for (int i = 0; i < k; i++) {
		double result = 0;
		for (int j = 0; j < d; j++) {
			result += (c[i].data[j] - sp[number].data[j]) * (c[i].data[j] - sp[number].data[j]);
			n = i + 1;
		}
		result = sqrt(result);
		printf("%lf ", result);
		if (result < target) {
			target = result;
			min_n = n;
		}
	}
	return min_n;
}

void move(int number, int n, int d) {
	double* test = (double*)malloc(sizeof(double) * d);
	double total;
	int count;
	for (int i = 0; i < d; i++) {
		count = 0;
		total = 0;
		for (int j = 0; j < n; j++) {
			if (sp[j].cluster == c[number].data_number) {
				total += sp[j].data[i];
				count++;
			}
		}
		if (count != 0) {
			total = total / count;
			test[i] = total;
		}
	}
	if (count != 0) {
		//c[number].data = test;
		for (int i = 0; i < d; i++) {
			c[number].data[i] = test[i];
		}
	}
	count = 0;
}

double** end_matrix;

void save(int d, int k) {
	for (int i = 0; i < k; i++) {
		for (int j = 0; j < d; j++) {
			end_matrix[i][j] = c[i].data[j];
		}
	}
}

int tf(int d, int k) {
	int testcode = 0;
	int compare = d * k;
	for (int i = 0; i < k; i++) {
		for (int j = 0; j < d; j++) {
			if (end_matrix[i][j] == c[i].data[j]) {
				testcode += 1;
			}
		}
	}
	if (testcode == compare) {
		return 1;
	}
	else {
		return 0;
	}
}


int main() {

	FILE* infp;
	FILE* outfp;

	srand((unsigned int)time(NULL));

	int read = 0;
	int k = 0;
	int d = 0;
	int n = 0;

	infp = fopen(INPUT_FILE_NAME, "r");
	outfp = fopen(OUTPUT_FILE_NAME, "w");

	read = fscanf(infp, "%d %d %d\n", &k, &d, &n); // 기본 첫줄 데이터 읽어오기
	// matrix = 각 데이터별 좌표의 위치를 저장하는 역할
	double** matrix = (double**)malloc(sizeof(double*) * n);
	for (int i = 0; i < n; i++) {
		matrix[i] = (double*)malloc(sizeof(double) * d);
	}
	double input;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < d; j++) {
			double test = fscanf(infp, "%lf", &input);
			matrix[i][j] = input;
		}
	}
	// 파일 닫기
	fclose(infp);
	// data의 각 수치를 저장할 구조체 선언
	sp = (struct save_point*)malloc(sizeof(struct save_point) * n);

	for (int i = 0; i < n; i++) {
		double* m = (double*)malloc(sizeof(double) * d);
		for (int j = 0; j < d; j++) {
			m[j] = matrix[i][j];
		}
		sp[i].data = m;
		sp[i].data_number = i + 1;
	}
	/****************************************************/
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < d; j++) {
			printf("%lf ", sp[i].data[j]);
		}
		printf("\n");
	}
	printf("\n");
	/****************************************************/

	// cluster 만들기
	c = (struct Cluster*)malloc(sizeof(struct Cluster) * k);
	// cluster에 몇번 데이터를 넣어주었는지 기록하는 배열
	check = (int*)malloc(sizeof(int) * k);
	// cluster에 데이터 넣기
	for (int i = 0; i < k; i++) {
		int as = -1;
		while (as == -1) {
			as = choice(d, n, k);
		}
		check[i] = as;
		c[i].data_number = i + 1;
		c[i].data = put(d, as);
	}
	// cluster data를 저장할 공간 동적할당
	end_matrix = (double**)malloc(sizeof(double*) * k);
	for (int i = 0; i < k; i++) {
		end_matrix[i] = (double*)malloc(sizeof(double) * d);
	}
	int judge = 0;
	int title = 1;
	/***************************************************/
	for (int i = 0; i < k; i++) {
		printf("%d ", check[i]);
	}
	printf("\n");
	/***************************************************/
	while (judge == 0) {
		// cluster compared data 저장하기
		save(d, k);
		// data 분류하기
		for (int i = 0; i < n; i++) {
			int test = Data_classification(i, d, k);
			printf("\n");
			sp[i].cluster = test;
		}
		/***************************************************/
		for (int i = 0; i < k; i++) {
			printf("cluster : %d ", i+1);
			for (int j = 0; j < d; j++) {
				printf("%lf ", c[i].data[j]);
			}
			printf("\n");
		}
		printf("\n");
		/***************************************************/
		// data 분류 출력
		fprintf(outfp,"%d\n", title);
		for (int i = 0; i < k; i++) {
			for (int j = 0; j < n; j++) {
				if (sp[j].cluster == c[i].data_number) {
					fprintf(outfp ,"%d ", sp[j].data_number);
				}
			}
			fprintf(outfp,"\n");
		}
		// cluster 위치 이동하기
		for (int i = 0; i < k; i++) {
			move(i, n, d);
		}
		title++;
		judge = tf(d, k);
	}

	for (int i = 0; i < d; i++) {
		free(matrix[i]);
	}
	free(matrix);
	for (int i = 0; i < k; i++) {
		free(sp[i].data);
	}
	for (int i = 0; i < k; i++) {
		free(end_matrix[i]);
	}
	free(end_matrix);
}