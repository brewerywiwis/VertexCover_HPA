FROM gcc:9.3

RUN mkdir -p /app

WORKDIR /app

COPY ./VertexCover.cpp /app

RUN g++ -fopenmp -O3 -o prog VertexCover.cpp

RUN rm ./VertexCover.cpp

ENV PATH="/app:${PATH}"