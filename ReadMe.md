# Caching Proxy

Build a caching server that caches responses from other servers.

You are required to build a CLI tool that starts a caching proxy server. It will forward requests to the actual server and cache the responses. If the same request is made again, it will return the cached response instead of forwarding the request to the server.

https://roadmap.sh/projects/caching-server

## Requirements

The user should be able to start the caching proxy server by running a command like the following:

```bash
caching-proxy --port <number> --origin <url>

    --port is the port on which the caching proxy server will run.

    --origin is the URL of the server to which the requests will be forwarded.

Example

caching-proxy --port 3000 --origin http://dummyjson.com

This starts the caching proxy server on port 3000 and forwards requests to http://dummyjson.com.

If a user makes a request to:

http://localhost:3000/products

The caching proxy server will forward the request to:

http://dummyjson.com/products

It will return the response along with headers and cache the response. The response will include headers indicating whether the response is from the cache or the server:

# If the response is from the cache
X-Cache: HIT

# If the response is from the origin server
X-Cache: MISS

If the same request is made again, the caching proxy server will return the cached response instead of forwarding the request to the server.
Clearing the Cache

You can clear the cache by running:

caching-proxy --clear-cache

Learning Outcome

After building this project, you will have a solid understanding of how caching works and how to build a caching proxy server to cache responses from other servers.
