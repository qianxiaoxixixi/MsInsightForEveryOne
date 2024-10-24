/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
import * as React from 'react';
import { render, screen } from '@testing-library/react';
import { Label, convertTime } from '../Common';

/**
 * 生成指定长度的随机字符串，使用字符串列表和随机数对应的方式进行匹配
 * @param length 字符串长度
 * @returns 随机字符串
 */
const generateRandomString = (length: number): string => {
    let result = '';
    const characters = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
    for (let i = 0; i < length; i++) {
        result += characters.charAt(Math.floor(Math.random() * characters.length));
    }
    return result;
};

// 测试Label组件是否能正确显示随机文本
it('test function Label with random string then show correct text', () => {
    const randomName = generateRandomString(10);
    render(<Label name={randomName} />);
    expect(screen.getByText(`${randomName} :`)).toBeDefined();
});

// 测试数字类型时间转换为HH:MM:SS.MS.US格式
it('test function convertTime with different inputs return correct results', () => {
    const nullInput = null;
    expect(convertTime(nullInput)).toBe('');
    const notNumStringInput = `a${generateRandomString(5)}`;
    expect(convertTime(notNumStringInput)).toBe(notNumStringInput);
    const hour = Math.floor(Math.random() * 10000);
    const minute = Math.floor(Math.random() * 60);
    const second = Math.floor(Math.random() * 60);
    const milliSecond = Math.floor(Math.random() * 1000);
    const microSecond = Math.floor(Math.random() * 1000);
    const numStringInput = `${(hour * 1000 * 60 * 60) + (minute * 1000 * 60) + (second * 1000) + milliSecond}.${String(microSecond).padStart(3, '0')}`;
    expect(convertTime(numStringInput)).toBe(`${String(hour).padStart(2, '0')}:${String(minute).padStart(2, '0')}:${String(second).padStart(2,
        '0')}.${String(milliSecond).padStart(3, '0')}.${String(microSecond).padStart(3, '0')}`);
});
