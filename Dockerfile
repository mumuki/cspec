FROM alpine:3.15

RUN apk add build-base vim

WORKDIR /cspec/

COPY . .

RUN make && make install
