# run program
docker run -it --rm -v $(pwd):/app final python /app/main.py

# get inside image
# docker run -it --rm -v $(pwd):/app final  /bin/bash
