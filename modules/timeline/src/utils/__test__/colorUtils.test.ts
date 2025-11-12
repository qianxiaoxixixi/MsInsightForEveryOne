/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */
import { hashToNumber } from '../colorUtils';

describe('hashToNumber', () => {
    it('when maxIndex is negative then return 0', () => {
        const maxIndex: number = -100000;
        const input: string = 'pppppppppppppp';
        const output = hashToNumber(input, maxIndex);
        expect(output).toBe(0);
    });

    it('when maxIndex is normal then return normal', () => {
        const maxIndex: number = 100000;
        const input: string = 'pppppppppppppp';
        const output = hashToNumber(input, maxIndex);
        expect(output).toBe(48608);
    });
});
