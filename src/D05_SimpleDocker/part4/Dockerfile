FROM nginx:latest
WORKDIR /home/
COPY ./server/. ./server/.
COPY main.sh .
COPY ./nginx/nginx.conf /etc/nginx/nginx.conf
RUN apt update
RUN apt -y upgrade
RUN apt install -y gcc spawn-fcgi libfcgi-dev