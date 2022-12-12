## OpenGL

### buffer 类型

- GL_ARRAY_BUFFER（顶点数组缓存，配合VertexArray使用）
- GL_ELEMENT_ARRAY_BUFFER（索引缓存，配合glDrawElements使用）
- GL_TEXTURE_BUFFER（纹理缓存，配合glTexBuffer使用）
- GL_UNIFORM_BUFFER（uniform缓存，传递复杂uniform）
- GL_TRANSFORM_FEEDBACK_BUFFER
- GL_PIXEL_PACK_BUFFER and GL_PIXEL_UNPACK_BUFFER（像素缓存）
- GL_COPY_READ_BUFFER and GL_COPY_WRITE_BUFFER（复制缓存）
- GL_DRAW_INDIRECT_BUFFER

### 正交投影矩阵

#### Game101正交投影矩阵推导(NDC基于右手系)

 ![Image text](https://img-blog.csdnimg.cn/712ecb105b8f44f1bb210f53fb79ca0b.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAbjU=,size_20,color_FFFFFF,t_70,g_se,x_16)

由于正交投影只是把一个长方体变换到一个中心位于原点，坐标范围[-1,1]的立方体，因此通过移动加缩放即可完成变换。

正交投影：

$$
M_{\text {ortho }}=\left[\begin{array}{cccc}
\frac{2}{r-l} & 0 & 0 & 0 \\
0 & \frac{2}{t-b} & 0 & 0 \\
0 & 0 & \frac{2}{n-f} & 0 \\
0 & 0 & 0 & 1
\end{array}\right]\left[\begin{array}{cccc}
1 & 0 & 0 & -\frac{r+l}{2} \\
0 & 1 & 0 & -\frac{t+b}{2} \\
0 & 0 & 1 & -\frac{n+f}{2} \\
0 & 0 & 0 & 1
\end{array}\right]
$$

#### OpenGL正交投影矩阵(NDC基于左手系)

- OpenGL的NDC基于左手系，所以n和f映射是相反的->将n和f取负
- OpenGL中n和f不是基于坐标，而是基于距离值->左乘一个z轴镜像矩阵

$$
M_{\text {glOrtho }}=\left[\begin{array}{cccc}
1 & 0 & 0 & 0 \\
0 & 1 & 0 & 0 \\
0 & 0 & -1 & 0 \\
0 & 0 & 0 & 1
\end{array}\right] *\left[\begin{array}{cccc}
\frac{2}{\mathrm{r}-1} & 0 & 0 & -\frac{\mathrm{r}+1}{\mathrm{r}-1} \\
0 & \frac{2}{\mathrm{t}-\mathrm{b}} & 0 & -\frac{\mathrm{t}+\mathrm{b}}{\mathrm{t}-\mathrm{b}} \\
0 & 0 & \frac{2}{\mathrm{f}-\mathrm{n}} & \frac{\mathrm{f}+\mathrm{n}}{\mathrm{f}-\mathrm{n}} \\
0 & 0 & 0 & 1
\end{array}\right]=\left[\begin{array}{cccc}
\frac{2}{\mathrm{r}-1} & 0 & 0 & -\frac{\mathrm{r}+1}{\mathrm{r}-1} \\
0 & \frac{2}{\mathrm{t}-\mathrm{b}} & 0 & -\frac{\mathrm{t}+\mathrm{b}}{\mathrm{t}-\mathrm{b}} \\
0 & 0 & \frac{-2}{\mathrm{f}-\mathrm{n}} & -\frac{\mathrm{f}+\mathrm{n}}{\mathrm{f}-\mathrm{n}} \\
0 & 0 & 0 & 1
\end{array}\right]
$$

#### 正交投影和宽高比（aspectRatio）

width = r-l,height = t-b, aspectRatio = r-l/t-b

left = - (r-l)/2 = - (r-l)/2 * t-b/r-l * r-l/t-b =  - (r-l)/2 * 2t/r-l * aspectRatio = -2top * aspectRatio
right = (r-l)/2 = (r-l)/2 * t-b/r-l * r-l/t-b =  (r-l)/2 * 2t/r-l * aspectRatio = top * aspectRatio
bottom = -(t-b)/2 = -top
top = (t-b)/2 = top

### 透视投影矩阵

#### Game101透视投影矩阵(NDC基于右手系)

先挤压成一个正交投影那样的长方体视景体，然后对长方体进行正交投影。

$$
\begin{aligned}
\mathrm{M}_{\text {persp }}=\mathrm{M}_{\text {ortho }} \mathrm{M}_{\text {persp->ortho }}= & {\left[\begin{array}{ccccc}
\frac{2}{\mathrm{r}-1} & 0 & 0 & -\frac{\mathrm{r}+1}{\mathrm{r}-1} \\
0 & \frac{2}{\mathrm{t}-\mathrm{b}} & 0 & -\frac{\mathrm{t}+\mathrm{b}}{\mathrm{t}-\mathrm{b}} \\
0 & 0 & \frac{2}{\mathrm{n}-\mathrm{f}} & -\frac{\mathrm{f}+\mathrm{n}}{\mathrm{n}-\mathrm{f}} \\
0 & 0 & 0 & 1
\end{array}\right]\left[\begin{array}{cccc}
\mathrm{n} & 0 & 0 & 0 \\
0 & \mathrm{n} & 0 & 0 \\
0 & 0 & \mathrm{n}+\mathrm{f} & -\mathrm{nf} \\
0 & 0 & 1 & 0
\end{array}\right]=} \\
& {\left[\begin{array}{cccc}
\frac{2 \mathrm{n}}{\mathrm{r}-1} & 0 & -\frac{\mathrm{r}+1}{\mathrm{r}-1} & 0 \\
0 & \frac{2 \mathrm{n}}{\mathrm{t}-\mathrm{b}} & -\frac{\mathrm{t}+\mathrm{b}}{\mathrm{t}-\mathrm{b}} & 0 \\
0 & 0 & \frac{\mathrm{n}+\mathrm{f}}{\mathrm{n}-\mathrm{f}} & -\frac{2 \mathrm{nf}}{\mathrm{n}-\mathrm{f}} \\
0 & 0 & 1 & 0
\end{array}\right] }
\end{aligned}
$$

#### OpenGL透视投影矩阵(NDC基于左手系)

- OpenGL的NDC基于左手系，所以n和f映射是相反的
- OpenGL中n和f不是基于坐标，而是基于距离值
- OpenGL设计出来的矩阵，裁剪空间的$w$坐标值为$-Z_{view}$，GAMES101的$w$值是$Z_{view}$，需要对$w$取反

$$
\begin{aligned}
\mathrm{M}_{\text { glPersp }}= & {
\left[\begin{array}{cccc}
-1 & 0 & 0 & 0 \\
0 & -1 & 0 & 0 \\
0 & 0 & -1 & 0 \\
0 & 0 & 0 & -1
\end{array}\right] *
\left[\begin{array}{cccc}
1 & 0 & 0 & 0 \\
0 & 1 & 0 & 0 \\
0 & 0 & -1 & 0 \\
0 & 0 & 0 & 1
\end{array}\right] *\left[\begin{array}{cccc}
\frac{-2 \mathrm{n}}{\mathrm{r}-1} & 0 & -\frac{\mathrm{r}+1}{\mathrm{r}-1} & 0 \\
0 & \frac{-2 \mathrm{n}}{\mathrm{t}-\mathrm{b}} & -\frac{\mathrm{t}+\mathrm{b}}{\mathrm{t}-\mathrm{b}} & 0 \\
0 & 0 & -\frac{\mathrm{n}+\mathrm{f}}{\mathrm{f}-\mathrm{n}} & -\frac{2 \mathrm{nf}}{\mathrm{f}-\mathrm{n}} \\
0 & 0 & 1 & 0
\end{array}\right]=} \\
& {\left[\begin{array}{cccc}
\frac{2 \mathrm{n}}{\mathrm{r}-1} & 0 & \frac{\mathrm{r}+1}{\mathrm{r}-1} & 0 \\
0 & \frac{2 \mathrm{n}}{\mathrm{t}-\mathrm{b}} & \frac{\mathrm{t}+\mathrm{b}}{\mathrm{t}-\mathrm{b}} & 0 \\
0 & 0 & -\frac{\mathrm{f}+\mathrm{n}}{\mathrm{f}-\mathrm{n}} & -\frac{2 \mathrm{nf}}{\mathrm{f}-\mathrm{n}} \\
0 & 0 & -1 & 0
\end{array}\right] }
\end{aligned}
$$

### 相机视图矩阵

相机位置：Position
相机旋转角度：Rotation
transform = translate(Position) * rotate(Rotation);
ViewMatrix = inverse(transform);
