# hw7

\newcommand{\Var}{\operatorname{Var}}
\newcommand{\Cov}{\operatorname{Cov}}

__Exercise__. _Find a closed form formula for
$E[\max\{e^M - e^N, 0\}]$ where $M$ and $N$ are jointly normal_.

_Hint_. $E[\max\{e^M - e^N, 0\}] = E[e^M\max\{1 - e^{N - M}\}, 0\}]$.

<details>
<summary>Solution</summary>
$$
\begin{aligned}
E[\max\{e^M - e^N, 0\}] &= E[e^M\max\{1 - e^{N - M}\}, 0\}] \\
&=  E[e^M]E[\max\{1 - e^{N + \Var(N) - (M + \Cov(M,N)}\}, 0\}] \\
&=  E[e^M]E[(1 - e^{N + \Var(N) - (M + \Cov(M,N)})1(1 - e^{N + \Var(N) - (M + \Cov(M,N)}\ge0)] \\
\end{aligned}
$$
</details>

Let $S_t = se^{-\sigma^2t/2 + \sigma B_t}$ for $t\ge0$ where $(B_t)$ is standard Browinian motion.
The value of a call at time $t$ with strike $k$ expiring at $T$ in an arbitrage-free model is
$\bar{V}_t = E_t[\max\{S_T - k, 0\}]$.

Let $\bar{v}(s, k, T) = \bar{V}_0$, where $s = S_0$. Note $\bar{v}$ is a function, not a random variable.

__Exercise__. _Show $\bar{V}_t = S_t \bar{v}(S_t, k, T - t)$_.

Note $\bar{v}(S_t, k, T - t)$ is a random variable.

_Hint_. $\bar{V}_t = E_t[\max\{S_t e^{-\sigma^2(T-t)/2 + \sigma (B_T - B_t)} - k, 0\}]$

<details>
<summary>Solution</summary>
Note $-\sigma^2 T/2 + \sigma B_T
= (-\sigma^2 t/2 + \sigma B_t) - (-\sigma^2 t/2 + \sigma B_t) + (-\sigma^2 T/2 + \sigma B_T)
= (-\sigma^2 t/2 + \sigma B_t) + (-\sigma^2(T - t)/2 + \sigma(B_T - B_t)$.

$$
\begin{aligned}
E_t[\max\{S_t e^{-\sigma^2(T-t)/2 + \sigma (B_T - B_t)} - k, 0\}]
&= E_t[\max\{s e^{-\sigma^2(T-t)/2 + \sigma (B_T - B_t)} - k, 0\}\mid S_t = s] \\
&= E[\max\{s e^{-\sigma^2(T-t)/2 + \sigma (B_T - B_t)} - k, 0\}\mid S_t = s] \\
\end{aligned}
$$
</details>

If a trader puts on a position $(M_t, N_t)$ at time $t$ it has
value $V_t = M_t + N_t S_t$ at $t$ and $V_u = M_t + N_t S_u$ at time $u \ge t$
if the position is constant over $[t, u]$
This is not, in general, the same as the model value $\bar{V}_u$.

__Exercise__. _Find a general formula for $V_u - \bar{V}_u$_. 

<details>
<summary>Solution</summary>
</details>
