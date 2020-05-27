#![feature(test)]

extern crate test;
use std::collections::HashSet;
use std::iter;
use test::Bencher;

#[derive(Debug)]
pub struct Rate {
    pub id: u8,
    pub from: String,
    pub to: String,
    pub exchange: String,
    pub rate: f64,
    pub vol: f64,
}

impl PartialEq for Rate {
    fn eq(&self, other: &Self) -> bool {
        self.id == other.id
    }
}

pub fn arb_from_rates<'a>(rates: &[&'a Rate], depth: u32) -> Vec<Vec<Vec<&'a Rate>>> {
    arb_from_combos(combos_from_rates(rates, depth))
}

fn arb_from_combos(mut combos: Vec<Vec<Vec<&Rate>>>) -> Vec<Vec<Vec<&Rate>>> {
    combos.iter_mut().for_each(|x| {
        x.retain(|j| is_arb(&j));
        x.iter_mut().for_each(|x| x.sort_unstable_by_key(|r| r.id));
        // Relying on the fact that each array will be unique once sorted
        x.dedup();
    });
    combos
}

fn is_arb(list: &[&Rate]) -> bool {
    if list.len() < 2 {
        return false;
    }

    if list[0].from != list[list.len() - 1].to {
        return false;
    }

    list.windows(2)
        .try_fold(list[0].rate, |prod, rates| {
            if rates[0].to != rates[1].from {
                return None;
            }
            Some(prod * rates[1].rate)
        })
        .map_or(false, |prod| prod > 1.0)
}

fn combos_from_rates<'a>(rates: &[&'a Rate], depth: u32) -> Vec<Vec<Vec<&'a Rate>>> {
    let mut ret = Vec::with_capacity(depth as usize + 1);
    ret.push(build_base(rates));

    for i in 1..(depth as usize) {
        let mut tmp = Vec::new();
        for j in 0..ret[i - 1].len() {
            for k in 0..rates.len() {
                if ret[i - 1][j].last().unwrap().to == rates[k].from
                    && !is_rate_in_list(&ret[i - 1][j], rates[k])
                    && !is_list_closing(&ret[i - 1][j])
                {
                    tmp.push(
                        ret[i - 1][j]
                            .iter()
                            .chain(iter::once(&rates[k]))
                            .copied()
                            .collect(),
                    );
                }
            }
        }
        ret.push(tmp);
    }

    ret
}

fn is_list_closing(list: &Vec<&Rate>) -> bool {
    list[0].from == list.last().unwrap().to
}

fn build_base<'a>(rates: &[&'a Rate]) -> Vec<Vec<&'a Rate>> {
    rates
        .iter()
        .enumerate()
        .flat_map(|(i, &rate_a)| {
            rates[(i + 1)..].iter().filter_map(move |&rate_b| {
                if rate_a.to == rate_b.from {
                    Some(vec![rate_a, rate_b])
                } else {
                    None
                }
            })
        })
        .collect()
}

fn is_rate_in_list(list: &[&Rate], r: &Rate) -> bool {
    list.iter().any(|&rate| rate == r)
}

fn is_dupe<'a>(list: &'a Vec<Vec<&'a Rate>>, arb: &'a Vec<&'a Rate>) -> bool {
    if list.len() == 0 {
        return false
    }

    let mut m: HashSet<u8> = HashSet::new();

    for i in 0..arb.len() {
        m.insert(
            arb[i].id,
        );
    }

    for i in 0..list.len() {
        let mut count = 0;
        for j in 0..list[i].len() {
            if m.contains(&list[i][j].id) {
                count = count + 1;
            }
        }

        if count == list[i].len() {
            return true
        }
    }

    return false
}

