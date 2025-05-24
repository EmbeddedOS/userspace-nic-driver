# Huge TLB page

For more detail, visit: [Huge TLB page](https://docs.kernel.org/admin-guide/mm/hugetlbpage.html).

Paging, in short, is a memory management scheme where the OS instead of mapping addresses one by one from virtual memory into physical memory, it maps addresses by pages. The page size should be balance between memory usage and the complexity of managing virtual memory. The size too small can lead to too complexity of managing virtual memory, increasing the page table entries, looking up to the translate table too much. While large page size improve the performance, it also can lead memory fragmentation inside the page and wasting memory usage.

By default the page size is kept at 4KiB in almost modern OSes.

## Why we use huge pages in our application
