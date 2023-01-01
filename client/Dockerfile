FROM ubuntu:18.04 AS builder

WORKDIR /client
COPY . .

RUN apt-get -y update \ 
    && apt-get install -y --no-install-recommends \ 
    g++ libmysqlcppconn-dev nlohmann-json-dev \
    && g++ -o client ./src/main.cpp ./src/client.cpp ./src/io.cpp ./src/sha256.cpp -pthread

FROM ubuntu:18.04
WORKDIR /client
COPY --from=builder /client/client ./
CMD [ "./client" ]
