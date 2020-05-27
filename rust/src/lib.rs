#![feature(test)]

extern crate test;
use std::collections::HashSet;
use std::iter;
use test::Bencher;

#[derive(Copy, Clone, Debug)]
pub struct Rate {
    pub id: u8,
    pub rate: f64,
    pub vol: f64,
    pub from: Currency,
    pub to: Currency,
    pub exchange: Exchange,
}

/// A pair or triple of rates that may form a closed loop
#[derive(Copy, Clone, Debug)]
pub struct Arb<'a> {
    a: &'a Rate,
    b: &'a Rate,
    c: Option<&'a Rate>,
}

#[derive(Copy, Clone, Debug, PartialEq, PartialOrd, Eq)]
pub enum Exchange {
    D,
    G,
    J,
    Foo,
}

#[derive(Copy, Clone, Debug, PartialEq, PartialOrd, Eq)]
pub enum Currency {
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    Y,
    Z,
}

impl PartialEq for Rate {
    fn eq(&self, other: &Self) -> bool {
        self.id == other.id
    }
}

impl<'a> PartialEq for Arb<'a> {
    fn eq(&self, other: &Self) -> bool {
        self.bitset() == other.bitset()
    }
}

#[derive(Debug, PartialEq)]
pub struct Bitset {
    arr: [u64; 4],
}

impl Bitset {
    pub const fn empty() -> Self {
        Bitset { arr: [0u64; 4] }
    }

    fn set(&mut self, val: u8) {
        let idx = val / 64;
        let shift = val % 64;
        self.arr[idx as usize] |= (1 << shift);
    }

    fn test(&mut self, val: u8) -> bool {
        let idx = val / 64;
        let shift = val % 64;
        (self.arr[idx as usize] & (1 << shift)) != 0
    }
}

impl<'a> Arb<'a> {
    fn is_closed(&self) -> bool {
        if self.a.from == self.b.to {
            return true;
        }
        if let Some(third) = self.c {
            return self.a.from == third.to;
        }
        false
    }

    fn contains(&self, rate: &Rate) -> bool {
        let c = match self.c {
            Some(r) => r == rate,
            _ => false,
        };
        self.a == rate || self.b == rate || c
    }

    fn is_arb(&self) -> bool {
        if !self.is_closed() {
            return false;
        }

        self.a.rate * self.b.rate * self.c.map(|o| o.rate).unwrap_or(1.0) > 1.0
    }

    pub fn bitset(&self) -> Bitset {
        let mut bs = Bitset::empty();
        bs.set(self.a.id);
        bs.set(self.b.id);
        bs.set(self.c.map(|c| c.id).unwrap_or(0));
        bs
    }
}

pub fn arb_from_rates<'a>(rates: &[&'a Rate]) -> Vec<Arb<'a>> {
    let base = build_base(rates);
    let mut out = Vec::with_capacity(base.len() * 10);
    for b in base {
        if b.is_closed() {
            out.push(b);
            continue;
        }
        for &i in rates {
            if !b.contains(i) && b.b.to == i.from && i.to == b.a.from {
                let mut closed = b;
                closed.c = Some(i);
                if closed.is_arb() {
                    out.push(closed);
                }
            }
        }
    }
    out.dedup();
    out
}

#[inline]
fn build_base<'a>(rates: &[&'a Rate]) -> Vec<Arb<'a>> {
    rates
        .iter()
        .enumerate()
        .flat_map(|(i, &rate_a)| {
            rates[(i + 1)..].iter().filter_map(move |&rate_b| {
                if rate_a.to == rate_b.from {
                    Some(Arb {
                        a: rate_a,
                        b: rate_b,
                        c: None,
                    })
                } else {
                    None
                }
            })
        })
        .collect()
}

#[inline]
pub fn v_to_rc<'a>(mut v: Vec<&'a Rate>) -> Arb<'a> {
    Arb {
        a: v.remove(0),
        b: v.remove(0),
        c: v.pop(),
    }
}

