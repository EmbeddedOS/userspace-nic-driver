# Huge TLB page

For more detail, visit: [Huge TLB page](https://docs.kernel.org/admin-guide/mm/hugetlbpage.html).

Paging, in short, is a memory management scheme where the OS instead of mapping addresses one by one from virtual memory into physical memory, it maps addresses by pages. The page size should be balance between memory usage and the complexity of managing virtual memory. The size too small can lead to too complexity of managing virtual memory, increasing the page table entries, looking up to the translate table too much. While large page size improve the performance, it also can lead memory fragmentation inside the page and wasting memory usage.

By default the page size is kept at 4KiB in almost modern OSes.

## Why use huge page

We need huge memory to process packets, and we don't want the OS translate virtual to physical address too much that can cause lower latency. Roughly, Less translation Lookaside Buffers means less translating time, reduce latency.

We implement Mempool for packet buffers, so that we can avoid internal memory fragmentation issue for huge pages.

## We use huge page, that's ok, but how huge?

Pages, even huge pages, can only be mapped to a physical memory block of the same size, and aligned to that size. That means 2MB huge page needs to be mapped to a 2MB boundary in physical RAM, similar for 1GiB huge page. Because of that, the huge pages are usually reserved at system start, when the physical memory isn't fragmented yet.

## How to enable huge pages?

If the user applications are going to request huge pages using `mmap` system call, then it is required that system administrator mount a file system of type `hugeltbfs`:

## How application use the huge pages?
