FROM frolvlad/alpine-glibc

RUN apk add --update libstdc++ && rm -rf /var/cache/apk/*

COPY build/bin/wello /opt/wello/bin/wello

ENTRYPOINT /opt/wello/bin/wello