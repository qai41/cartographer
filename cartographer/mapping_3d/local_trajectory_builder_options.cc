/*
 * Copyright 2016 The Cartographer Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "cartographer/mapping_3d/local_trajectory_builder_options.h"

#include "cartographer/mapping_3d/kalman_local_trajectory_builder_options.h"
#include "cartographer/mapping_3d/motion_filter.h"
#include "cartographer/mapping_3d/optimizing_local_trajectory_builder_options.h"
#include "cartographer/mapping_3d/scan_matching/ceres_scan_matcher.h"
#include "cartographer/mapping_3d/submaps.h"
#include "cartographer/sensor/voxel_filter.h"
#include "glog/logging.h"

namespace cartographer {
namespace mapping_3d {

proto::LocalTrajectoryBuilderOptions CreateLocalTrajectoryBuilderOptions(
    common::LuaParameterDictionary* const parameter_dictionary) {
  proto::LocalTrajectoryBuilderOptions options;
  options.set_min_range(parameter_dictionary->GetDouble("min_range"));
  options.set_max_range(parameter_dictionary->GetDouble("max_range"));
  options.set_scans_per_accumulation(
      parameter_dictionary->GetInt("scans_per_accumulation"));
  options.set_voxel_filter_size(
      parameter_dictionary->GetDouble("voxel_filter_size"));
  *options.mutable_high_resolution_adaptive_voxel_filter_options() =
      sensor::CreateAdaptiveVoxelFilterOptions(
          parameter_dictionary
              ->GetDictionary("high_resolution_adaptive_voxel_filter")
              .get());
  *options.mutable_low_resolution_adaptive_voxel_filter_options() =
      sensor::CreateAdaptiveVoxelFilterOptions(
          parameter_dictionary
              ->GetDictionary("low_resolution_adaptive_voxel_filter")
              .get());
  *options.mutable_ceres_scan_matcher_options() =
      scan_matching::CreateCeresScanMatcherOptions(
          parameter_dictionary->GetDictionary("ceres_scan_matcher").get());
  *options.mutable_motion_filter_options() = CreateMotionFilterOptions(
      parameter_dictionary->GetDictionary("motion_filter").get());
  *options.mutable_submaps_options() = mapping_3d::CreateSubmapsOptions(
      parameter_dictionary->GetDictionary("submaps").get());
  *options.mutable_kalman_local_trajectory_builder_options() =
      CreateKalmanLocalTrajectoryBuilderOptions(
          parameter_dictionary->GetDictionary("kalman_local_trajectory_builder")
              .get());
  *options.mutable_optimizing_local_trajectory_builder_options() =
      CreateOptimizingLocalTrajectoryBuilderOptions(
          parameter_dictionary
              ->GetDictionary("optimizing_local_trajectory_builder")
              .get());
  const string use_string = parameter_dictionary->GetString("use");
  proto::LocalTrajectoryBuilderOptions::Use use;
  CHECK(proto::LocalTrajectoryBuilderOptions::Use_Parse(use_string, &use))
      << "Unknown local_trajectory_builder kind: " << use_string;
  options.set_use(use);
  return options;
}

}  // namespace mapping_3d
}  // namespace cartographer
