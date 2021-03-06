// Adapter for all ops that remove consumed_inputs

#pragma once

#include "onnx/version_converter/adapters/adapter.h"

namespace ONNX_NAMESPACE {
namespace version_conversion {

class Split_12_13 : public Adapter {
 public:
  explicit Split_12_13() : Adapter("Split", OpSetID(12), OpSetID(13)) {}

  void attrToInput(
      std::shared_ptr<Graph> graph,
      Node* node,
      std::vector<int64_t> axes) const {
    Tensor t;
    t.elem_type() = TensorProto_DataType_INT64;
    t.sizes() = std::vector<int64_t>{static_cast<int64_t>(axes.size())};
    auto& data = t.int64s();
    for (auto a : axes) {
      data.emplace_back(a);
    }
    Value* v;
    v = graph->addInitializerAndInput(t);
    node->addInput(v);
  }

  void adapt(std::shared_ptr<Graph> graph, Node* node) const override {
    if (node->hasAttribute(ksplit)){
      attrToInput(graph, node, node->is(ksplit));
      node->removeAttribute(ksplit);
    }
  }
};

} // namespace version_conversion
} // namespace ONNX_NAMESPACE
