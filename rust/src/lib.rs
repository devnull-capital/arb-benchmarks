#![feature(test)]

extern crate test;
use std::collections::HashMap;
use test::Bencher;

#[derive(Debug)]
pub struct Rate {
    id: u8,
    from: String,
    to: String,
    exchange: String,
    rate: f64,
    vol: f64,
}

pub fn arb_from_rates<'a>(rates: Vec<&'a Rate>, depth: u32) -> Vec<Vec<Vec<&'a Rate>>> {
    return arb_from_combos(combos_from_rates(rates, depth))
}

fn arb_from_combos<'a>(combos: Vec<Vec<Vec<&'a Rate>>>) -> Vec<Vec<Vec<&'a Rate>>> {
    let mut ret: Vec<Vec<Vec<&Rate>>> = Vec::new();

    for i in combos {
        let mut tmp: Vec<Vec<&Rate>> = Vec::new();
        for j in i {
            if is_arb(&j) && !is_dupe(&tmp, &j) {
                tmp.push(j);
            }
        }

        ret.push(tmp);
    }

    return ret
}

fn is_dupe<'a>(list: &'a Vec<Vec<&'a Rate>>, arb: &'a Vec<&'a Rate>) -> bool {
    if list.len() == 0 {
        return false
    }

    let mut m: HashMap<u8, bool> = HashMap::new();

    for i in 0..arb.len() {
        m.insert(
            arb[i].id,
            true
        );
    }

    let mut count = 0;
    for i in 0..list.len() {
        for j in 0..list[i].len() {
            if m.contains_key(&list[i][j].id) {
                count = count + 1;
            }
        }

        if count == list[i].len() {
            return true
        }
    }

    return false
}

fn is_arb<'a>(list: &'a Vec<&'a Rate>) -> bool {
    if list.len() < 2 {
        return false
    }

    if list[0].from != list[(list.len()-1) as usize].to {
        return false
    }

    let mut prod = list[0].rate;
    for i in 1..list.len() {
        if list[(i-1) as usize].to != list[i].from {
            return false
        }

        prod *= list[i].rate;
    }

    return prod > 1.0
}

fn combos_from_rates<'a>(rates: Vec<&'a Rate>, depth: u32) -> Vec<Vec<Vec<&'a Rate>>> {
    let mut ret: Vec<Vec<Vec<&Rate>>> = Vec::new();
    //ret.push(build_base(&rates));
    // build base
    let mut base: Vec<Vec<&Rate>> = Vec::new();

    for i in 0..rates.len() {
        for j in (i+1)..rates.len() {
            if rates[i].to == rates[j].from {
                base.push(vec![rates[i], rates[j]]);
            }
        }
    }
    ret.push(base);

    for i in 1..depth {
        let mut tmp: Vec<Vec<&Rate>> = Vec::new();
        for j in 0..ret[(i-1) as usize].len() {
            for k in 0..rates.len() {
                if ret[(i-1) as usize][j as usize][(ret[(i-1) as usize][j as usize].len()-1) as usize].to == rates[k as usize].from && !is_rate_in_list(&ret[(i-1) as usize][j as usize], rates[k as usize]) && !is_list_closing(&ret[(i-1) as usize][j as usize]) {
                    let mut tmp1: Vec<&Rate> = Vec::new();
                    for z in 0..ret[(i-1) as usize][j as usize].len() {
                        tmp1.push(ret[(i-1) as usize][j as usize][z]); 
                    }
                    tmp1.push(rates[k as usize]);
                    tmp.push(tmp1);
                }
            }
        }
        ret.push(tmp);
    }

    return ret
}

fn is_list_closing<'a>(list: &'a Vec<&'a Rate>) -> bool {
    list[0].from == list[list.len()-1].to
}

fn build_base<'a>(rates: &'a Vec<&'a Rate>) -> Vec<Vec<&'a Rate>> {
    let mut ret: Vec<Vec<&Rate>> = Vec::new();

    for i in 0..rates.len() {
        for j in (i+1)..rates.len() {
            if rates[i].to == rates[j].from {
                ret.push(vec![rates[i], rates[j]]);
            }
        }
    }

    return ret
}

