/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

export interface ParseCardsParam {
    cards: string[];
};

/**
 * 根据cardId集合解析timeline
 *
 * @param {cards} cards cardId
 * @return {[]}
 */
export const parseCards = async(param: ParseCardsParam): Promise<any> => {
    return window.requestData('parse/cards', param, 'timeline');
};
