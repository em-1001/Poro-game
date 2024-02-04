# 게임소개 

+ 플레이어가 조작하게 될 캐릭터의 이름은 '포로'입니다.
+ 게임을 시작하면 쿠키 3개가 랜덤하게 위치할 것 입니다. 
+ 플레이어는 포로를 조작하여 쿠키 3개를 모두 먹고     집으로 가야 성공할 수 있습니다
+ 만약 쿠키를 다 먹지 못하고 집으로 가거나 집으로 가는  도중 돌아다니는 각종 몬스터에게 잡히면 실패하게  됩니다. 


#### Calculate the mean of $q$
평균을 구하기 앞서 $q(x_{t-1}|x_t,x_0)$를 베이즈 정리를 이용해 아래와 같이 표현할 수 있다. 

$$q(x_{t-1}|x_t,x_0) = q(x_t|x_{t-1},x_0) \frac{q(x_{t-1}|x_0)}{q(x_t|x_0)}$$

이를 계산하기 위해선 $q(x_t|x_0)$와 $q(x_t|x_{t-1})$각각의 분포를 tractable한 가우시안 형태로 알아야 한다.    
앞서 Diffusion Process에서 보았듯이 $q(x_t|x_{t-1})$를 Reparameterization Trick으로 표현하면 다음과 같다. 

$$q(x_t | x_{t-1}) = \sqrt{1-\beta_t}x_{t-1} + \sqrt{\beta_t} \epsilon_{t-1}$$

$$\alpha_t := 1-\beta_t \ and \ \bar{\alpha} := \prod_{s=1}^t \alpha_s$$

기호를 위와 같이 알파로 재정의하면 $x_t$는 아래와 같이 정의된다. 

$$\begin{aligned}
x_t &= \sqrt{\alpha_t}x_{t-1} + \sqrt{1-\alpha_t}\epsilon_{t-1}　　　　　　　;\text{where} \ \epsilon_{t-1}, \epsilon_{t-2},... \sim \mathcal{N}(0,1) \\   
&= \sqrt{\alpha_t \alpha_{t-1}}x_{t-2} + \sqrt{1-\alpha_t \alpha_ {t-1}}\bar{\epsilon}_ {t-2}　　\ 　;\text{where} \ \bar{\epsilon}_ {t-2} \ \text{merges two Gaussians} \ (*) \\ 
&= \cdots \\ 
&= \sqrt{\bar{\alpha}_t}x_0 + \sqrt{1 - \bar{\alpha}_t}\epsilon
\end{aligned}$$

결과적으로 $q(x_t|x_0)$는 아래와 같이 표현된다.

$$q(x_t|x_0) = \mathcal{N}(x_t; \sqrt{\bar{\alpha}_t}x_0, (1 - \bar{\alpha}_t)I)$$

이제 $q(x_t|x_0)$와 $q(x_t|x_{t-1})$ 분포를 tractable한 가우시안 형태로 아래와 같이 표현할 수 있다. 

$$\begin{aligned}
&q(x_t|x_0) \sim \mathcal{N}(\sqrt{\bar{\alpha}_t}x_0, (1 - \bar{\alpha}_t)I) \\ 
&q(x _t|x _{t-1}) \sim \mathcal{N}(\sqrt{1-\beta_t}x _{t-1}, \beta_tI)
\end{aligned}$$

이를 이용하여 $q(x_{t-1}|x_t,x_0)$를 계산하면 다음과 같다. 

$$\begin{aligned}
q(x_{t-1}|x_t,x_0) &= q(x_t|x_{t-1},x_0) \frac{q(x_{t-1}|x_0)}{q(x_t|x_0)} \\ 
&\propto \exp\left(-\frac{1}{2}\left(\frac{(x_t - \sqrt{\alpha_t}x_{t-1})^2}{\beta_t} + \frac{(x_{t-1} - \sqrt{\bar{\alpha}_ {t-1}}x_0)^2}{1 - \bar{\alpha}_ {t-1}} - \frac{(x_t - \sqrt{\bar{\alpha}_ t}x_{0})^2}{1 - \bar{\alpha}_t}\right) \right) \\ 
&= \exp\left(-\frac{1}{2}\left(\frac{x _t^2 - 2\sqrt{\alpha _t}x _t x _{t-1} + \alpha _t x _{t-1}^2}{\beta _t} + \frac{x _{t-1}^2 - 2\sqrt{\bar{\alpha} _{t-1}}x _0 x _{t-1} + \bar{\alpha} _{t-1} x _0^2}{1 - \bar{\alpha} _{t-1}} - \frac{(x _t - \sqrt{\bar{\alpha} _t}x _{0})^2}{1 - \bar{\alpha} _t} \right) \right) \\ 
&= \exp\left(-\frac{1}{2}\left(\left(\frac{\alpha _t}{\beta _t} + \frac{1}{1 - \bar{\alpha} _{t-1}}\right)x _{t-1}^2 - \left(\frac{2\sqrt{\alpha _t}}{\beta _t}x _t + \frac{2\sqrt{\bar{\alpha} _{t-1}}}{1 - \bar{\alpha} _{t-1}}x _0 \right)x _{t-1} + C(x _t, x _0) \right) \right)
\end{aligned}$$

2번째 줄의 비례식은 가우시안 분포의 확률 밀도 함수(probability density function)에 의해 성립한다. 

$$f(x) = \frac{1}{\sigma \sqrt{2\pi}} e^{-\frac{1}{2}\left(\frac{x-\mu}{\sigma}\right)^2} \propto e^{-\frac{1}{2}\left(\frac{x-\mu}{\sigma}\right)^2}$$

4번째 줄의 $C(x _t, x _0)$는 $x _{t-1}$과 관련이 없기 때문에 상수처리된다.

#### Calculate the mean of $p_{\theta}$
