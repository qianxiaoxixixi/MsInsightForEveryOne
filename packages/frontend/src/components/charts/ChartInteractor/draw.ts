import type { Theme } from '@emotion/react';
import { CustomCrossRenderer } from './custom';
import { Session } from '../../../entity/session';
import { Pos, getTimeDifference } from './common';
import { drawRoundedRect } from '../common';
import { InteractorMouseState } from './ChartInteractor';
import { THUMB_WIDTH_PX } from '../../base';
import { getTextParser } from '../TimelineAxis';

const UP_LINE: number = 30;
const DOWN_LINE: number = 45;

type DrawArrowOptions = {
    toX: number;
    toY: number;
    fromX: number;
    fromY: number;
    length: number;
    angle: number;
    color: string;
};
export const drawArrow = (ctx: CanvasRenderingContext2D, { toX, toY, fromX, fromY, length, angle, color }: DrawArrowOptions): void => {
    const a = Math.atan2((toY - fromY), (toX - fromX));
    const xC = toX - length * Math.cos(a + angle * Math.PI / 180);
    const yC = toY - length * Math.sin(a + angle * Math.PI / 180);
    const xD = toX - length * Math.cos(a - angle * Math.PI / 180);
    const yD = toY - length * Math.sin(a - angle * Math.PI / 180);
    ctx.save();
    ctx.beginPath();
    ctx.fillStyle = color;
    ctx.moveTo(toX, toY);
    ctx.lineTo(xC, yC);
    ctx.lineTo(xD, yD);
    ctx.lineTo(toX, toY);
    ctx.fill();
    ctx.closePath();
    ctx.restore();
};

const drawTimeDiff = (ctx: CanvasRenderingContext2D | null, maskRange: number[], xScale: (posX: number) => number,
    isNsMode: boolean, theme: Theme, selectedRange: [number, number] | undefined): void => {
    if (!ctx) {
        return;
    }
    ctx.textBaseline = 'top';
    ctx.textAlign = 'center';
    let timeString;
    if (selectedRange !== undefined) {
        timeString = getTimeDifference(Math.floor(selectedRange[0]), Math.floor(selectedRange[1]), isNsMode);
    } else {
        timeString = getTimeDifference(Math.floor(xScale(maskRange[0])), Math.floor(xScale(maskRange[1])), isNsMode);
    }
    drawTimeDiffText(ctx, timeString, maskRange, theme, isNsMode);
    // left
    drawArrow(ctx, {
        toX: maskRange[0],
        toY: (UP_LINE + DOWN_LINE) / 2,
        fromX: maskRange[0] + 1,
        fromY: (UP_LINE + DOWN_LINE) / 2,
        length: 10,
        angle: 30,
        color: theme.selectedChartColor,
    });

    // right
    drawArrow(ctx, {
        toX: maskRange[1],
        toY: (UP_LINE + DOWN_LINE) / 2,
        fromX: maskRange[1] - 1,
        fromY: (UP_LINE + DOWN_LINE) / 2,
        length: 10,
        angle: 30,
        color: theme.selectedChartColor,
    });
};

// hover时的蓝色圆角方框以及时间字符串
const drawHoverTimeRect = (ctx: CanvasRenderingContext2D,
    width: number, mousePosNow: Pos, xScale: (posX: number) => number, { isNsMode, domainRange }: Session): void => {
    ctx.textAlign = 'center';
    ctx.textBaseline = 'top';
    ctx.font = '12px -apple-system, BlinkMacSystemFont, "Segoe UI", "Roboto", "Oxygen", "Ubuntu", "Cantarell", "Fira Sans", "Droid Sans", "Helvetica Neue", sans-serif';
    const displayText = getTextParser(isNsMode)(Math.floor(xScale(mousePosNow.x)), [ domainRange.domainStart, domainRange.domainEnd ]);
    const textLength = ctx.measureText(displayText).width;
    const roundRectWidth = textLength + 10;
    const roundRectHeight = 16;
    const rouned = 8;
    ctx.fillStyle = '#3778ED';
    if (mousePosNow.x < 0 || mousePosNow.x > ctx.canvas.clientWidth - THUMB_WIDTH_PX) {
        return;
    }
    if (mousePosNow.x <= roundRectWidth / 2) {
        drawRoundedRect([ 0, 0, roundRectWidth, roundRectHeight ], ctx, rouned);
        ctx.fill();
        ctx.fillStyle = 'white';
        ctx.fillText(displayText, roundRectWidth / 2, 3);
        return;
    }

    if (mousePosNow.x >= width - roundRectWidth / 2 && mousePosNow.x <= ctx.canvas.clientWidth - THUMB_WIDTH_PX) {
        drawRoundedRect([ width - roundRectWidth, 0, roundRectWidth, roundRectHeight ], ctx, rouned);
        ctx.fill();
        ctx.fillStyle = 'white';
        ctx.fillText(displayText, width - roundRectWidth / 2, 3);
        return;
    }
    drawRoundedRect([ mousePosNow.x - roundRectWidth / 2, 0, roundRectWidth, roundRectHeight ], ctx, rouned);
    ctx.fill();
    ctx.fillStyle = 'white';
    ctx.fillText(displayText, mousePosNow.x, 3);
};

