// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#if !defined(ORT_MINIMAL_BUILD) || defined(ORT_EXTENDED_MINIMAL_BUILD)

#include "core/framework/kernel_type_str_resolver_utils.h"

#include "flatbuffers/flatbuffers.h"

#include "core/common/common.h"
#include "core/flatbuffers/schema/ort.fbs.h"
#include "core/optimizer/transpose_optimization/layout_transformation_potentially_added_ops.h"

namespace onnxruntime::kernel_type_str_resolver_utils {

static constexpr auto* kStandaloneKernelTypeStrResolverFileIdentifier = "ktsr";

#if !defined(ORT_MINIMAL_BUILD)

gsl::span<const OpIdentifierWithStringViews> GetLayoutTransformationRequiredOpIdentifiers() {
  return kLayoutTransformationPotentiallyAddedOps;
}

Status SaveKernelTypeStrResolverToBuffer(const KernelTypeStrResolver& kernel_type_str_resolver,
                                         flatbuffers::DetachedBuffer& buffer, gsl::span<const uint8_t>& buffer_span) {
  flatbuffers::FlatBufferBuilder builder;
  flatbuffers::Offset<fbs::KernelTypeStrResolver> fbs_kernel_type_str_resolver;
  ORT_RETURN_IF_ERROR(kernel_type_str_resolver.SaveToOrtFormat(builder, fbs_kernel_type_str_resolver));
  builder.Finish(fbs_kernel_type_str_resolver, kStandaloneKernelTypeStrResolverFileIdentifier);
  buffer = builder.Release();
  buffer_span = gsl::make_span(buffer.data(), buffer.size());
  return Status::OK();
}

#endif  // !defined(ORT_MINIMAL_BUILD)

Status LoadKernelTypeStrResolverFromBuffer(KernelTypeStrResolver& kernel_type_str_resolver,
                                           gsl::span<const uint8_t> buffer_span) {
  flatbuffers::Verifier verifier{buffer_span.data(), buffer_span.size_bytes()};
  ORT_RETURN_IF_NOT(verifier.VerifyBuffer<fbs::KernelTypeStrResolver>(kStandaloneKernelTypeStrResolverFileIdentifier),
                    "Failed to verify KernelTypeStrResolver flatbuffers data.");
  const auto* fbs_kernel_type_str_resolver = flatbuffers::GetRoot<fbs::KernelTypeStrResolver>(buffer_span.data());
  ORT_RETURN_IF_ERROR(kernel_type_str_resolver.LoadFromOrtFormat(*fbs_kernel_type_str_resolver));
  return Status::OK();
}

Status AddLayoutTransformationRequiredOpsToKernelTypeStrResolver(KernelTypeStrResolver& kernel_type_str_resolver) {
  KernelTypeStrResolver resolver_with_required_ops{};

  // to generate kLayoutTransformationRequiredOpsKernelTypeStrResolverBytes, run the test:
  //   KernelTypeStrResolverUtilsTest.DISABLED_PrintExpectedLayoutTransformationRequiredOpsResolverByteArray

  // clang-format off
  constexpr uint8_t kLayoutTransformationRequiredOpsKernelTypeStrResolverBytes[] = {
      0x10, 0x00, 0x00, 0x00, 0x6b, 0x74, 0x73, 0x72, 0x00, 0x00, 0x06, 0x00, 0x08, 0x00, 0x04, 0x00,
      0x06, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0xfc, 0x04, 0x00, 0x00,
      0x48, 0x06, 0x00, 0x00, 0xac, 0x06, 0x00, 0x00, 0xa4, 0x05, 0x00, 0x00, 0x2c, 0x03, 0x00, 0x00,
      0xd0, 0x01, 0x00, 0x00, 0xe0, 0x05, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x10, 0x02, 0x00, 0x00,
      0x58, 0x01, 0x00, 0x00, 0x08, 0x01, 0x00, 0x00, 0x38, 0x05, 0x00, 0x00, 0xc0, 0x02, 0x00, 0x00,
      0xb0, 0x00, 0x00, 0x00, 0x40, 0x02, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x48, 0x03, 0x00, 0x00,
      0x30, 0xf9, 0xff, 0xff, 0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
      0x14, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x3a, 0x53, 0x71, 0x75, 0x65, 0x65, 0x7a, 0x65,
      0x3a, 0x31, 0x00, 0x00, 0x54, 0xf9, 0xff, 0xff, 0x7c, 0x06, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
      0x02, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x82, 0xf9, 0xff, 0xff,
      0x00, 0x00, 0x00, 0x01, 0x7c, 0xf9, 0xff, 0xff, 0x78, 0xf9, 0xff, 0xff, 0x10, 0x00, 0x00, 0x00,
      0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x1b, 0x00, 0x00, 0x00,
      0x63, 0x6f, 0x6d, 0x2e, 0x6d, 0x69, 0x63, 0x72, 0x6f, 0x73, 0x6f, 0x66, 0x74, 0x3a, 0x4e, 0x68,
      0x77, 0x63, 0x4d, 0x61, 0x78, 0x50, 0x6f, 0x6f, 0x6c, 0x3a, 0x31, 0x00, 0xac, 0xf9, 0xff, 0xff,
      0x24, 0x06, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
      0x04, 0x00, 0x00, 0x00, 0xda, 0xf9, 0xff, 0xff, 0x00, 0x00, 0x00, 0x01, 0xd4, 0xf9, 0xff, 0xff,
      0xd0, 0xf9, 0xff, 0xff, 0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
      0x18, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x3a, 0x55, 0x6e, 0x73, 0x71, 0x75, 0x65, 0x65,
      0x7a, 0x65, 0x3a, 0x31, 0x31, 0x00, 0x00, 0x00, 0xf8, 0xf9, 0xff, 0xff, 0xd8, 0x05, 0x00, 0x00,
      0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
      0x26, 0xfa, 0xff, 0xff, 0x00, 0x00, 0x00, 0x01, 0x20, 0xfa, 0xff, 0xff, 0x1c, 0xfa, 0xff, 0xff,
      0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00,
      0x0c, 0x00, 0x00, 0x00, 0x3a, 0x54, 0x72, 0x61, 0x6e, 0x73, 0x70, 0x6f, 0x73, 0x65, 0x3a, 0x31,
      0x00, 0x00, 0x00, 0x00, 0x44, 0xfa, 0xff, 0xff, 0x8c, 0x05, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
      0x02, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x72, 0xfa, 0xff, 0xff,
      0x00, 0x00, 0x00, 0x01, 0x6c, 0xfa, 0xff, 0xff, 0x68, 0xfa, 0xff, 0xff, 0x14, 0x00, 0x00, 0x00,
      0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00,
      0x0b, 0x00, 0x00, 0x00, 0x3a, 0x53, 0x71, 0x75, 0x65, 0x65, 0x7a, 0x65, 0x3a, 0x31, 0x33, 0x00,
      0x90, 0xfa, 0xff, 0xff, 0x40, 0x05, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
      0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0xbe, 0xfa, 0xff, 0xff, 0x00, 0x00, 0x00, 0x01,
      0xb8, 0xfa, 0xff, 0xff, 0xb4, 0xfa, 0xff, 0xff, 0xe4, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
      0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0xa0, 0xfa, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00,
      0xd0, 0xfa, 0xff, 0xff, 0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
      0x18, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x3a, 0x49, 0x64, 0x65, 0x6e, 0x74, 0x69, 0x74,
      0x79, 0x3a, 0x31, 0x34, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xfa, 0xff, 0xff, 0x1c, 0x04, 0x00, 0x00,
      0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
      0x26, 0xfb, 0xff, 0xff, 0x00, 0x00, 0x00, 0x01, 0x20, 0xfb, 0xff, 0xff, 0x1c, 0xfb, 0xff, 0xff,
      0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00,
      0x0b, 0x00, 0x00, 0x00, 0x3a, 0x53, 0x71, 0x75, 0x65, 0x65, 0x7a, 0x65, 0x3a, 0x31, 0x31, 0x00,
      0x40, 0xfb, 0xff, 0xff, 0x90, 0x04, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
      0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x6e, 0xfb, 0xff, 0xff, 0x00, 0x00, 0x00, 0x01,
      0x68, 0xfb, 0xff, 0xff, 0x64, 0xfb, 0xff, 0xff, 0x14, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
      0x02, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00,
      0x3a, 0x55, 0x6e, 0x73, 0x71, 0x75, 0x65, 0x65, 0x7a, 0x65, 0x3a, 0x31, 0x33, 0x00, 0x00, 0x00,
      0x90, 0xfb, 0xff, 0xff, 0x08, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
      0x61, 0x78, 0x65, 0x73, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
      0x88, 0xfb, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0xb8, 0xfb, 0xff, 0xff, 0x18, 0x04, 0x00, 0x00,
      0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
      0xe6, 0xfb, 0xff, 0xff, 0x00, 0x00, 0x00, 0x01, 0xe0, 0xfb, 0xff, 0xff, 0xdc, 0xfb, 0xff, 0xff,
      0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00,
      0x0c, 0x00, 0x00, 0x00, 0x3a, 0x55, 0x6e, 0x73, 0x71, 0x75, 0x65, 0x65, 0x7a, 0x65, 0x3a, 0x31,
      0x00, 0x00, 0x00, 0x00, 0x04, 0xfc, 0xff, 0xff, 0xcc, 0x03, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
      0x02, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x32, 0xfc, 0xff, 0xff,
      0x00, 0x00, 0x00, 0x01, 0x2c, 0xfc, 0xff, 0xff, 0x28, 0xfc, 0xff, 0xff, 0x10, 0x00, 0x00, 0x00,
      0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00,
      0x3a, 0x49, 0x64, 0x65, 0x6e, 0x74, 0x69, 0x74, 0x79, 0x3a, 0x31, 0x33, 0x00, 0x00, 0x00, 0x00,
      0x50, 0xfc, 0xff, 0xff, 0x80, 0x03, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
      0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x7e, 0xfc, 0xff, 0xff, 0x00, 0x00, 0x00, 0x01,
      0x78, 0xfc, 0xff, 0xff, 0x74, 0xfc, 0xff, 0xff, 0x28, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
      0x07, 0x00, 0x00, 0x00, 0x94, 0x00, 0x00, 0x00, 0xbc, 0x00, 0x00, 0x00, 0x5c, 0x00, 0x00, 0x00,
      0xe4, 0x00, 0x00, 0x00, 0x04, 0x01, 0x00, 0x00, 0x28, 0x01, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00,
      0x1b, 0x00, 0x00, 0x00, 0x63, 0x6f, 0x6d, 0x2e, 0x6d, 0x69, 0x63, 0x72, 0x6f, 0x73, 0x6f, 0x66,
      0x74, 0x3a, 0x51, 0x4c, 0x69, 0x6e, 0x65, 0x61, 0x72, 0x43, 0x6f, 0x6e, 0x76, 0x3a, 0x31, 0x00,
      0xc0, 0xfc, 0xff, 0xff, 0x08, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00,
      0x79, 0x5f, 0x73, 0x63, 0x61, 0x6c, 0x65, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
      0xb8, 0xfc, 0xff, 0xff, 0x06, 0x00, 0x00, 0x00, 0xe8, 0xfc, 0xff, 0xff, 0x08, 0x00, 0x00, 0x00,
      0x0c, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x54, 0x33, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
      0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x1e, 0xfd, 0xff, 0xff, 0x00, 0x00, 0x00, 0x01,
      0xe8, 0xfc, 0xff, 0xff, 0x07, 0x00, 0x00, 0x00, 0x18, 0xfd, 0xff, 0xff, 0x08, 0x00, 0x00, 0x00,
      0x0c, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x54, 0x31, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
      0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x10, 0xfd, 0xff, 0xff, 0x02, 0x00, 0x00, 0x00,
      0x48, 0xfd, 0xff, 0xff, 0x44, 0xfd, 0xff, 0xff, 0x08, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00,
      0x02, 0x00, 0x00, 0x00, 0x54, 0x32, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
      0x04, 0x00, 0x00, 0x00, 0x3c, 0xfd, 0xff, 0xff, 0x05, 0x00, 0x00, 0x00, 0x44, 0xfd, 0xff, 0xff,
      0x03, 0x00, 0x00, 0x00, 0x74, 0xfd, 0xff, 0xff, 0x08, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00,
      0x02, 0x00, 0x00, 0x00, 0x54, 0x34, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
      0x68, 0xfd, 0xff, 0xff, 0x08, 0x00, 0x00, 0x00, 0x98, 0xfd, 0xff, 0xff, 0x08, 0x00, 0x00, 0x00,
      0x10, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x77, 0x5f, 0x73, 0x63, 0x61, 0x6c, 0x65, 0x00,
      0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x90, 0xfd, 0xff, 0xff, 0x04, 0x00, 0x00, 0x00,
      0xc0, 0xfd, 0xff, 0xff, 0x08, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00,
      0x78, 0x5f, 0x73, 0x63, 0x61, 0x6c, 0x65, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
      0xb8, 0xfd, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0xe8, 0xfd, 0xff, 0xff, 0x14, 0x00, 0x00, 0x00,
      0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00,
      0x09, 0x00, 0x00, 0x00, 0x3a, 0x47, 0x61, 0x74, 0x68, 0x65, 0x72, 0x3a, 0x31, 0x00, 0x00, 0x00,
      0x10, 0xfe, 0xff, 0xff, 0x00, 0x02, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
      0x04, 0x00, 0x00, 0x00, 0xfc, 0xfd, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x2c, 0xfe, 0xff, 0xff,
      0xa4, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
      0x04, 0x00, 0x00, 0x00, 0x5a, 0xfe, 0xff, 0xff, 0x00, 0x00, 0x00, 0x01, 0x54, 0xfe, 0xff, 0xff,
      0x50, 0xfe, 0xff, 0xff, 0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
      0x18, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x3a, 0x54, 0x72, 0x61, 0x6e, 0x73, 0x70, 0x6f,
      0x73, 0x65, 0x3a, 0x31, 0x33, 0x00, 0x00, 0x00, 0x78, 0xfe, 0xff, 0xff, 0x58, 0x01, 0x00, 0x00,
      0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
      0xa6, 0xfe, 0xff, 0xff, 0x00, 0x00, 0x00, 0x01, 0xa0, 0xfe, 0xff, 0xff, 0x9c, 0xfe, 0xff, 0xff,
      0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00,
      0x0b, 0x00, 0x00, 0x00, 0x3a, 0x49, 0x64, 0x65, 0x6e, 0x74, 0x69, 0x74, 0x79, 0x3a, 0x31, 0x00,
      0xc0, 0xfe, 0xff, 0xff, 0x10, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
      0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0xee, 0xfe, 0xff, 0xff, 0x00, 0x00, 0x00, 0x01,
      0xe8, 0xfe, 0xff, 0xff, 0xe4, 0xfe, 0xff, 0xff, 0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
      0x01, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x3a, 0x49, 0x64, 0x65,
      0x6e, 0x74, 0x69, 0x74, 0x79, 0x3a, 0x31, 0x36, 0x00, 0x00, 0x00, 0x00, 0x0c, 0xff, 0xff, 0xff,
      0x08, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x56, 0x00, 0x00, 0x00,
      0x02, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x42, 0xff, 0xff, 0xff,
      0x00, 0x00, 0x00, 0x01, 0x3c, 0xff, 0xff, 0xff, 0x38, 0xff, 0xff, 0xff, 0x14, 0x00, 0x00, 0x00,
      0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00,
      0x0a, 0x00, 0x00, 0x00, 0x3a, 0x47, 0x61, 0x74, 0x68, 0x65, 0x72, 0x3a, 0x31, 0x31, 0x00, 0x00,
      0x60, 0xff, 0xff, 0xff, 0xb0, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
      0x04, 0x00, 0x00, 0x00, 0x4c, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x7c, 0xff, 0xff, 0xff,
      0x54, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
      0x04, 0x00, 0x00, 0x00, 0xaa, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x01, 0xa4, 0xff, 0xff, 0xff,
      0xa0, 0xff, 0xff, 0xff, 0x14, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
      0x18, 0x00, 0x00, 0x00, 0x54, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x3a, 0x47, 0x61, 0x74,
      0x68, 0x65, 0x72, 0x3a, 0x31, 0x33, 0x00, 0x00, 0xc8, 0xff, 0xff, 0xff, 0x08, 0x00, 0x00, 0x00,
      0x0c, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x54, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
      0x1c, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x08, 0x00, 0x07, 0x00,
      0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00,
      0x08, 0x00, 0x0c, 0x00, 0x04, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
      0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x54, 0x69, 0x6e, 0x64, 0x00, 0x00, 0x00, 0x00,
      0x01, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 0x00, 0x04, 0x00,
      0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
  };
  // clang-format on

  ORT_RETURN_IF_ERROR(LoadKernelTypeStrResolverFromBuffer(resolver_with_required_ops,
                                                          kLayoutTransformationRequiredOpsKernelTypeStrResolverBytes));
  kernel_type_str_resolver.Merge(std::move(resolver_with_required_ops));
  return Status::OK();
}

}  // namespace onnxruntime::kernel_type_str_resolver_utils

#endif  // !defined(ORT_MINIMAL_BUILD) || defined(ORT_EXTENDED_MINIMAL_BUILD)
