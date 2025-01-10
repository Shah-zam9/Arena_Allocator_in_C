<h1>Custom Arena Allocator</h1>

<p>This project implements a custom memory allocator that manages a pre-allocated memory region. The allocator follows specific alignment rules to efficiently allocate memory chunks</p>

<h2>Alignment Rules</h2>
<ul>
    <li>Data of size <strong>1 byte</strong> can be placed at any address.</li>
    <li>Data sizes that are powers of two must be placed at an address that is a multiple of that power of two.</li>
    <li>Data sizes less than 8 bytes, but not powers of two, must be placed at an address that is a multiple of the nearest smaller power of two.</li>
    <li>Data sizes greater than 8 bytes must be placed at an address that is a multiple of 8.</li>
</ul>
