declare module 'JSONStream' {
    export function parse(pattern: any): NodeJS.ReadWriteStream;
    export function parse(patterns: any[]): NodeJS.ReadWriteStream;

    export function stringify(): NodeJS.ReadWriteStream;

    export function stringify(open: string, sep: string, close: string): NodeJS.ReadWriteStream;

    export function stringify(newlineOnly: NewlineOnlyIndicator): NodeJS.ReadWriteStream;
    export type NewlineOnlyIndicator = false;
}
