---
title: Derive quadratic equation solution
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Derive_quadratic_equation_solution
---
$$
    ax^2+bx+c=0\,\!
$$

$$
    ax^2+bx=-c\,\!
$$

Convert $$ax^2+bx$$ to the form $$u^2x^2+2uvx+v^2$$.
Ignore the last term (for now) and let $$v=b$$. If we start with $$u=a$$ we can multiply both sides by $$a$$.

$$
    a^2x^2+abx=-ac\,\!
$$

Not quite the form needed. Still missing a $$2$$ in the second term. We need a number such that $$2d=d^2$$ which is clearly $$d=2$$. This gives $$u=2a$$ which can be achieved by multiplying both sides by $$4$$

$$
    4a^2x^2+4abx=-4ac\,\!
$$

Now add $$b^2$$ to both sides:

$$
    4a^2x^2+4abx + b^2 =b^2-4ac
$$

$$
    (2ax+b)^2                   =b^2-4ac
$$

$$
    2ax+b                         =\pm\sqrt{b^2-4ac}
$$

$$
    2ax                             =-b\pm\sqrt{b^2-4ac}
$$

$$
    x                                 =\frac{-b\pm\sqrt{b^2-4ac}}{2a}
$$
