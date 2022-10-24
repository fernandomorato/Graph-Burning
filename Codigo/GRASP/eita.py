import numpy as np

def power_iteration(A, num_iterations: int):
    # Ideally choose a random vector
    # To decrease the chance that our vector
    # Is orthogonal to the eigenvector
    b_k = np.random.rand(A.shape[1])
    print(A.shape[1])
    print(b_k)

    for _ in range(num_iterations):
        # calculate the matrix-by-vector product Ab
        b_k1 = np.dot(A, b_k)

        # calculate the norm
        b_k1_norm = np.linalg.norm(b_k1)

        # re normalize the vector
        b_k = b_k1 / b_k1_norm

    return b_k
n, m = map(int, input().split())
arr = np.zeros((n, n))
for i in range(m):
	a, b = map(int, input().split())
	arr[a][b] = arr[b][a] = 1;
for i in range(n):
	arr[i][i] = 1;
print(power_iteration(arr, 1000))