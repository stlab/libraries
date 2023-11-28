use std::cmp::Ordering;

/// A priority queue with three priorities. Elements with equal priorities are popped in FIFO order.
pub struct CoarsePriorityQueue<T> {
    inner: std::collections::BinaryHeap<Prioritized<T>>,
    count: usize,
}

impl<T> CoarsePriorityQueue<T> {
    /// Creates a new, empty `CoarsePriorityQueue`.
    pub fn new() -> Self {
        Self {
            inner: std::collections::BinaryHeap::new(),
            count: 0,
        }
    }

    /// Checks if the queue is empty.
    pub fn is_empty(&self) -> bool {
        self.inner.is_empty()
    }

    /// Removes the greatest item from the queue at returns it, or None if it is empty.
    pub fn pop(&mut self) -> Option<T> {
        self.inner
            .pop()
            .and_then(|prioritized| Some(prioritized.take_inner()))
    }

    /// Pushes an item onto the queue at the given `priority`.
    pub fn push(&mut self, item: T, priority: Priority) {
        self.inner
            .push(Prioritized::new(item, priority, self.count));
        self.count += 1;
    }
}

/// The three priorities used by `CoarsePriorityQueue`.
#[derive(Eq, PartialEq, Copy, Clone)]
#[repr(C)]
pub enum Priority {
    Low,
    Default,
    High,
}

/// Use of `Priority` and `Prioritized` requires the client maintain a nondecreasing count which
/// is supplied to the constructor of `Prioritized<T>`. That count is bitwise-OR'd with the
/// result of `highbit_mask` for a given `Priority`. The resulting `usize` is used for ordering and
/// equality of a `Prioritized<T>`. The purpose of this is to implement FIFO ordering (with coarse
/// priorities) with a single BinaryHeap.
///
/// Note: This implies the value of the count must not exceed
/// 0b001111111111111111111111111111111111111111111111111111111111111, or 2,305,843,009,213,693,951,
/// but this is considered unlikely enough to not be exposed in the public documentation for
/// CoarsePriorityQueue.
impl Priority {
    const fn highbit_mask(&self) -> usize {
        match self {
            Priority::Low => 0 << usize::BITS - 2,
            Priority::Default => 1 << usize::BITS - 2,
            Priority::High => 2 << usize::BITS - 2,
        }
    }

    /// Precondition: count < 2,305,843,009,213,693,951
    fn merge_priority_count(&self, count: usize) -> usize {
        self.highbit_mask() | count
    }
}

impl PartialOrd for Priority {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        self.highbit_mask().partial_cmp(&other.highbit_mask())
    }
}

/// Pairs an instance of `T` with a `Priority`.
/// Equality and ordering of a Prioritized<T> only considers `priority`, and disregards `inner`.
struct Prioritized<T> {
    inner: T,
    priority: usize,
}

impl<T> Prioritized<T> {
    /// Precondition: count must be less than 2,305,843,009,213,693,951.
    pub fn new(inner: T, priority: Priority, count: usize) -> Self {
        Prioritized {
            inner,
            priority: priority.merge_priority_count(count),
        }
    }

    pub fn take_inner(self) -> T {
        self.inner
    }
}

impl<T> PartialEq for Prioritized<T> {
    #[inline]
    fn eq(&self, other: &Self) -> bool {
        self.priority == other.priority
    }
}

impl<T> Eq for Prioritized<T> {}

impl<T> PartialOrd for Prioritized<T> {
    #[inline]
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        self.priority.partial_cmp(&other.priority)
    }
}

impl<T> Ord for Prioritized<T> {
    #[inline]
    fn cmp(&self, other: &Self) -> Ordering {
        self.priority.cmp(&other.priority)
    }
}
