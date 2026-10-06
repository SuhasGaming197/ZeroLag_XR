#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

extern "C" {
    void InitFilter();
    void UpdateFilter(float x, float y, float z, float dt);
    void PredictFuture(float forward_time_ms, float* out_x, float* out_y, float* out_z);
}

py::tuple Predict(float forward_time_ms) {
    float x, y, z;
    PredictFuture(forward_time_ms, &x, &y, &z);
    return py::make_tuple(x, y, z);
}

PYBIND11_MODULE(zerolag, m) {
    m.doc() = "ZeroLag XR predictive filter pybind11 wrapper";
    m.def("init_filter", &InitFilter, "Initialize the predictive filter");
    m.def("update_filter", &UpdateFilter, "Update the filter with new coordinates");
    m.def("predict_future", &Predict, "Predict future coordinates");
}
