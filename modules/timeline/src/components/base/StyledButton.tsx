import styled from '@emotion/styled';
import { ButtonProps } from 'antd/lib/button';
import { Button } from 'antd';
import * as React from 'react';
import { useTheme } from '@emotion/react';
import { ForwardedRef, FunctionComponent, SVGProps } from 'react';
import { StyledTooltip } from './StyledTooltip';

interface CustomButtonProps {
    isEmphasize?: boolean;
    isDisabled?: boolean;
    isSuspend?: boolean;
    tooltip?: string | null;
    icon?: FunctionComponent<SVGProps<SVGSVGElement> & { title?: string }>;
}

export const StyledButton = styled(Button)`
    display: flex;
    align-items: center;
    width: ${(props): string => props.width !== undefined ? `${props.width}px` : '22px'};
    height: 20px;
    line-height: 20px;
    color: ${(props): string => props.theme.fontColor} !important;
    background-color: ${(props): string => props?.style?.backgroundColor ?? props.theme.insightHeaderButtonBackgroundColor}!important;
    &:hover {
        background-color: ${(props): string => props?.style?.backgroundColor ?? props.theme.insightHeaderButtonBackgroundColor};
        cursor: ${(props): string => props.disabled as boolean ? 'not-allowed' : 'pointer'};
    }

    border-radius: 4px;
    justify-content: center;
    border: none;
    margin-right: 3px;
`;

export const CustomButton = React.forwardRef(({ icon, isDisabled, isSuspend, tooltip, isEmphasize, ...props }: ButtonProps & CustomButtonProps,
    ref?: ForwardedRef<HTMLButtonElement>): JSX.Element => {
    const theme = useTheme();
    let buttonFillColor = theme.buttonColor.enableClickColor;
    if (isEmphasize && !isDisabled) {
        buttonFillColor = theme.buttonColor.emphasizeColor;
    } else if (isDisabled) {
        buttonFillColor = theme.buttonColor.disableClickColor;
    }
    const Icon = icon;
    return <StyledTooltip title={tooltip}>
        <StyledButton ref={ref} disabled={isDisabled} {...props} icon={Icon && <Icon fill={buttonFillColor}/>}
            style={{ backgroundColor: isSuspend ? theme.buttonColor.suspendBGColor : theme.buttonColor.unSuspendBGColor }}>
        </StyledButton>
    </StyledTooltip>;
});

CustomButton.displayName = 'CustomButton';

export const PressButton = styled(Button)`
    display: flex;
    align-items: center;
    color: ${(props): string => props.theme.fontColor} !important;
    background-color: ${(props): string => props?.style?.backgroundColor ?? props.theme.insightHeaderButtonBackgroundColor}!important;
    border-radius: 4px;
    justify-content: center;
    height: 20px;
    margin-top: 6px;
    border: none;
    margin-right: 3px;
`;
