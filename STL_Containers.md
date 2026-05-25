## 1. Sequence Containers (Linear Data)

Sequence containers store elements in a strict linear sequence.

### `std::vector` (Dynamic Array)

- **Internal Working:** Manages a dynamically allocated contiguous block of memory on the heap. When capacity is reached, it allocates a new block (usually 1.5x or 2x size), copies elements, and frees the old block.
- **CP Trivia / Suggestion:** Always use `reserve(N)` if you know the maximum number of elements in advance to prevent costly reallocations. Prefer `emplace_back()` over `push_back()` when adding complex objects to construct them directly in place.

**Complexities (Space: $O(N)$):**

| Operation           | Best   | Average | Worst  | When / Why                                                          |
| ------------------- | ------ | ------- | ------ | ------------------------------------------------------------------- |
| **Access (`[]`)**   | $O(1)$ | $O(1)$  | $O(1)$ | Direct memory offset.                                               |
| **Insert (Back)**   | $O(1)$ | $O(1)$  | $O(N)$ | Worst case occurs when capacity is full, triggering a reallocation. |
| **Insert (Middle)** | $O(N)$ | $O(N)$  | $O(N)$ | Must shift all subsequent elements to the right.                    |

**Important APIs:**

```cpp
std::vector<int> v;
v.reserve(100);       // Pre-allocate memory
v.push_back(10);      // Add to end
v.emplace_back(20);   // Construct at end (faster for objects)
v.pop_back();         // Remove last element
int x = v.front();    // First element
int y = v.back();     // Last element

```

### `std::deque` (Double-Ended Queue)

- **Internal Working:** An array of pointers to fixed-size memory chunks (pages) scattered across the heap. It grows by allocating new chunks and updating the central pointer array.
- **CP Trivia / Suggestion:** The absolute best choice for **Sliding Window Minimum/Maximum** problems. Avoid using it just for random access, as it has slightly higher overhead than a `vector`.

**Complexities (Space: $O(N)$):**

| Operation           | Best   | Average | Worst  | When / Why                                                    |
| ------------------- | ------ | ------- | ------ | ------------------------------------------------------------- |
| **Access (`[]`)**   | $O(1)$ | $O(1)$  | $O(1)$ | Two pointer lookups (find chunk -> find index).               |
| **Insert (Ends)**   | $O(1)$ | $O(1)$  | $O(1)$ | Allocates a new chunk instantly if the current ends are full. |
| **Insert (Middle)** | $O(N)$ | $O(N)$  | $O(N)$ | Requires shifting elements within and across chunks.          |

**Important APIs:**

```cpp
std::deque<int> dq;
dq.push_front(1); // Fast insert at front
dq.push_back(2);  // Fast insert at back
dq.pop_front();   // Fast remove from front
int val = dq[0];  // Random access allowed

```

### `std::list` (Doubly-Linked List)

- **Internal Working:** Heap-allocated individual nodes connected via `prev` and `next` pointers.
- **CP Trivia / Suggestion:** **Avoid in Competitive Programming.** The lack of contiguous memory destroys CPU cache locality, making it practically much slower than a `vector` even for operations where it theoretically shines.

**Complexities (Space: $O(N)$ - High overhead per node):**

| Operation         | Best   | Average | Worst  | When / Why                                                            |
| ----------------- | ------ | ------- | ------ | --------------------------------------------------------------------- |
| **Access**        | $O(1)$ | $O(N)$  | $O(N)$ | Best case is accessing the very front/back. Otherwise, must traverse. |
| **Insert/Delete** | $O(1)$ | $O(1)$  | $O(1)$ | Requires you to already have the iterator pointing to the location.   |

**Important APIs:**

```cpp
std::list<int> l = {1, 2, 3};
l.push_front(0);
// Insert in middle requires an iterator
auto it = l.begin();
std::advance(it, 2);
l.insert(it, 99);

```

---

## 2. Associative Containers (Sorted Data)

These containers keep their elements sorted automatically.

### `std::map` & `std::set`

- **Internal Working:** Implemented as **Red-Black Trees** (self-balancing binary search trees). Nodes are re-colored and rotated upon insertion/deletion to keep the tree height at $O(\log N)$.
- **CP Trivia / Suggestion:** Use these when you need to answer queries about the _order_ of elements, such as "find the smallest element strictly greater than X". The `.lower_bound()` and `.upper_bound()` functions are incredibly powerful for binary search over dynamic data.

**Complexities (Space: $O(N)$):**

