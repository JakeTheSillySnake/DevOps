.PHONY: all pginx spawn-fcgi server.fcgi art3 part4 img4 run4 part5 img5 run5 test5 part6 docker-compose test clean stop

TARGET3 = $(shell pgrep -f server.fcgi)
TARGET4_R = $(shell sudo docker ps | grep nginx_4)
TARGET5_R = $(shell sudo docker ps | grep nginx_5)
TARGET6_R = $(shell sudo docker ps | grep proxy_6)
TARGET4 = $(shell sudo docker ps -a | grep nginx_4)
TARGET5 = $(shell sudo docker ps -a | grep nginx_5)
TARGET6 = $(shell sudo docker ps -a | grep proxy_6)

all: part3

part3: nginx spawn-fcgi

part4: img4 run4 test

part5: img5 run5 test test5

part6: docker-compose run6 test

nginx: nginx/nginx.conf
	sudo cp nginx/nginx.conf /etc/nginx/nginx.conf

spawn-fcgi: server.fcgi
	sudo service nginx restart
	sudo spawn-fcgi -a 127.0.0.1 -p 8080 ./server/server.fcgi
	curl localhost:81

server.fcgi: ./server/server.c
	gcc ./server/server.c -lfcgi -o ./server/server.fcgi

img4:
	cp ./part4/Dockerfile .
	sudo docker build -t custom:nginx .

run4:
	sudo service nginx stop
	sudo docker run -dit -p 80:81 --name nginx_4 -v ./nginx/nginx.conf:/etc/nginx/nginx.conf custom:nginx
	sudo docker exec nginx_4 ./main.sh

img5:
	cp ./part5/Dockerfile .
	sudo docker build -t custom:nginx .

run5:
	sudo service nginx stop
	sudo docker run -dit -p 80:81 --name nginx_5 -v ./nginx/nginx.conf:/etc/nginx/nginx.conf custom:nginx
	sudo docker exec nginx_5 ./main.sh
	
test5:
	dockle -ak NGINX_GPGKEY -ak NGINX_GPGKEYS -ak NGINX_GPGKEY_PATH custom:nginx

docker-compose:
	cp ./part5/Dockerfile .
	sudo docker-compose build
	sudo docker-compose up -d

run6:
	sudo service nginx stop
	sudo docker exec server_6 ./main.sh

test:
	curl localhost:80
	curl localhost:80/status

stop:
ifeq ($(TARGET3),)
else
	sudo kill -9 $(TARGET3)
endif
ifeq ($(TARGET4_R),)
else
	sudo docker stop nginx_4
endif
ifeq ($(TARGET5_R),)
else
	sudo docker stop nginx_5
endif
ifeq ($(TARGET6_R),)
else
	sudo docker stop server_6
	sudo docker stop proxy_6
endif

clean: stop
ifeq ($(TARGET4),)
else
	sudo docker rm nginx_4
	sudo docker rmi -f custom:nginx
endif
ifeq ($(TARGET5),)
else
	sudo docker rm nginx_5
	sudo docker rmi -f custom:nginx
endif
ifeq ($(TARGET6),)
else
	sudo docker rm proxy_6
	sudo docker rm server_6
	sudo docker rmi -f src_server_6
endif
	rm -rf ./server/*.fcgi
	rm -rf Dockerfile