# rng_buf

This is an optimized Ring Buffer implementation using the POSIX shared memory trick.

This implementation is much more convenient in use then classical
implementations, also there is some benefit in CPU usage.

A circular-buffer implementation is optimized by mapping the underlying buffer
to two contiguous regions of virtual memory. (Naturally, the underlying
buffer‘s length must then equal some multiple of the system’s page size.)
Reading from and writing to the circular buffer may then be carried out with
greater efficiency by means of direct memory access; those accesses which fall
beyond the end of the first virtual-memory region will automatically wrap
around to the beginning of the underlying buffer. When the read offset is
advanced into the second virtual-memory region, both offsets—read and
write—are decremented by the length of the underlying buffer.

 - https://www.gnuradio.org/blog/2017-01-05-buffers/
 - https://fgiesen.wordpress.com/2012/07/21/the-magic-ring-buffer/
 - https://en.wikipedia.org/wiki/Circular_buffer#Optimization

### Limitations

This implementation is HW-dependent and was tested for x86_64. You will observe
funny effects when HW uses VIVT caches.

If you would like to use it on different HW, you should have done some testing
in advance. src/rng_buf_tests could be a start point for it.

### Requirements

1. Support for POSIX shared memory
2. Proper HW (cache-controller)

### Build

```
 $ cmake . && make
```

### Example

Use src/rng_buf_tests.cc as an example.
