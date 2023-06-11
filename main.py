import turtle

class Tree:
    def __init__(self, n):
        self.n = n
        self.adj = [[] for _ in range(n)]
        self.label = [i for i in range(n)]
        self.A = [[] for _ in range(n)]
        self.pos = [(0, 0) for _ in range(n)]
    
    def add_edge(self, u, v):
        self.adj[u].append(v)
    
    def set_label(self, node, label):
        self.label[node] = label

    def dfs(self, node, parent, depth):
        self.A[depth].append(node)
        for nei in self.adj[node]:
            if nei == parent:
                continue
            self.dfs(nei, node, depth+1)
    
    def draw_node(self, pen, node, radius):
        pen.penup()
        pen.goto(self.pos[node])
        pen.pendown()
        pen.fillcolor('light grey')
        pen.begin_fill()
        pen.circle(radius)
        pen.end_fill()
        pen.penup()
        pen.goto(self.pos[node][0]-int(radius/8), self.pos[node][1]+radius-int(radius/3))
        pen.write(self.label[node], font=("Comic Sans", int(radius/2), "normal"))
    
    def draw_edge(self, pen, node, nei, radius):
        pen.penup()
        pen.goto(self.pos[node][0], self.pos[node][1]+radius)
        pen.pendown()
        pen.goto(self.pos[nei][0], self.pos[nei][1]+radius)

    def draw(self, pen, width, height, xoffset):
        self.dfs(0, -1, 0)
        m = 0
        mx = 0
        for i in range(len(self.A)):
            if self.A[i] != []:
                m = i+1
                mx = max(mx, len(self.A[i]))
        radius = min(height/m/4, width/mx/4)
        for i in range(m):
            k = len(self.A[i])
            for j in range(k):
                self.pos[self.A[i][j]] = (
                    width/(k+1)*(j+1)-width/2+xoffset, 
                    height-height/(m+1)*(i+1)-radius-height/2
                )
        for node in range(self.n):
            for nei in self.adj[node]:
                self.draw_edge(pen, node, nei, radius)
        for node in range(self.n):
            self.draw_node(pen, node, radius)

def input_tree():
    n = int(input())
    tree = Tree(n)
    for _ in range(2*(n-1)):
        s = input().split(' ')
        u = int(s[0])
        v = int(s[1])
        tree.add_edge(u, v)
    s = input().split(' ')
    for node in range(len(s)):
        label = int(s[node])
        tree.set_label(node, label)
    return tree

def main():
    width = turtle.window_width()
    height = turtle.window_height()
    pen = turtle.Turtle()
    pen.speed(0)
    x = input_tree()
    x.draw(pen, width/2, height, -width/4)
    y = input_tree()
    y.draw(pen, width/2, height, width/4)
    pen.hideturtle()
    turtle.done()

if __name__ == "__main__":
    main()
