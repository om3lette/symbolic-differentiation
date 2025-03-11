FROM ubuntu:22.04 AS builder

WORKDIR /sources
COPY . .

RUN apt-get update && apt-get upgrade -y && \
    apt-get install make build-essential -y && \
    make RELEASE=1 all

FROM ubuntu:22.04
COPY --from=builder /sources/build/differentiator /sources/build/test-build /symbolic-differentiation/
WORKDIR /symbolic-differentiation

CMD ["./test-build"]