fn is_rate_in_list<'a>(list: &'a Vec<&'a Rate>, r: &'a Rate) -> bool {
    // note: not doing nil checks for speed reasons...
    for i in 0..list.len() {
        if list[i].id == r.id {
            return true
        }
    }

    return false
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
    let c1 = combos_from_rates(l1, 4);

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
fn test_arb_from_combos() {
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
    let arb = arb_from_rates(l1, 4);
    assert_eq!(arb.len(), 4);
    assert_eq!(arb[0].len(), 0);
    assert_eq!(arb[1].len(), 1);
    assert_eq!(arb[1][0].len(), 3);
    assert_eq!(arb[2].len(), 0);
    assert_eq!(arb[3].len(), 0);
}

#[bench]
fn bench_arb(b: &mut Bencher) {
    b.iter(|| {
        arb_from_rates(vec![&Rate{
            id: 0,
            from: String::from("a"),
            to: String::from("b"),
            exchange: String::from("j"),
            rate: 2.0,
            vol: 1.0,
        }, &Rate{
            id: 2,
            from: String::from("a"),
            to: String::from("c"),
            exchange: String::from("j"),
            rate: 2.0,
            vol: 1.0,
        }, &Rate{
            id: 3,
            from: String::from("a"),
            to: String::from("d"),
            exchange: String::from("j"),
            rate: 2.0,
            vol: 1.0,
        }, &Rate{
            id: 4,
            from: String::from("a"),
            to: String::from("e"),
            exchange: String::from("j"),
            rate: 2.0,
            vol: 1.0,
        }, &Rate{
            id: 5,
            from: String::from("b"),
            to: String::from("a"),
            exchange: String::from("j"),
            rate: 2.0,
            vol: 1.0,
        }, &Rate{
            id: 6,
            from: String::from("b"),
            to: String::from("c"),
            exchange: String::from("j"),
            rate: 2.0,
            vol: 1.0,
        }, &Rate{
            id: 7,
            from: String::from("b"),
            to: String::from("d"),
            exchange: String::from("j"),
            rate: 2.0,
            vol: 1.0,
        }, &Rate{
            id: 8,
            from: String::from("b"),
            to: String::from("e"),
            exchange: String::from("j"),
            rate: 2.0,
            vol: 1.0,
        }, &Rate{
            id: 9,
            from: String::from("c"),
            to: String::from("a"),
            exchange: String::from("j"),
            rate: 2.0,
            vol: 1.0,
        }, &Rate{
            id: 10,
            from: String::from("c"),
            to: String::from("b"),
            exchange: String::from("j"),
            rate: 2.0,
            vol: 1.0,
        }, &Rate{
            id: 11,
            from: String::from("c"),
            to: String::from("d"),
            exchange: String::from("j"),
            rate: 2.0,
            vol: 1.0,
        }, &Rate{
            id: 12,
            from: String::from("c"),
            to: String::from("e"),
            exchange: String::from("j"),
            rate: 2.0,
            vol: 1.0,
        }, &Rate{
            id: 13,
            from: String::from("d"),
            to: String::from("a"),
            exchange: String::from("j"),
            rate: 2.0,
            vol: 1.0,
        }, &Rate{
            id: 14,
            from: String::from("d"),
            to: String::from("b"),
            exchange: String::from("j"),
            rate: 2.0,
            vol: 1.0,
        }, &Rate{
            id: 15,
            from: String::from("d"),
            to: String::from("c"),
            exchange: String::from("j"),
            rate: 2.0,
            vol: 1.0,
        }, &Rate{
            id: 16,
            from: String::from("d"),
            to: String::from("e"),
            exchange: String::from("j"),
            rate: 2.0,
            vol: 1.0,
        }, &Rate{
            id: 17,
            from: String::from("e"),
            to: String::from("a"),
            exchange: String::from("j"),
            rate: 2.0,
            vol: 1.0,
        }, &Rate{
            id: 18,
            from: String::from("e"),
            to: String::from("b"),
            exchange: String::from("j"),
            rate: 2.0,
            vol: 1.0,
        }, &Rate{
            id: 19,
            from: String::from("e"),
            to: String::from("c"),
            exchange: String::from("j"),
            rate: 2.0,
            vol: 1.0,
        }, &Rate{
            id: 20,
            from: String::from("e"),
            to: String::from("d"),
            exchange: String::from("j"),
            rate: 2.0,
            vol: 1.0,
        }], 5);
    });
}