fn main() {
    let r1 = &Rate {
        id: 0,
        from: Currency::A,
        to: Currency::B,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r2 = &Rate {
        id: 2,
        from: Currency::A,
        to: Currency::C,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r3 = &Rate {
        id: 3,
        from: Currency::A,
        to: Currency::D,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r4 = &Rate {
        id: 4,
        from: Currency::A,
        to: Currency::E,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r5 = &Rate {
        id: 5,
        from: Currency::B,
        to: Currency::A,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r6 = &Rate {
        id: 6,
        from: Currency::B,
        to: Currency::C,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r7 = &Rate {
        id: 7,
        from: Currency::B,
        to: Currency::D,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r8 = &Rate {
        id: 8,
        from: Currency::B,
        to: Currency::E,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r9 = &Rate {
        id: 9,
        from: Currency::C,
        to: Currency::A,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r10 = &Rate {
        id: 10,
        from: Currency::C,
        to: Currency::B,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r11 = &Rate {
        id: 11,
        from: Currency::C,
        to: Currency::D,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r12 = &Rate {
        id: 12,
        from: Currency::C,
        to: Currency::E,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r13 = &Rate {
        id: 13,
        from: Currency::D,
        to: Currency::A,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r14 = &Rate {
        id: 14,
        from: Currency::D,
        to: Currency::B,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r15 = &Rate {
        id: 15,
        from: Currency::D,
        to: Currency::C,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r16 = &Rate {
        id: 16,
        from: Currency::D,
        to: Currency::E,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r17 = &Rate {
        id: 17,
        from: Currency::E,
        to: Currency::A,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r18 = &Rate {
        id: 18,
        from: Currency::E,
        to: Currency::B,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r19 = &Rate {
        id: 19,
        from: Currency::E,
        to: Currency::C,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r20 = &Rate {
        id: 20,
        from: Currency::E,
        to: Currency::D,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let l = vec![
        r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15, r16, r17, r18, r19, r20,
    ];

    for _ in 0..100 {
        arb_from_rates(&l);
    }
}

#[test]
fn test_is_rate_in_list() {
    let r1 = Rate {
        id: 0,
        from: Currency::A,
        to: Currency::B,
        exchange: Exchange::D,
        rate: 1.0,
        vol: 1.0,
    };
    let r2 = Rate {
        id: 1,
        from: Currency::E,
        to: Currency::F,
        exchange: Exchange::G,
        rate: 1.0,
        vol: 1.0,
    };
    let r3 = Rate {
        id: 2,
        from: Currency::H,
        to: Currency::I,
        exchange: Exchange::J,
        rate: 1.0,
        vol: 1.0,
    };
    let l1 = v_to_rc(vec![&r1, &r2, &r3]);
    let l2 = v_to_rc(vec![&r2, &r3]);

    assert_eq!(l1.contains(&r1), true);
    assert_eq!(l2.contains(&r1), false);
}

#[test]
fn test_build_base() {
    let r1 = Rate {
        id: 0,
        from: Currency::A,
        to: Currency::B,
        exchange: Exchange::J,
        rate: 1.0,
        vol: 1.0,
    };
    let r2 = Rate {
        id: 1,
        from: Currency::B,
        to: Currency::A,
        exchange: Exchange::Foo,
        rate: 1.0,
        vol: 1.0,
    };
    let l1 = vec![&r1, &r2];

    let b1 = build_base(&l1);
    assert_ne!(b1.len(), 0);
}

#[test]
fn test_is_list_closing() {
    let r1 = Rate {
        id: 0,
        from: Currency::A,
        to: Currency::B,
        exchange: Exchange::J,
        rate: 1.0,
        vol: 1.0,
    };
    let r2 = Rate {
        id: 1,
        from: Currency::B,
        to: Currency::A,
        exchange: Exchange::Foo,
        rate: 1.0,
        vol: 1.0,
    };
    let r3 = Rate {
        id: 2,
        from: Currency::B,
        to: Currency::C,
        exchange: Exchange::Foo,
        rate: 1.0,
        vol: 1.0,
    };
    let l1 = v_to_rc(vec![&r1, &r2]);
    let l2 = v_to_rc(vec![&r1, &r3]);

    assert_eq!(l1.is_closed(), true);
    assert_eq!(l2.is_closed(), false);
}

#[test]
fn test_combos_from_rates() {
    let r1 = Rate {
        id: 0,
        from: Currency::A,
        to: Currency::B,
        exchange: Exchange::J,
        rate: 1.0,
        vol: 1.0,
    };
    let r2 = Rate {
        id: 1,
        from: Currency::B,
        to: Currency::C,
        exchange: Exchange::J,
        rate: 1.0,
        vol: 1.0,
    };
    let r3 = Rate {
        id: 2,
        from: Currency::C,
        to: Currency::D,
        exchange: Exchange::J,
        rate: 1.0,
        vol: 1.0,
    };
    let r4 = Rate {
        id: 3,
        from: Currency::Y,
        to: Currency::Z,
        exchange: Exchange::J,
        rate: 1.0,
        vol: 1.0,
    };

    let l1 = vec![&r1, &r2, &r3, &r4];
    let c1 = arb_from_rates(&l1);

    assert_eq!(c1.len(), 0);
}

#[test]
fn test_is_arb() {
    assert_eq!(
        v_to_rc(vec![
            &Rate {
                id: 0,
                from: Currency::A,
                to: Currency::B,
                exchange: Exchange::J,
                rate: 1.0,
                vol: 1.0,
            },
            &Rate {
                id: 1,
                from: Currency::B,
                to: Currency::A,
                exchange: Exchange::J,
                rate: 1.0,
                vol: 1.0,
            }
        ])
        .is_arb(),
        false
    );

    assert_eq!(
        v_to_rc(vec![
            &Rate {
                id: 0,
                from: Currency::Y,
                to: Currency::Z,
                exchange: Exchange::J,
                rate: 1.0,
                vol: 1.0,
            },
            &Rate {
                id: 1,
                from: Currency::A,
                to: Currency::B,
                exchange: Exchange::J,
                rate: 1.0,
                vol: 1.0,
            }
        ])
        .is_arb(),
        false
    );

    assert_eq!(
        v_to_rc(vec![
            &Rate {
                id: 0,
                from: Currency::A,
                to: Currency::B,
                exchange: Exchange::J,
                rate: 2.0,
                vol: 1.0,
            },
            &Rate {
                id: 1,
                from: Currency::B,
                to: Currency::C,
                exchange: Exchange::J,
                rate: 2.0,
                vol: 1.0,
            }
        ])
        .is_arb(),
        false
    );

    assert_eq!(
        v_to_rc(vec![
            &Rate {
                id: 0,
                from: Currency::A,
                to: Currency::B,
                exchange: Exchange::J,
                rate: 2.0,
                vol: 1.0,
            },
            &Rate {
                id: 1,
                from: Currency::B,
                to: Currency::C,
                exchange: Exchange::J,
                rate: 2.0,
                vol: 1.0,
            },
            &Rate {
                id: 2,
                from: Currency::C,
                to: Currency::A,
                exchange: Exchange::J,
                rate: 2.0,
                vol: 1.0,
            }
        ])
        .is_arb(),
        true
    );

    assert_eq!(
        v_to_rc(vec![
            &Rate {
                id: 0,
                from: Currency::A,
                to: Currency::B,
                exchange: Exchange::J,
                rate: 1.0,
                vol: 1.0,
            },
            &Rate {
                id: 1,
                from: Currency::B,
                to: Currency::C,
                exchange: Exchange::J,
                rate: 1.0,
                vol: 1.0,
            },
            &Rate {
                id: 2,
                from: Currency::C,
                to: Currency::A,
                exchange: Exchange::J,
                rate: 1.0,
                vol: 1.0,
            }
        ])
        .is_arb(),
        false
    );
}

#[test]
fn test_arb_from_rates() {
    let r1 = Rate {
        id: 0,
        from: Currency::A,
        to: Currency::B,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r2 = Rate {
        id: 1,
        from: Currency::B,
        to: Currency::C,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r3 = Rate {
        id: 3,
        from: Currency::C,
        to: Currency::A,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r4 = Rate {
        id: 4,
        from: Currency::Y,
        to: Currency::Z,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };

    let l1 = vec![&r1, &r2, &r3, &r4];
    let arb = arb_from_rates(&l1);
    assert_eq!(arb, vec![v_to_rc(vec![&r1, &r2, &r3])]);
}

#[bench]
fn bench_arb(b: &mut Bencher) {
    let r1 = &Rate {
        id: 0,
        from: Currency::A,
        to: Currency::B,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r2 = &Rate {
        id: 2,
        from: Currency::A,
        to: Currency::C,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r3 = &Rate {
        id: 3,
        from: Currency::A,
        to: Currency::D,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r4 = &Rate {
        id: 4,
        from: Currency::A,
        to: Currency::E,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r5 = &Rate {
        id: 5,
        from: Currency::B,
        to: Currency::A,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r6 = &Rate {
        id: 6,
        from: Currency::B,
        to: Currency::C,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r7 = &Rate {
        id: 7,
        from: Currency::B,
        to: Currency::D,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r8 = &Rate {
        id: 8,
        from: Currency::B,
        to: Currency::E,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r9 = &Rate {
        id: 9,
        from: Currency::C,
        to: Currency::A,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r10 = &Rate {
        id: 10,
        from: Currency::C,
        to: Currency::B,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r11 = &Rate {
        id: 11,
        from: Currency::C,
        to: Currency::D,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r12 = &Rate {
        id: 12,
        from: Currency::C,
        to: Currency::E,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r13 = &Rate {
        id: 13,
        from: Currency::D,
        to: Currency::A,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r14 = &Rate {
        id: 14,
        from: Currency::D,
        to: Currency::B,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r15 = &Rate {
        id: 15,
        from: Currency::D,
        to: Currency::C,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r16 = &Rate {
        id: 16,
        from: Currency::D,
        to: Currency::E,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r17 = &Rate {
        id: 17,
        from: Currency::E,
        to: Currency::A,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r18 = &Rate {
        id: 18,
        from: Currency::E,
        to: Currency::B,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r19 = &Rate {
        id: 19,
        from: Currency::E,
        to: Currency::C,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let r20 = &Rate {
        id: 20,
        from: Currency::E,
        to: Currency::D,
        exchange: Exchange::J,
        rate: 2.0,
        vol: 1.0,
    };
    let l = vec![
        r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15, r16, r17, r18, r19, r20,
    ];

    // note: don't want rust to optimize away by not using output
    //let mut tmp: Vec<Vec<Vec<&Rate>>> = Vec::new();
    b.iter(|| {
        //tmp = arb_from_rates(&l, 5);
        arb_from_rates(&l);
    });
    //println!("tmp len: {}", tmp.len());
}
