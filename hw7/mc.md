---
title: Monte Carlo
author: Keith A. Lewis
institute: KALX, LLC
classoption: fleqn
fleqn: true
abstract: Integrate using random variates
...

\newcommand\RR{\mathbf{R}}
\newcommand{\Var}{\operatorname{Var}}
\newcommand{\Cov}{\operatorname{Cov}}
\renewcommand{\o}[1]{\overline{#1}}

The _Monte Carlo_ method of evaluating integrals using random variates was
invented by Stan Ulam and Nick Metropolis while working on The Manhattan Project. 
It is based on the facts that if $U$ is uniformly distributed on the interval $[0,1]$
then $E[f(U)] = \int_0^1 f(x)\,dx$
and if $(X_j)$ are independent, identically distributed random variables then
the average $(X_1 + \cdots + X_n)/n$ tends to $E[X]$.

If $F'(x) = f(x)$ then the fundamental theorem of calculus states
$\int_0^1 f(x)\,dx = F(1) - F(0)$, however finding the anti-derivative, $F$,
of $f$ may be difficult. Monte Carlo estimates the integral
by generating uniform $[0,1]$ variates $u_1$, ..., $u_n$ and computing
the averages $(f(u_1) + \cdots + f(u_n))/n$.
Replacing the numerical variates $(u_j)$
by independent uniform random variables $(U_j)$ lets us draw statistical conclusions.
Clearly $E[\sum_1^n f(U_j)/n] = E[f(U)]$.

__Exercise__. _Show $\Var(\sum_1^n f(U_j)/n) = \Var(f(U))/n$_.

_Hint_. If random variables $X$ and $Y$ are independent
then $f(X)$ and $g(Y)$ are independent for any functions $f$ and $g$.

This is called the _weak law of large numbers_ but it reveals an important
general fact: __when trying to estimate a random variable using $n$ samples
the standard deviation is proportional to $1/\sqrt{n}$__.

Monte Carlo methods can be used for any random variable, not just uniform on $[0,1]$.

__Exercise__. _If $X$ has cdf $F$ then $E[g(X)] = E[g(F^{-1}(U))]$ where $U$ is
uniformly distributed on the interval $[0,1]$_.

_Hint_. Show $X$ and $F^{-1}(U)$ have the same law.

# Variance Reduction

Although variance is proportional to $1/n$ there are methods to reduce the constant of proprionality.

## Antithetic Variates

If $X$ and $Y$ have the same law then $E[X] = E[Y]$ so $E[(X + Y)/2] = E[X] = E[Y]$
and $\Var((X + Y)/2) = \Var(X)/4 + \Cov(X,Y)/2 + \Var(Y)/4 = \Var(X)/2 + \Cov(X,Y)/2$.
If $X = Y$ then $\Var((X + Y)/2) = \Var(X) = \Var(Y)$ and if
$X = -Y$ then $\Var((X + Y)/2) = 0$.

__Exercise__. _If $X$ and $-X$ have the same law and $\Cov(f(X),f(-X)) < \Var(f(X))$ then
$\Var((f(X) + f(-X))/2) < \Var(f(X))$_.

The estimate of $E[f(X)]$ can be improved by averaging with the estimate of $E[f(-X)]$
if $\Cov(f(X),f(-X)) < \Var(f(X))$.

### Black Model

The Fischer Black model for the forward price of a stock is $F_t = fe^{σB_t - σ^2t/2}$.
The antithetic variate $F^*_t = fe^{-σB_t - σ^2t/2}$ can be used to reduce variance.

## Control Variate

A _control variate_ for a random variable $X$ is a random variable $Y$
that is close to $X$ that has known mean and variance.

If $X$ and $Y$ are any random variables with non-zero variance then
$E[X] = E[X - c(Y - E[Y])]$ for any $c\in\RR$ and
$\Var(X - c(Y - E[Y])) = \Var(X) - 2c\Cov(X, Y - E[Y]) + c^2\Var(Y - E[Y])$.

__Exercise__. _Show this is minimized when $c = \Cov(X, Y)/\Var(Y)$_.

_Hint_. Take the derivative with respect to $c$ and note $\Var(Y - E[Y]) = \Var(Y) > 0$.


__Exercise__. _Show the miniumum is $\Var(X) - \Cov(X,Y)^2/\Var(Y)$_.


__Exercise__. _If $\Var(X) = \Var(Y) = σ^2$ and $ρ$ is the correlation of $X$ and $Y$ then
$\Var(X) - \Cov(X,Y)^2/\Var(Y) = σ^2(1 - ρ^2)$_.


If $Y$ is close to $X$ then $\Cov(X,Y)$ is positive so $X - c(Y - E[Y])$ has
smaller variance than $X$ and sampling $X - c(Y - E[Y])$ would reduce the variance.
Since $\Cov(X, Y - E[Y]) = \Cov(X, Y)$ and $\Var(Y - E[Y]) = \Var(Y)$ is known
we only need to find $\Cov(X, Y)$. This can be estimated by Monte Carlo sampling
of $X$ and $Y$.

### Asian option

__Exercise__. _Find the mean and variance of $\log (\Pi_j S_{t_j})^{1/n}$_.

_Hint_. $(\Pi_j S_j)^{1/n} = fe^{(1/n)\sum_j σB_{t_j} - σ^2t_j/2}$.

$\Var(\log (\Pi_j S_{t_j})^{1/n}) = (σ^2/n^2)\sum_{i,j} \min\{t_i,t_j\}$.
</details>

The expected value of $\max((\Pi_{j=1}^n S_{t_j})^{1/n} - k, 0\}$ can be computed using the Black-Scholes formula.

__Exercise__. _If $N$ is normal with mean $μ$ and variance $σ^2$ show_
$$
E[\max\{e^N - a,0\}^2]
= e^{2μ + 2σ^2} P(N > \log a - 2σ^2) - 2a e^{μ + σ^2/2} P(N > \log a - σ^2) + a^2P(N > \log a).
$$

_Hint_. $((e^N - a)^+)^2 = (e^{2N} - 2ae^N + a^2)1(e^N > a)$.

