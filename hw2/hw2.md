\newcommand{\Var}{\operatorname{Var}}
\newcommand{\erf}{\operatorname{erf}}

## Due 2pm on 9/15/2022

Black-Scholes/Merton European option pricing.

You have probably already implemented this in an earlier course but this homework
will provide you with a new angle that can be generalized to any model of stock price.

__Exercise 1__. _Show $\int_{-\infty}^\infty e^{-\pi x^2}\,dx = 1$_.

_Hint_: Let $I = \int_{-\infty}^\infty e^{-\pi x^2}\,dx$ so
$I^2 = \int_{-\infty}^\infty\int_{-\infty}^\infty e^{-\pi x^2}e^{-\pi y^2}\,dx\,dy$.
Change to polar coordinates $x = r\cos\theta$, $y = r\sin\theta$ and show
$I^2 = \int_0^{2\pi}\int_0^\infty e^{-\pi r^2}\,r\,dr\,d\theta$.
Note $de^{-\pi r^2} = -2\pi r e^{-\pi r^2}\,dr$
so $\int re^{-\pi r^2/2} = -e^{-\pi r^2}/2\pi$.

__Exercise 2__. _Show $\int_{-\infty}^\infty e^{-\alpha x^2}\,dx = \sqrt{\pi/\alpha}$, $\alpha > 0$_.

The standard normal density function is $\phi(x) = e^{-x^2/2}/\sqrt{2\pi} > 0$, $-\infty < x < \infty$.
The above shows $\int_{-\infty}^\infty \phi(x)\,dx = 1$ so $\phi$ is a probability density.
The cumulative distribution function is $\Phi(x) = \int_{-\infty}^x f(u)\,du$.
Let $Z$ be the random variable defined by this so $P(Z\le z) = \Phi(z)$ and

This function is not part of the standard C library, but `erf` is.
$$
	\erf(x) = \frac{2}{\sqrt{\pi}} \int_0^x e^{-t^2}\,dt
$$

__Exercise__ 3. _Show $\Phi(z) = (1 + \erf(z/\sqrt{2}))/2$_.

For any function $f$, $E[f(Z)] = \int_{-\infty}^\infty f(z) e^{-z^2/2}\,dz/\sqrt{2\pi}$.

To compute all moments $\mu_n = E[Z^n]$ we can use the formula for the _moment generating function_
$E[e^{sZ}] = \sum_{n=0}^\infty E[Z^n]s^n/n! = \sum_{n=0}^\infty \mu_n s^n/n!$.

__Exercise 4__. _Show $E[e^{sZ}] = e^{s^2/2}$_.

_Hint_: $sz - z^2/2 = s^2/2 - (z - s)^2/2$.

We have $(s^2/2)^n/n! = \mu_{2n}s^{2n}/(2n)!$ so $\mu_{2n} = (2n)!/2^n n!$.
All odd moments are 0. The fourth moment is $\mu_4 = 4!/2^2 2! = 24/8 = 3$.

__Exercise 5__. _Show $E[e^{sZ} f(Z)] = E[e^{sZ}] E[f(Z + s)]$_.

_Hint_: $sz - z^2/2 = s^2/2 - (z - s)^2/2$.

In the Fischer Black model the forward value of a stock at time $t$
is
$$
	F_t = fe^{-\sigma^2t/2 + \sigma B_t}
$$
where $(B_t)_{t\ge0}$ is
standard Brownian motion. Recall $E[B_t] = 0$ and $\Var(B_t) = t$,
$t\ge0$.  The forward value of a put with strike $k$ and expiration $t$
is $E[\max\{k - F_t, 0\}] = E[(k - F_t)^+]$.  
Since 
$$
	F = f^{-s^2/2 + sZ}
$$
where $s = \sigma\sqrt{t}$ and $Z$ is standard normal, has the same distribution as $F_t$ we need
to compute $E[(k - F)^+]$.

__Exercise 6.__ _Show $E[(k - F)^+] = kP(F\le k) - fP(Fe^{s^2}\le k)$_.

_Hint_: Use $\max\{x,0\} = x^+ = x1(x \ge 0)$ where $1(x\ge0) = 1$ if $x\ge0$ and $1(x\ge0) = 0$ if $x < 0$.
You will also need $E[e^{sZ} f(Z)] = E[e^{sZ}] E[f(Z + s)]$.

__Exercise 7.__ _Show $F\le k$ if and only if $Z\le (\log k/f + s^2/2)/s$
and $Fe^{s^2}\le k$ if and only if $Z + s \le (\log k/f + s^2/2)/s$_.

__Exercise 8.__ _Follow the directions in this [video](https://urldefense.com/v3/__https://nyu.zoom.us/rec/share/sOWDzECOg9NTA7vWe1-HHUn4pfKVaQDLHqSV_kzp-ahGF6szMYZyg5ZV8Fz4ke2q.XUuDLn6WR-J5fbse__;!!BhJSzQqDqA!UwblKz5iwFI0FozKCe7Bf8FDQSaoSVKHCAYmQco5BUdpgCC0GarUovQ8KiqmzHuwcVcTSGaGuOiIvWIt$)_.