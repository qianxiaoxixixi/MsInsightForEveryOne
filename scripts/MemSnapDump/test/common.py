"""
-------------------------------------------------------------------------
This file is part of the MindStudio project.
Copyright (c) 2026 Huawei Technologies Co.,Ltd.

MindStudio is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:

         http://license.coscl.org.cn/MulanPSL2

THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details.
-------------------------------------------------------------------------
"""

import unittest
from typing import List
from base import Segment, BlockState


# 校验单个segment是否合法
def valid_segment(segment: Segment, test_util: unittest.TestCase):
    test_util.assertGreaterEqual(segment.active_size, segment.allocated_size)
    test_util.assertGreaterEqual(segment.total_size, segment.active_size)
    allocated = 0
    activated = 0
    for block in segment.blocks:
        test_util.assertGreater(block.size, 0)
        if block.state != BlockState.INACTIVE:
            activated += block.size
            if block.state == BlockState.ACTIVE_ALLOCATED:
                allocated += block.size
        test_util.assertEqual(block.segment_ptr, segment)
    test_util.assertEqual(allocated, segment.allocated_size)
    test_util.assertEqual(activated, segment.active_size)


def valid_segments(segments: List[Segment], test_util: unittest.TestCase):
    pre_seg_start = -1
    pre_seg_end = 0
    for seg in segments:
        seg_start = seg.address
        seg_end = seg.address + seg.total_size
        test_util.assertTrue(pre_seg_start < pre_seg_end <= seg_start < seg_end)
        valid_segment(seg, test_util)
        pre_seg_start = seg_start
        pre_seg_end = seg_end
