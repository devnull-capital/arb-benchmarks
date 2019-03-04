package arbitrage

import (
	"fmt"
	"testing"
)

func TestIsRateInList(t *testing.T) {
	r := &rate{
		from:     "foo",
		to:       "bar",
		exchange: "baz",
		vol:      1.0,
		rate:     1.0,
	}

	for i, tt := range []struct {
		list     []*rate
		r        *rate
		expected bool
	}{
		{
			list: []*rate{
				&rate{
					from:     "foo",
					to:       "bar",
					exchange: "baz",
					vol:      1.0,
					rate:     1.0,
				},
				&rate{
					from:     "foo",
					to:       "bar",
					exchange: "baz",
					vol:      1.0,
					rate:     1.0,
				},
			},
			r: &rate{
				from:     "foo",
				to:       "bar",
				exchange: "baz",
				vol:      1.0,
				rate:     1.0,
			},
			expected: false,
		},
		{
			list: []*rate{
				&rate{
					from:     "foo",
					to:       "bar",
					exchange: "baz",
					vol:      1.0,
					rate:     1.0,
				},
				&rate{
					from:     "foo",
					to:       "bar",
					exchange: "baz",
					vol:      1.0,
					rate:     1.0,
				},
				r,
			},
			r:        r,
			expected: true,
		},
	} {
		t.Run(fmt.Sprintf("%v", i), func(t *testing.T) {
			if isRateInList(tt.list, tt.r) != tt.expected {
				t.Errorf("expected %v from rate %v in list %v", tt.expected, tt.r, tt.list)
			}
		})
	}
}

func TestBuildBase(t *testing.T) {
	t.Run("should not build any when the to's and from's don't match", func(t *testing.T) {
		rates := []*rate{
			&rate{},
			&rate{
				to:   "foo",
				from: "foo",
			},
		}
		r := buildBase(rates)
		if len(r) != 0 {
			t.Errorf("expected zero length result, received %d for %v", len(r), r)
		}
	})
	t.Run("should build on proper input", func(t *testing.T) {
		rates := []*rate{
			&rate{
				from: "a",
				to:   "b",
			},
			&rate{
				from: "b",
				to:   "c",
			},
			&rate{
				from: "c",
				to:   "d",
			},
			&rate{
				from: "y",
				to:   "z",
			},
		}
		r := buildBase(rates)
		if len(r) != 2 || len(r[0]) != 2 || len(r[1]) != 2 {
			t.Errorf("expected two length result with three sub-length, received %d for %v", len(r), r)
		}
	})
}

func TestCombosFromRates(t *testing.T) {
	t.Run("should build on proper input", func(t *testing.T) {
		rates := []*rate{
			&rate{
				from: "a",
				to:   "b",
			},
			&rate{
				from: "b",
				to:   "c",
			},
			&rate{
				from: "c",
				to:   "d",
			},
			&rate{
				from: "y",
				to:   "z",
			},
		}
		r := combosFromRates(rates, 4)
		if len(r) != 4 || len(r[0]) != 2 || len(r[0][0]) != 2 || len(r[0][1]) != 2 || len(r[1]) != 1 || len(r[1][0]) != 3 {
			t.Errorf("expected two length result with three sub-length, received %d for %v", len(r), r)
		}
	})
}

func TestIsArb(t *testing.T) {
	for i, tt := range []struct {
		list     []*rate
		expected bool
	}{
		{
			list: []*rate{
				&rate{},
			},
			expected: false,
		},
		{
			list: []*rate{
				&rate{
					from: "a",
					to:   "b",
				},
				&rate{
					from: "y",
					to:   "z",
				},
			},
			expected: false,
		},
		{
			list: []*rate{
				&rate{
					from: "a",
					to:   "b",
				},
				&rate{
					from: "b",
					to:   "c",
				},
			},
			expected: false,
		},
		{
			list: []*rate{
				&rate{
					from: "a",
					to:   "b",
					rate: 2.0,
				},
				&rate{
					from: "b",
					to:   "c",
					rate: 2.0,
				},
			},
			expected: false,
		},
		{
			list: []*rate{
				&rate{
					from: "a",
					to:   "b",
					rate: 2.0,
				},
				&rate{
					from: "b",
					to:   "c",
					rate: 2.0,
				},
				&rate{
					from: "c",
					to:   "a",
					rate: 2.0,
				},
			},
			expected: true,
		},
		{
			list: []*rate{
				&rate{
					from: "a",
					to:   "b",
					rate: 1.0,
				},
				&rate{
					from: "b",
					to:   "c",
					rate: 1.0,
				},
				&rate{
					from: "c",
					to:   "a",
					rate: 1.0,
				},
			},
			expected: false,
		},
	} {
		t.Run(fmt.Sprintf("%v", i), func(t *testing.T) {
			if isArb(tt.list) != tt.expected {
				t.Errorf("expected %v from list %v", tt.expected, tt.list)
			}
		})
	}
}