fn main() {
    let r1 = &Rate{
        id: 0,
        from: String::from("a"),
        to: String::from("b"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r2 = &Rate{
        id: 2,
        from: String::from("a"),
        to: String::from("c"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r3 = &Rate{
        id: 3,
        from: String::from("a"),
        to: String::from("d"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r4 = &Rate{
        id: 4,
        from: String::from("a"),
        to: String::from("e"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r5 = &Rate{
        id: 5,
        from: String::from("b"),
        to: String::from("a"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r6 = &Rate{
        id: 6,
        from: String::from("b"),
        to: String::from("c"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r7 = &Rate{
        id: 7,
        from: String::from("b"),
        to: String::from("d"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r8 = &Rate{
        id: 8,
        from: String::from("b"),
        to: String::from("e"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r9 = &Rate{
        id: 9,
        from: String::from("c"),
        to: String::from("a"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r10 = &Rate{
        id: 10,
        from: String::from("c"),
        to: String::from("b"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r11 = &Rate{
        id: 11,
        from: String::from("c"),
        to: String::from("d"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r12 = &Rate{
        id: 12,
        from: String::from("c"),
        to: String::from("e"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r13 = &Rate{
        id: 13,
        from: String::from("d"),
        to: String::from("a"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r14 = &Rate{
        id: 14,
        from: String::from("d"),
        to: String::from("b"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r15 = &Rate{
        id: 15,
        from: String::from("d"),
        to: String::from("c"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r16 = &Rate{
        id: 16,
        from: String::from("d"),
        to: String::from("e"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r17 = &Rate{
        id: 17,
        from: String::from("e"),
        to: String::from("a"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r18 = &Rate{
        id: 18,
        from: String::from("e"),
        to: String::from("b"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r19 = &Rate{
        id: 19,
        from: String::from("e"),
        to: String::from("c"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r20 = &Rate{
        id: 20,
        from: String::from("e"),
        to: String::from("d"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let l = vec![r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15, r16, r17, r18, r19, r20];

    //let mut tmp: Vec<Vec<Vec<&Rate>>> = Vec::new();
    for _ in 0..100 {
        arb_from_rates(&l, 5);
    }
    //println!("tmp len: {}", tmp.len());
}

#[test]
fn test_is_rate_in_list() {
    let r1 = Rate {
        id: 0,
        from: String::from("a"),
        to: String::from("b"),
        exchange: String::from("d"),
        rate: 1.0,
        vol: 1.0,
    };
    let r2 = Rate {
        id: 1,
        from: String::from("e"),
        to: String::from("f"),
        exchange: String::from("g"),
        rate: 1.0,
        vol: 1.0,
    };
    let r3 = Rate {
        id: 2,
        from: String::from("h"),
        to: String::from("i"),
        exchange: String::from("j"),
        rate: 1.0,
        vol: 1.0,
    };
    let l1 = vec![&r1, &r2, &r3];
    let l2 = vec![&r2, &r3];

    assert_eq!(is_rate_in_list(&l1, &r1), true);
    assert_eq!(is_rate_in_list(&l2, &r1), false);
}

#[test]
fn test_build_base() {
    let r1 = Rate{
        id: 0,
        from: String::from("a"),
        to: String::from("b"),
        exchange: String::from("j"),
        rate: 1.0,
        vol: 1.0,
    };
    let r2 = Rate{
        id: 1,
        from: String::from("b"),
        to:   String::from("a"),
        exchange: String::from("foo"),
        rate: 1.0,
        vol: 1.0,
    };
    let l1 = vec![&r1, &r2];

    let b1 = build_base(&l1);
    assert_ne!(b1.len(), 0);
}

#[test]
fn test_is_list_closing() {
    let r1 = Rate{
        id: 0,
        from: String::from("a"),
        to: String::from("b"),
        exchange: String::from("j"),
        rate: 1.0,
        vol: 1.0,
    };
    let r2 = Rate{
        id: 1,
        from: String::from("b"),
        to:   String::from("a"),
        exchange: String::from("foo"),
        rate: 1.0,
        vol: 1.0,
    };
    let r3 = Rate{
        id: 2,
        from: String::from("b"),
        to:   String::from("c"),
        exchange: String::from("foo"),
        rate: 1.0,
        vol: 1.0,
    };
    let l1 = vec![&r1, &r2];
    let l2 = vec![&r1, &r3];

    assert_eq!(is_list_closing(&l1), true);
    assert_eq!(is_list_closing(&l2), false);
}

#[test]
fn test_combos_from_rates() {
    let r1 = Rate{
        id: 0,
        from: String::from("a"),
        to: String::from("b"),
        exchange: String::from("j"),
        rate: 1.0,
        vol: 1.0,
    };
    let r2 = Rate{
        id: 1,
        from: String::from("b"),
        to: String::from("c"),
        exchange: String::from("j"),
        rate: 1.0,
        vol: 1.0,
    };
    let r3 = Rate{
        id: 2,
        from: String::from("c"),
        to: String::from("d"),
        exchange: String::from("j"),
        rate: 1.0,
        vol: 1.0,
    };
    let r4 = Rate{
        id: 3,
        from: String::from("y"),
        to: String::from("z"),
        exchange: String::from("j"),
        rate: 1.0,
        vol: 1.0,
    };

    let l1 = vec![&r1, &r2, &r3, &r4];
    let c1 = combos_from_rates(&l1, 4);

    assert_eq!(c1.len(), 4);
    assert_eq!(c1[0].len(), 2);
    assert_eq!(c1[0][0].len(), 2);
    assert_eq!(c1[0][1].len(), 2);
    assert_eq!(c1[1].len(), 1);
    assert_eq!(c1[1][0].len(), 3);
}

#[test]
fn test_is_arb() {
    assert_eq!(is_arb(&vec![&Rate{
        id: 0,
        from: String::from("y"),
        to: String::from("z"),
        exchange: String::from("j"),
        rate: 1.0,
        vol: 1.0,
    }]), false);

    assert_eq!(is_arb(&vec![&Rate{
        id: 0,
        from: String::from("a"),
        to: String::from("b"),
        exchange: String::from("j"),
        rate: 1.0,
        vol: 1.0,
    }, &Rate{
        id: 1,
        from: String::from("b"),
        to: String::from("a"),
        exchange: String::from("j"),
        rate: 1.0,
        vol: 1.0,
    }]), false);

    assert_eq!(is_arb(&vec![&Rate{
        id: 0,
        from: String::from("y"),
        to: String::from("z"),
        exchange: String::from("j"),
        rate: 1.0,
        vol: 1.0,
    }, &Rate{
        id: 1,
        from: String::from("a"),
        to: String::from("b"),
        exchange: String::from("j"),
        rate: 1.0,
        vol: 1.0,
    }]), false);

    assert_eq!(is_arb(&vec![&Rate{
        id: 0,
        from: String::from("a"),
        to: String::from("b"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    }, &Rate{
        id: 1,
        from: String::from("b"),
        to: String::from("c"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    }]), false);

    assert_eq!(is_arb(&vec![&Rate{
        id: 0,
        from: String::from("a"),
        to: String::from("b"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    }, &Rate{
        id: 1,
        from: String::from("b"),
        to: String::from("c"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    }, &Rate{
        id: 2,
        from: String::from("c"),
        to: String::from("a"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    }]), true);

    assert_eq!(is_arb(&vec![&Rate{
        id: 0,
        from: String::from("a"),
        to: String::from("b"),
        exchange: String::from("j"),
        rate: 1.0,
        vol: 1.0,
    }, &Rate{
        id: 1,
        from: String::from("b"),
        to: String::from("c"),
        exchange: String::from("j"),
        rate: 1.0,
        vol: 1.0,
    }, &Rate{
        id: 2,
        from: String::from("c"),
        to: String::from("a"),
        exchange: String::from("j"),
        rate: 1.0,
        vol: 1.0,
    }]), false);
}

#[test]
fn test_is_dupe() {
    let r1 = Rate{
        id: 0,
        from: String::from("a"),
        to: String::from("b"),
        exchange: String::from("j"),
        rate: 1.0,
        vol: 1.0,
    };
    let r2 = Rate{
        id: 1,
        from: String::from("b"),
        to: String::from("c"),
        exchange: String::from("j"),
        rate: 1.0,
        vol: 1.0,
    };
    let r3 = Rate{
        id: 2,
        from: String::from("c"),
        to: String::from("d"),
        exchange: String::from("j"),
        rate: 1.0,
        vol: 1.0,
    };
    let r4 = Rate{
        id: 3,
        from: String::from("y"),
        to: String::from("z"),
        exchange: String::from("j"),
        rate: 1.0,
        vol: 1.0,
    };

    assert_eq!(is_dupe(&vec![vec![&r1, &r2, &r3], vec![&r4, &r4, &r4]], &vec![&r1, &r2, &r3]), true);
    assert_eq!(is_dupe(&vec![vec![&r4, &r4, &r4], vec![&r4, &r4, &r4]], &vec![&r1, &r2, &r3]), false);
}

#[test]
fn test_arb_from_rates() {
    let r1 = Rate{
        id: 0,
        from: String::from("a"),
        to: String::from("b"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r2 = Rate{
        id: 1,
        from: String::from("b"),
        to: String::from("c"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r3 = Rate{
        id: 3,
        from: String::from("c"),
        to: String::from("a"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r4 = Rate{
        id: 4,
        from: String::from("y"),
        to: String::from("z"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    
    let l1 = vec![&r1, &r2, &r3, &r4];
    let arb = arb_from_rates(&l1, 4);
    assert_eq!(arb.len(), 4);
    assert_eq!(arb[0].len(), 0);
    assert_eq!(arb[1].len(), 1);
    assert_eq!(arb[1][0].len(), 3);
    assert_eq!(arb[2].len(), 0);
    assert_eq!(arb[3].len(), 0);
}

#[bench]
fn bench_arb(b: &mut Bencher) {
    let r1 = &Rate{
        id: 0,
        from: String::from("a"),
        to: String::from("b"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r2 = &Rate{
        id: 2,
        from: String::from("a"),
        to: String::from("c"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r3 = &Rate{
        id: 3,
        from: String::from("a"),
        to: String::from("d"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r4 = &Rate{
        id: 4,
        from: String::from("a"),
        to: String::from("e"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r5 = &Rate{
        id: 5,
        from: String::from("b"),
        to: String::from("a"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r6 = &Rate{
        id: 6,
        from: String::from("b"),
        to: String::from("c"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r7 = &Rate{
        id: 7,
        from: String::from("b"),
        to: String::from("d"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r8 = &Rate{
        id: 8,
        from: String::from("b"),
        to: String::from("e"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r9 = &Rate{
        id: 9,
        from: String::from("c"),
        to: String::from("a"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r10 = &Rate{
        id: 10,
        from: String::from("c"),
        to: String::from("b"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r11 = &Rate{
        id: 11,
        from: String::from("c"),
        to: String::from("d"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r12 = &Rate{
        id: 12,
        from: String::from("c"),
        to: String::from("e"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r13 = &Rate{
        id: 13,
        from: String::from("d"),
        to: String::from("a"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r14 = &Rate{
        id: 14,
        from: String::from("d"),
        to: String::from("b"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r15 = &Rate{
        id: 15,
        from: String::from("d"),
        to: String::from("c"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r16 = &Rate{
        id: 16,
        from: String::from("d"),
        to: String::from("e"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r17 = &Rate{
        id: 17,
        from: String::from("e"),
        to: String::from("a"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r18 = &Rate{
        id: 18,
        from: String::from("e"),
        to: String::from("b"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r19 = &Rate{
        id: 19,
        from: String::from("e"),
        to: String::from("c"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let r20 = &Rate{
        id: 20,
        from: String::from("e"),
        to: String::from("d"),
        exchange: String::from("j"),
        rate: 2.0,
        vol: 1.0,
    };
    let l = vec![r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15, r16, r17, r18, r19, r20];

    // note: don't want rust to optimize away by not using output
    //let mut tmp: Vec<Vec<Vec<&Rate>>> = Vec::new();
    b.iter(|| {
        //tmp = arb_from_rates(&l, 5);
        arb_from_rates(&l, 5);
    });
    //println!("tmp len: {}", tmp.len());
}