| Operation         | Best        | Average     | Worst       | When / Why                                                           |
| ----------------- | ----------- | ----------- | ----------- | -------------------------------------------------------------------- |
| **Search / Find** | $O(1)$      | $O(\log N)$ | $O(\log N)$ | Best case: element is at the root. Otherwise, traverses tree height. |
| **Insert/Delete** | $O(\log N)$ | $O(\log N)$ | $O(\log N)$ | Traversal + Tree rebalancing time.                                   |

**Important APIs:**

```cpp
std::set<int> s;
s.insert(10);
s.erase(10);
// Binary search built-in
auto it = s.lower_bound(5); // First element >= 5

std::map<std::string, int> m;
m["Alice"] = 1; // Inserts or updates
if (m.count("Alice")) { /* Check existence */ }

```

---

## 3. Unordered Associative Containers (Hash Tables)

These containers optimize for speed over order.

### `std::unordered_map` & `std::unordered_set`

- **Internal Working:** Hash tables with separate chaining. Elements are hashed into an integer index corresponding to an array of "buckets". Collisions (multiple keys hashing to the same index) form a linked list in that bucket.
- **CP Trivia / Suggestion:** **Beware of "Hash Clashes" in platforms like Codeforces.** Malicious test cases are designed to trigger the worst-case $O(N)$ complexity by supplying numbers that all hash to the same bucket. Always use a custom hash function (like `chrono::steady_clock` combined with an `xorshift`) if using `unordered_map` in CP, or just stick to `std::map` if $O(\log N)$ is fast enough.

**Complexities (Space: $O(N)$):**

| Operation         | Best   | Average | Worst  | When / Why                                                               |
| ----------------- | ------ | ------- | ------ | ------------------------------------------------------------------------ |
| **Search/Find**   | $O(1)$ | $O(1)$  | $O(N)$ | Worst case occurs when many keys hash to the exact same bucket.          |
| **Insert/Delete** | $O(1)$ | $O(1)$  | $O(N)$ | Constant unless a massive rehash is triggered or heavy collisions exist. |

**Important APIs:**

```cpp
std::unordered_map<int, int> freq;
freq[5]++; // Fast frequency counting
if (freq.find(5) != freq.end()) {
    // Element exists
}

```

---

## 4. Container Adaptors (Restricted Interfaces)

These wrap existing containers (usually `deque` or `vector`) to enforce specific rules. They do not have iterators.

### `std::stack` (LIFO) & `std::queue` (FIFO)

- **Internal Working:** By default, both wrap a `std::deque`. They simply disable random access and restrict insertions/deletions to specific ends.
- **CP Trivia / Suggestion:** Stacks are essential for Depth-First Search (DFS) (though recursion is usually preferred) and Monotonic Stack problems (e.g., finding the Next Greater Element). Queues are the backbone of Breadth-First Search (BFS).

**Complexities (Space: $O(N)$):**

| Operation                       | Time Complexity | Notes                                         |
| ------------------------------- | --------------- | --------------------------------------------- |
| **Push / Pop**                  | $O(1)$          | Handled entirely by the underlying container. |
| **Top (Stack) / Front (Queue)** | $O(1)$          | Direct access.                                |

**Important APIs:**

```cpp
std::stack<int> st;
st.push(1);
int top_val = st.top(); // Peek at top
st.pop();               // Remove top

std::queue<int> q;
q.push(1);
int front_val = q.front(); // Peek at front
q.pop();                   // Remove front

```

### `std::priority_queue` (Heaps)

- **Internal Working:** Wraps a `std::vector` and manages it as a Binary Max-Heap using algorithms like `std::make_heap`, `std::push_heap`, and `std::pop_heap`.
- **CP Trivia / Suggestion:** Essential for Dijkstra's Algorithm (Shortest Path) and greedy algorithms. **Important:** By default, it is a Max-Heap (largest element is on top). To make a Min-Heap, you must pass comparator templates.

**Complexities (Space: $O(N)$):**

| Operation      | Time Complexity | Notes                                                                 |
| -------------- | --------------- | --------------------------------------------------------------------- |
| **Top (Peek)** | $O(1)$          | The root of the heap is always at index 0 of the underlying vector.   |
| **Push / Pop** | $O(\log N)$     | Must "bubble up" or "bubble down" elements to maintain heap property. |

**Important APIs:**

```cpp
// Default: Max-Heap
std::priority_queue<int> max_pq;
max_pq.push(10);
max_pq.push(50);
int highest = max_pq.top(); // 50

// Min-Heap configuration
std::priority_queue<int, std::vector<int>, std::greater<int>> min_pq;
min_pq.push(10);
min_pq.push(50);
int lowest = min_pq.top(); // 10

```
