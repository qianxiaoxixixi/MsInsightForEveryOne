import type { IframeHTMLAttributes } from "vue";

export interface ModuleConfig {
    name: string;
    requestName: Lowercase<string>;
    attributes: IframeHTMLAttributes;
    isDefault?: boolean;
};

export const modulesConfig: ModuleConfig[] = [
    {
        name: 'Timeline View',
        requestName: 'timeline',
        attributes: {
            src: './plugins/Timeline/index.html'
        },
        isDefault: true,
    },
    {
        name: 'Summary',
        requestName: 'summary',
        attributes: {
            src: 'http://localhost:3000/summary.html'
        },
        // isDefault: true,
    },
    {
        name: 'Communication',
        requestName: 'communication',
        attributes: {
            src: 'http://localhost:3000/communication.html'
        },
        // isDefault: true,
    },
    {
        name: 'Memory',
        requestName: 'memory',
        attributes: {
            src: './plugins/Memory/index.html'
        },
        // isDefault: true
    }
];
