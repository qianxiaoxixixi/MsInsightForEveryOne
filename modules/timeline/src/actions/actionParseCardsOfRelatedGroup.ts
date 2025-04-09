/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

import { register } from './register';
import { parseCards } from '../api/request';
import { runInAction } from 'mobx';
import { message } from 'antd';
import type { InsightUnit } from '../entity/insight';
import type { CardMetaData, ThreadMetaData } from '../entity/data';
import type { Session } from '../entity/session';
import { preOrderFlatten } from '../entity/common';
import { getRootUnit } from '../utils';
import { CardUnit } from '../insight/units/AscendUnit';

function getRankIdByCardId(cardId: string): string {
    // text 场景：rankId = cardId，直接返回
    // db 场景：cardId = '{文件名} {rankId}'，需特殊处理
    // 匹配最后一个空格之后的所有数字
    const match = cardId.match(/ (?<rankId>\d+)$/);
    if ((match?.groups?.rankId) !== undefined) {
        return match.groups.rankId;
    } else {
        return cardId;
    }
}

function getSelectedRankList(session: Session): string[] {
    return session.selectedUnits.map((item) => (item.metadata as ThreadMetaData).rankList ?? []).flat();
}

function getUnparsedCards(session: Session, rankIds: string[]): InsightUnit[] {
    return preOrderFlatten(getRootUnit(session.units), 0, {
        when: (node) => !(node instanceof CardUnit && node.metadata?.cardName !== 'Host'),
    })
        .filter((item) => item instanceof CardUnit && item.metadata?.cardName !== 'Host' && item.shouldParse)
        .filter((item) => rankIds.includes(getRankIdByCardId((item.metadata as CardMetaData).cardId)));
}

function getUnparsedCardIds(session: Session): string[] {
    let unparsedCards: InsightUnit[] = [];

    // 必须只选中一个
    if (session.selectedUnits.length === 1) {
        const rankList = getSelectedRankList(session);
        if (Array.isArray(rankList) && rankList.length !== 0) {
            unparsedCards = getUnparsedCards(session, rankList);
        }
    }

    return unparsedCards.map((item) => (item.metadata as CardMetaData).cardId);
}

export const actionParseCardsOfRelatedGroup = register({
    name: 'parseCardsOfRelatedGroup',
    label: (session, t) => {
        const unparsedCardIds = getUnparsedCardIds(session);
        return unparsedCardIds.length === 0
            ? t('timeline:contextMenu.Parsed Cards of Related Group')
            : t('timeline:contextMenu.Parse Cards of Related Group', { ranks: unparsedCardIds.map(getRankIdByCardId).join(',') });
    },
    disabled: (session) => {
        const unparsedCardIds = getUnparsedCardIds(session);
        return unparsedCardIds.length === 0;
    },
    visible: (session) => {
        if (session.selectedUnits.length !== 1) {
            return false;
        }
        const rankList = getSelectedRankList(session);
        return Array.isArray(rankList) && rankList.length !== 0;
    },
    perform: (session): void => {
        const rankList = getSelectedRankList(session);
        const unparsedCards = getUnparsedCards(session, rankList);
        const unparsedCardIds = unparsedCards.map((item) => (item.metadata as CardMetaData).cardId);

        parseCards({ cards: unparsedCardIds }).then((): void => {
            runInAction((): void => {
                unparsedCards.forEach((item): void => {
                    item.isParseLoading = true;
                });
            });
        }).catch(err => {
            message.error(err);
        });
    },
});
