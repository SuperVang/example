
# ref

https://eigen.tuxfamily.org/dox/group__QuickRefPage.html

$$
\begin{aligned}
u_0 & = \cdots = u_p = 0 \\
u_{m-p} & = \cdots = u_{m} = 1 \\
u_{j+p} & = \frac{1}{p}\sum_{i=j}^{j+p-1}\bar{u}_i \quad\quad j=1,\cdots,n-p
\end{aligned}
$$
where $p$ is the degree and $m+1$ the number knots

$$
\begin{aligned}
C(u) & = \sum_{i=0}^{n}N_{i,p}P_i
\end{aligned}
$$


$$
\begin{aligned}
\frac{d^i}{du^i}C(u) & = \sum_{i=0}^{n} \frac{d^i}{du^i} N_{i,p}(u)P_i
\end{aligned}
$$

$$
\begin{aligned} 
q(t) = {cc} a_0+a_1t+a_2t^2 & \quad t_0 \leq t \leq t_b \\
    Vt +c & \quad  t_b < t \leq (tf-t_b) \\
    \bar a_0+ \bar a_1t+ \bar a_2t^2 &  \quad (t_f-t_b) < t \leq t_f 
\end{aligned}
$$