FROM ubuntu:18.04 AS builder

RUN apt-get -y update \
    && apt-get install --no-install-recommends -y \
    g++ cmake make libmysqlcppconn-dev 

WORKDIR /server
COPY . .

RUN ./build.sh

FROM ubuntu:18.04
WORKDIR /server
COPY --from=builder /server/build/server ./
COPY --from=builder /usr/lib/x86_64-linux-gnu/ /usr/lib/x86_64-linux-gnu/
CMD [ "./server" ]
