# 基於Jupyter Notebook Docker镜像
FROM jupyter/scipy-notebook:latest

# 将requirements.txt复制到容器中
COPY requirement.txt .

# 切换到/tmp目录并安装所有依赖项
RUN pip install --no-cache-dir -r requirement.txt

# 设置工作目录
WORKDIR /app