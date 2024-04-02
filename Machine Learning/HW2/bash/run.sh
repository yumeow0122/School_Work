#sudo docker run -it -v $(pwd):/app project

docker run -p 8888:8888 -v "$(pwd)":/app my_custom_jupyter
