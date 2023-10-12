use std::cmp::{Ordering};

/// A `usize` constraining valid values to [0, 4) with runtime assertions.
#[derive(Eq, PartialEq, Ord, PartialOrd, Copy, Clone)]
pub struct Priority(pub usize);

impl Priority {
    pub fn new(value: usize) -> Self {
        assert!((0..4).contains(&value), "Priorities must be in [0, 4)");
        Self(value)
    }

    /// Returns a usize of the form 0bXX000000 where XX is a binary representation of this priority.
    /// The value of this priority is guaranteed to fit in two bits because `Priority` values are constrained to [0b00, 0b11].
    pub fn to_highbit_mask(&self) -> usize {
        &self.0 << (usize::BITS - 2)
    }
}

/// Pairs an instance of `T` with a `Priority`. 
/// Equality and ordering of a Prioritized<T> only considers `priority`, disregarding `element`.
pub struct Prioritized <T> {
    priority: Priority,
    element: T
}

impl <T> Prioritized<T> {
    pub fn new(priority: Priority, element: T) -> Self {
        Prioritized { priority, element }
    }

    pub fn take_element(self) -> T {
        self.element
    }
}

impl<T> PartialEq for Prioritized <T> {
    #[inline]
    fn eq(&self, other: &Self) -> bool {
        self.priority == other.priority
    }
}

impl <T> Eq for Prioritized <T> {}

impl<T> PartialOrd for Prioritized <T> {
    #[inline]
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        self.priority.partial_cmp(&other.priority)
    }
}

impl<T> Ord for Prioritized <T> {
    #[inline]
    fn cmp(&self, other: &Self) -> Ordering {
        self.priority.cmp(&other.priority)
    }
}