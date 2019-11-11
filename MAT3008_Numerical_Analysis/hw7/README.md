# Numerical Analysis

## hw7
- Get the eigenvalue and eigenvector using jacobi function on symmetric matrix


### Quick Start
```
cd hw7
gcc -o main main.c -lm
./main
```

### Report
전반적으로 hw6의 내용과 거의 동일하여 보고서에 중복되는 내용이 있습니다. 참고 부탁드립니다.      


##### 코드설명
```
typedef double(*funcPtr)(double, double, int*);
typedef struct distributor{
    funcPtr func;
    char* name;
    double arg1;
    double arg2; 
} distributor;

double uniform_random(double l, double r, int* idum);
double gaussian_random(double  m, double s, int* idum);

distributor distributors[2] = {
    { uniform_random, "Uniform Distribution", -3.0, 2.0 },
    { gaussian_random, "Gaussian Distribution", -0.5, 1.5 }
}; 
```
function ptr 사용하여 uniform random, gaussian random 함수 모두 돌아가게끔 설계했습니다.   
다만 과제명세서에 나온 random function은 gauusian random뿐이기에, 해당 구조체를 사용하지는 않았습니다.

```
double uniform_random(double l, double r, int* idum){
    double p = ran1(idum);
    return l + (r-l)*p;
}
double gaussian_random(double  m, double s, int* idum){
    double n = gasdev(idum);
    return n*s + m;
}
```
gasdev.c 파일의 내용을 참고하여 작성했습니다. 0과 1 사이의 값이 나오는 ran1 함수를 사용하여 원하는 영역 내에서만 값이 나오도록 했습니다.

```
    for(int i=1;i<=n;i++){
        for(int j=i;j<=n;j++){
            matrix[i][j]=matrix[j][i]=gaussian_random(0,1.0,&idum);
        }
    }

    jacobi(matrix, n, eigenvalue, eigenvector, &nrot);
    eigsrt(eigenvalue, eigenvector, n);
```
jacobi function은 symmetric한 행렬에 대해서 유요하기 때문에, symmetric한 행렬을 만들어 적용해보았습니다.

##### Result
![result](./imgs/result.jpg)    

### + hw6과 hw7에서 random으로 수를 뽑는 과정에서, 초기 시드값을 바꾸지 않았습니다. 시드값을 바꿔서 수를 랜덤을 뽑는 것이 과제의 목표가 아니라고 생각했습니다. stl의 rand 함수를 사용하여 나오는 수를 적절히 조절하여 uniform하거나 gaussian distribution에 적합하도록 수 분포를 재배치하는 것이 과제의 목표라고 생각했습니다.
