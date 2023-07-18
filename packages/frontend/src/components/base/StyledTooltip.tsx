import * as React from 'react';
import { Tooltip } from 'antd';
import { TooltipProps } from 'antd/lib/tooltip';
import { useTheme } from '@emotion/react';

export const StyledTooltip: React.FC<TooltipProps> = ({ children, overlayInnerStyle, ...props }: TooltipProps) => {
    const theme = useTheme();
    return <Tooltip
        color={theme.tooltipBGColor}
        overlayInnerStyle={{ color: theme.tooltipFontColor, ...overlayInnerStyle }}
        {...props}
    >
        {children}
    </Tooltip>;
};