export const drawTimeDiffText = (ctx: CanvasRenderingContext2D, timeString: string, maskRange: number[], theme: Theme, isNsMode: boolean): void => {
    ctx.font = '12px sans-serif';
    const textLength = ctx.measureText(timeString).width + 20;
    if (textLength >= Math.abs(maskRange[1] - maskRange[0])) {
        ctx.beginPath();
        ctx.moveTo(maskRange[0], (UP_LINE + DOWN_LINE) / 2);
        ctx.lineTo(maskRange[1], (UP_LINE + DOWN_LINE) / 2);
        ctx.strokeStyle = theme.timeDiffPictureColor;
        ctx.stroke();

        ctx.beginPath();
        ctx.moveTo((maskRange[1] + maskRange[0]) / 2, (UP_LINE + DOWN_LINE) / 2);
        ctx.lineTo((maskRange[1] + maskRange[0]) / 2 - 5, DOWN_LINE);
        ctx.lineTo((maskRange[1] + maskRange[0]) / 2 + 5, DOWN_LINE);
        ctx.fillStyle = theme.timeDiffBackgroundColor;
        ctx.fill();
        // back
        ctx.beginPath();
        ctx.rect((maskRange[1] + maskRange[0] - textLength) / 2, DOWN_LINE, textLength, DOWN_LINE - UP_LINE);
        ctx.fillStyle = theme.timeDiffBackgroundColor;
        ctx.fill();

        // text
        ctx.fillStyle = theme.fontColor;
        ctx.font = '12px sans-serif';
        ctx.fillText(timeString, (maskRange[1] + maskRange[0]) / 2, DOWN_LINE + 4);
    } else {
        // back
        ctx.beginPath();
        ctx.rect(maskRange[0], UP_LINE, maskRange[1] - maskRange[0], DOWN_LINE - UP_LINE);
        ctx.fillStyle = theme.timeDiffBackgroundColor;
        ctx.fill();
        // text
        ctx.fillStyle = theme.fontColor;
        ctx.font = '12px sans-serif';
        ctx.fillText(timeString, (maskRange[1] + maskRange[0]) / 2, (UP_LINE + DOWN_LINE) / 2 - 5);
        // line
        ctx.beginPath();
        ctx.moveTo(maskRange[0], (UP_LINE + DOWN_LINE) / 2);
        ctx.lineTo((maskRange[0] + maskRange[1]) / 2 - textLength / 2, (UP_LINE + DOWN_LINE) / 2);
        ctx.moveTo(maskRange[1], (UP_LINE + DOWN_LINE) / 2);
        ctx.lineTo((maskRange[0] + maskRange[1]) / 2 + textLength / 2, (UP_LINE + DOWN_LINE) / 2);
        ctx.strokeStyle = theme.timeDiffPictureColor;
        ctx.stroke();
    }
};

export const draw = (ctx: CanvasRenderingContext2D | null, width: number, height: number,
    xReverseScale: (ts: number) => number, xScale: (posX: number) => number,
    interactorMouseState: InteractorMouseState, selectedRange: undefined | [number, number], isNsMode: boolean,
    session: Session, customRenderers: CustomCrossRenderer[], theme: Theme): void => {
    if (ctx === null) { return; }
    // clear all
    ctx.clearRect(0, 0, width, height);
    let maskRange;
    const clickPos = interactorMouseState.clickPos.current;
    const mousePosNow = interactorMouseState.lastPos.current;
    // draw mask
    if (clickPos !== undefined && mousePosNow !== undefined) {
        // 1st priority
        // is brushing
        maskRange = [ clickPos.x, mousePosNow.x ];
    } else if (selectedRange !== undefined) {
        // 2nd priority
        // not brushing now but has selected range
        maskRange = [ xReverseScale(selectedRange[0]), xReverseScale(selectedRange[1]) ];
    }
    if (maskRange !== undefined) {
        maskRange.sort((a, b) => a - b);
        ctx.fillStyle = 'rgba(66,66,66,0.5)';
        ctx.fillRect(0, 0, maskRange[0], height);
        ctx.fillRect(maskRange[1], 0, width - maskRange[1], height);
        drawTimeDiff(ctx, maskRange, xScale, isNsMode, theme, selectedRange);
    }
    // draw hoverline and timeaxis highlight
    if (clickPos !== undefined) {
        ctx.strokeStyle = '#3778ED';
        ctx.beginPath();
        ctx.moveTo(clickPos.x, 0);
        ctx.lineTo(clickPos.x, height);
        ctx.stroke();
    }

    if (mousePosNow !== undefined) {
        // ---hover line---
        ctx.strokeStyle = '#3778ED';
        ctx.beginPath();
        ctx.moveTo(mousePosNow.x, 0);
        ctx.lineTo(mousePosNow.x, height);
        ctx.stroke();

        // timeRect & text
        drawHoverTimeRect(ctx, width, mousePosNow, xScale, session);
    }
    customRenderers.forEach(it => it(ctx, session, xReverseScale, theme));
};
