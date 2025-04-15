FROM nginx:latest
RUN apt update && apt -y upgrade \
    && apt install -y gcc spawn-fcgi libfcgi-dev \
    && rm -rf /var/lib/apt/lists
WORKDIR /home/
COPY ./server/. ./server/.
COPY main.sh .
COPY ./nginx/nginx.conf /etc/nginx/nginx.conf
RUN chown -R nginx:nginx /var /run /home /etc/nginx
USER nginx