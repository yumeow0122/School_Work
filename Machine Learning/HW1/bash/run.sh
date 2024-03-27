#sudo docker run -it -v $(pwd):/app project
sudo docker run -it -v "$(pwd)":/app project python main.py
