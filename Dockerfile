FROM alpine:3.15.2

RUN apk add build-base libc6-compat

WORKDIR /cspec/

COPY . .

RUN make install

CMD [ "/bin/sh" ]
