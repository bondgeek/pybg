# distutils: language = c++
# not using distutils for libraries, Visual Studio auto-linking doesn't like

include '../../types.pxi'
cimport pybg.version

from libcpp cimport bool
from libcpp.vector cimport vector
from libcpp.string cimport string

from pybg.quantlib.handle cimport shared_ptr, Handle, RelinkableHandle
from pybg.quantlib.time._calendar cimport Calendar
from pybg.quantlib.time._date cimport Date
from pybg.quantlib.time._daycounter cimport DayCounter
from pybg.quantlib.time._period cimport Frequency
from pybg.quantlib.termstructures.yields._flat_forward cimport YieldTermStructure

cdef extern from 'ql/termstructures/yield/ratehelpers.hpp' namespace 'QuantLib':

    cdef cppclass RateHelper:
        pass

cdef extern from '_piecewise_support_code.hpp' namespace 'QuantLib':

    cdef shared_ptr[YieldTermStructure] term_structure_factory(
        string& traits,
        string& interpolator,
        Date& settlement_date,
        vector[shared_ptr[RateHelper]]& curve_input,
        DayCounter& day_counter,
        Real tolerance
    ) except +

cdef extern from 'ql/termstructures/yield/piecewiseyieldcurve.hpp' namespace 'QuantLib':

    cdef cppclass PiecewiseYieldcurve[Traits, Interpolator]:
        PiecewiseYieldCurve()
        # Constructurors are not supported through the Cython syntax !
        #PiecewiseYieldCurve(
        #    Date& referenceDate,
        #    #vector[shared_ptr[Traits::helper]]& instruments,
        #    DayCounter& dayCounter,
        #    Real accuracy,
        #    #Interpolator& i,
        #    #Bootstrap<this_curve>& bootstrap
        #)
