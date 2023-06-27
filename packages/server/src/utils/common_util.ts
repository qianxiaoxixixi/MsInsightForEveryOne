import { SHA256 } from 'crypto-js';

/**
 * get traceId
 *
 * @param tid tid
 * @param pid pid
 * @returns traceId
 */
export function getTrackId(tid: number, pid: string): number {
    const str = pid + tid.toString();
    const hashDigest = SHA256(str);
    return hashDigest.words[0];
}
