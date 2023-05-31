# run program
docker build -t ds15 .
docker run -it --rm -v $(pwd):/app ds15 python /app/main.py

# get inside image
# docker run -it --rm -v $(pwd):/app ds15  /bin/bash