func TestArbFromCombos(t *testing.T) {
	t.Run("good rates should produce arb", func(t *testing.T) {
		rates := []*rate{
			&rate{
				from: "a",
				to:   "b",
				rate: 2.0,
			},
			&rate{
				from: "b",
				to:   "c",
				rate: 2.0,
			},
			&rate{
				from: "c",
				to:   "a",
				rate: 2.0,
			},
			&rate{
				from: "y",
				to:   "z",
				rate: 2.0,
			},
		}
		arb := arbFromCombos(combosFromRates(rates, 4))
		if len(arb) != 4 || len(arb[0]) != 0 || len(arb[1]) != 1 || len(arb[1][0]) != 3 || len(arb[2]) != 0 || len(arb[3]) != 0 {
			t.Errorf("returned bad arb\n%v", arb)
		}
	})
}

func TestIsDupe(t *testing.T) {
	r1 := &rate{}
	r2 := &rate{}
	r3 := &rate{}
	arb := []*rate{r1, r2, r3}

	for i, tt := range []struct {
		list     [][]*rate
		arb      []*rate
		expected bool
	}{
		{
			list: [][]*rate{
				[]*rate{
					&rate{},
					&rate{},
				},
				arb,
			},
			arb:      arb,
			expected: true,
		},
		{
			list: [][]*rate{
				[]*rate{
					&rate{},
					&rate{},
				},
				[]*rate{
					&rate{},
					&rate{},
				},
			},
			arb:      arb,
			expected: false,
		},
	} {
		t.Run(fmt.Sprintf("%v", i), func(t *testing.T) {
			if isDupe(tt.list, tt.arb) != tt.expected {
				t.Errorf("expected %v with list %v and arb %v", tt.expected, tt.list, tt.arb)
			}
		})
	}
}

func TestIsListClosing(t *testing.T) {
	for i, tt := range []struct {
		list     []*rate
		expected bool
	}{
		{
			list: []*rate{
				&rate{
					from: "a",
					to:   "b",
				},
				&rate{
					from: "b",
					to:   "a",
				},
			},
			expected: true,
		},
		{
			list: []*rate{
				&rate{
					from: "a",
					to:   "b",
				},
				&rate{
					from: "b",
					to:   "c",
				},
				&rate{
					from: "y",
					to:   "z",
				},
			},
			expected: false,
		},
	} {
		t.Run(fmt.Sprintf("%v", i), func(t *testing.T) {
			if tt.expected != isListClosing(tt.list) {
				t.Errorf("expected %v from list %v", tt.expected, tt.list)
			}
		})
	}

}

func BenchmarkArbFromRates(b *testing.B) {
	rates := []*rate{
		&rate{
			from: "a",
			to:   "b",
			rate: 2.0,
		},
		&rate{
			from: "b",
			to:   "c",
			rate: 2.0,
		},
		&rate{
			from: "c",
			to:   "d",
			rate: 2.0,
		},
		&rate{
			from: "d",
			to:   "e",
			rate: 2.0,
		},
		&rate{
			from: "b",
			to:   "a",
			rate: 2.0,
		},
		&rate{
			from: "c",
			to:   "a",
			rate: 2.0,
		},
		&rate{
			from: "d",
			to:   "a",
			rate: 2.0,
		},
		&rate{
			from: "e",
			to:   "a",
			rate: 2.0,
		},
		&rate{
			from: "c",
			to:   "b",
			rate: 2.0,
		},
		&rate{
			from: "d",
			to:   "b",
			rate: 2.0,
		},
		&rate{
			from: "e",
			to:   "b",
			rate: 2.0,
		},
		&rate{
			from: "d",
			to:   "c",
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

	for i := 0; i < b.N; i++ {
		_ = arbFromRates(rates, 10)
	}
}
