/*-----------------------------------------------
課題8 LU分解法

4I31 福嶋　開人
2014/01/22
----------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define KB 1024//文字サイズ
#define ML 1000//用意する配列の大きさ

int main(void) {

	//変数を宣言
	char buf[KB];
	FILE *fp = NULL;
	double **a = NULL, *ap = NULL, x[ML], b[ML], w, p;//a,ap:配列,x:答えの配列(yもこれと同一),b,w:並び替え用,p:ピボット
	int M, mm[ML], l, iw;//M:列数,mm:列の並び替えを記憶，l&iw:mmの入れ替え用


	printf("課題8 LU分解法\n");

	//ファイルをおっぷん
	if ((fopen_s(&fp, "C:\\Users\\kaito\\Desktop\\matdata1000w.txt", "r")) != 0){
		printf("ファイルのオープンに失敗しました．\n");
		return 1;//異常終了
	}

	//行数を読み込み
	fgets(buf, KB, fp);
	M = atoi(buf);

	//列数を読み込み
	fgets(buf, KB, fp);//今回はこの値を使用しない

	//配列を動的確保(1000×1000の正方配列)
	if ((a = (double **)malloc(sizeof(double *)*M)) == NULL){
		printf("メモリの確保に失敗しました.\n");
		return 1;//異常終了
	}
	if ((ap = (double *)malloc(sizeof(double)*M*M)) == NULL){
		printf("メモリの確保に失敗しました.\n");
		return 1;//異常終了
	}

	//ポインタのポインタに関連付け(配列型にする)
	for (int i = 0; i < M; i++)
	{
		a[i] = ap + M * i;
	}

	//配列に値を格納する．
	for (int i = 0; i < M; i++)
	{
		//左辺の正方形
		for (int j = 0; j < M; j++)
		{
			fgets(buf, KB, fp);
			a[i][j] = atof(buf);
		}
		//右辺の一行
		fgets(buf, KB, fp);
		b[i] = atof(buf);
	}

	//ファイルのクローズ
	fclose(fp);

	/*	LU分解法	*/
	for (int i = 0; i <= M - 1; i++) {
		mm[i] = i;
	}
	for (int k = 0; k <= M - 1; k++) {
		/*	ピボット開始	*/
		l = k;
		p = a[k][k];
		for (int j = k + 1; j <= M - 1; j++) {
			if (fabs(p) < fabs(a[j][k])){
				l = j;
				p = a[j][k];
			}
		}
		if (l != k) {
			for (int j = 0; j <= M - 1; j++) {
				w = a[k][j];
				a[k][j] = a[l][j];
				a[l][j] = w;
			}
			iw = mm[k];
			mm[k] = mm[l];
			mm[l] = iw;
		}
		//ピボット終了

		for (int j = k + 1; j <= M - 1; j++) {
			a[k][j] /= p;
		}
		for (int i = k + 1; i <= M - 1; i++) {
			for (int j = k + 1; j <= M - 1; j++) {
				a[i][j] -= a[i][k] * a[k][j];
			}
		}
	}//LU分解終了

	/*	前進消去	*/
	for (int k = 0; k <= M - 1; k++) {
		x[k] = b[mm[k]];
		for (int j = 0; j < k; j++) {
			x[k] -= a[k][j] * x[j];
		}
		x[k] /= a[k][k];
	}//前進消去終了

	/*	後退代入	*/
	for (int k = M - 1; k >= 0; k--) {
		for (int j = k + 1; j <= M - 1; j++) {
			x[k] -= a[k][j] * x[j];
		}
	}//後退代入終了

	/*	解答表示	*/
	printf("ガウスの消去法を適用した答えです．\n");
	for (int i = 0; i <= M - 1; i++){
		printf("x[%d] = %lf\n", i, x[i]);
	}

	//動的確保の解放
	free(ap);
	free(a);

	getch();
	return 0;
}