/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
import { action, makeAutoObservable } from 'mobx';
import type { InsightTemplate } from '../entity/insight';
import { entryTemplate } from '../insight/templates/entry';

export class InsightStore {
    readonly templates: Map<string, InsightTemplate>;

    constructor() {
        makeAutoObservable(this);
        this.templates = new Map<string, InsightTemplate>();
    }

    get(id: string): InsightTemplate | undefined {
        return this.templates.get(id);
    }

    has(id: string): boolean {
        return this.templates.has(id);
    }

    loadTemplates(): Promise<void> {
        this.templates.clear();
        const list = [entryTemplate];
        return new Promise<void>(resolve => {
            setTimeout(action(() => {
                list.forEach(template => {
                    this.templates.set(template.id, template);
                });
                resolve();
            }), 0);
        });
    }
}
