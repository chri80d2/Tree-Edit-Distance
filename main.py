import matplotlib.pyplot as plt

def plot_subproblems(
    X: list[int], 
    left_strategy: list[int], 
    shasha_zhang: list[int], 
    klein: list[int], 
    demaine: list[int], 
    title: str
):
    plt.plot(X, left_strategy, marker="D", markevery=5, color='tab:green', label=f'Zhang and Shasha recursive')
    plt.plot(X, shasha_zhang, marker="o", markevery=5, color='tab:red', label=f'Zhang and Shasha iterative')
    plt.plot(X, klein, marker="^", markevery=6, color='tab:blue', label=f'Klein')
    plt.plot(X, demaine, marker="s", markevery=7, color='tab:orange', label=f'Demaine')
    plt.xlabel('n')
    plt.ylabel('relevant sub-problems')
    plt.title(title)
    plt.legend()
    plt.show()

def plot_time(
    X: list[int], 
    left_strategy: list[int], 
    shasha_zhang: list[int], 
    klein: list[int], 
    demaine: list[int], 
    title: str
):
    #plt.plot(X, left_strategy, marker="D", markevery=5, color='tab:green', label=f'Zhang and Shasha recursive')
    plt.plot(X, shasha_zhang, marker="o", markevery=5, color='tab:red', label=f'Zhang and Shasha iterative')
    plt.plot(X, klein, marker="^", markevery=6, color='tab:blue', label=f'Klein')
    plt.plot(X, demaine, marker="s", markevery=7, color='tab:orange', label=f'Demaine')
    plt.xlabel('n')
    plt.ylabel('milliseconds')
    plt.title(title)
    plt.legend()
    plt.show()

def main():
    f = open('input.txt', 'r')
    lines: list[str] = f.readlines()
    title: str = ''
    i: int = 0
    while i < len(lines):
        line = lines[i][0:-1]
        N: int = int(line)-2
        X: list[int] = [i+1+3 for i in range(N)]
        A: list[list[int]] = [[] for _ in range(8)]
        i += 1
        line = lines[i][0:-1]
        title = line
        i += 1
        for _ in range(N):
            line = lines[i][0:-1]
            B = line.split(' ')
            for j in range(8):
                A[j].append(int(B[j]))
            i += 1
        #plot_subproblems(X, A[0], A[1], A[2], A[3], title)
        plot_time(X, A[4], A[5], A[6], A[7], title)

if __name__ == '__main__':
    main()
