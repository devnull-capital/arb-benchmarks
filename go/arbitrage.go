package main

import (
	"log"
	"runtime"
)

func main() {
	rates := []*rate{
		&rate{
			from: "a",
			to:   "b",
			rate: 2.0,
		},
		&rate{
			from: "a",
			to:   "c",
			rate: 2.0,
		},
		&rate{
			from: "a",
			to:   "d",
			rate: 2.0,
		},
		&rate{
			from: "a",
			to:   "e",
			rate: 2.0,
		},
		&rate{
			from: "b",
			to:   "a",
			rate: 2.0,
		},
		&rate{
			from: "b",
			to:   "c",
			rate: 2.0,
		},
		&rate{
			from: "b",
			to:   "d",
			rate: 2.0,
		},
		&rate{
			from: "b",
			to:   "e",
			rate: 2.0,
		},
		&rate{
			from: "c",
			to:   "a",
			rate: 2.0,
		},
		&rate{
			from: "c",
			to:   "b",
			rate: 2.0,
		},
		&rate{
			from: "c",
			to:   "d",
			rate: 2.0,
		},
		&rate{
			from: "c",
			to:   "e",
			rate: 2.0,
		},
		&rate{
			from: "d",
			to:   "a",
			rate: 2.0,
		},
		&rate{
			from: "d",
			to:   "b",
			rate: 2.0,
		},
		&rate{
			from: "d",
			to:   "c",
			rate: 2.0,
		},
		&rate{
			from: "d",
			to:   "e",
			rate: 2.0,
		},
		&rate{
			from: "e",
			to:   "a",
			rate: 2.0,
		},
		&rate{
			from: "e",
			to:   "b",
			rate: 2.0,
		},
		&rate{
			from: "e",
			to:   "c",
			rate: 2.0,
		},
		&rate{
			from: "e",
			to:   "d",
			rate: 2.0,
		},
	}

	_ = runtime.GOMAXPROCS(1)
	var tmp [][][]*rate
	for i := 0; i < 100; i++ {
		tmp = arbFromRates(rates, 5)
	}
	log.Printf("tmp len: %d", len(tmp))
}

type rate struct {
	from, to, exchange string
	rate, vol          float64
}

func arbFromRates(rates []*rate, depth uint) [][][]*rate {
	return arbFromCombos(combosFromRates(rates, depth))
}

func arbFromCombos(combos [][][]*rate) [][][]*rate {
	ret := make([][][]*rate, len(combos))
	for i := range combos {
		for j := range combos[i] {
			if isArb(combos[i][j]) && !isDupe(ret[i], combos[i][j]) {
				ret[i] = append(ret[i], combos[i][j])
			}
		}
	}

	return ret
}

func isDupe(list [][]*rate, arb []*rate) bool {
	if list == nil || len(list) == 0 {
		return false
	}

	m := make(map[*rate]struct{})
	for i := range arb {
		m[arb[i]] = struct{}{}
	}

	var (
		count int
		ok    bool
	)
	for i := range list {
		count = 0
		for j := range list[i] {
			if _, ok = m[list[i][j]]; ok {
				count++
			}
		}

		if count == len(list[i]) {
			return true
		}
	}

	return false
}

func isArb(list []*rate) bool {
	if len(list) < 2 {
		return false
	}

	if list[0].from != list[len(list)-1].to {
		return false
	}

	prod := list[0].rate
	for i := 1; i < len(list); i++ {
		if list[i-1].to != list[i].from {
			return false
		}

		prod *= list[i].rate
	}

	return prod > 1.0
}

func combosFromRates(rates []*rate, depth uint) [][][]*rate {
	// note: no safety checks
	//if rates == nil || len(rates) < 2 {
	//return nil, errors.New("not enough rates")
	//}

	//if depth < 1 {
	//return nil, errors.New("min depth is 1")
	//}

	ret := make([][][]*rate, int(depth))
	ret[0] = buildBase(rates)
	for i := 1; i < int(depth); i++ {
		for j := range ret[i-1] {
			for k := range rates {
				if ret[i-1][j][len(ret[i-1][j])-1].to == rates[k].from && !isRateInList(ret[i-1][j], rates[k]) && !isListClosing(ret[i-1][j]) {
					var tmp []*rate
					for z := range ret[i-1][j] {
						tmp = append(tmp, ret[i-1][j][z])
					}
					tmp = append(tmp, rates[k])
					ret[i] = append(ret[i], tmp)
				}
			}
		}
	}

	return ret
}

func isListClosing(list []*rate) bool {
	return list[0].from == list[len(list)-1].to
}

func buildBase(rates []*rate) [][]*rate {
	var ret [][]*rate
	for i := range rates {
		for j := i + 1; j < len(rates); j++ {
			if rates[i].to == rates[j].from {
				ret = append(ret, []*rate{rates[i], rates[j]})
			}
		}
	}

	return ret
}

func isRateInList(list []*rate, r *rate) bool {
	// note: not doing nil checks for speed reasons...
	for idx := range list {
		if list[idx] == r {
			return true
		}
	}

	return false
}
