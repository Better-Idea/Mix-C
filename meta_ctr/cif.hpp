#pragma once

namespace mixc{
    template<bool condition, class if_true, class if_false>
    struct cif{
        using result = if_true;
    };

    template<class if_true, class if_false>
    struct cif<false, if_true, if_false>{
        using result = if_false;
    };
}
