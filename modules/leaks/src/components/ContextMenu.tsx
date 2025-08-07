/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */
import React, { useRef, useEffect } from 'react';
import styled from '@emotion/styled';
import { Session } from '../entity/session';
import { runInAction } from 'mobx';
import { observer } from 'mobx-react';

interface Position {
    x: number;
    y: number;
};

export interface MenuItemModel {
    label: React.ReactNode;
    key: string;
    action: () => void;
    visible?: boolean;
    disabled?: boolean;
}

const MenuItem = styled.div`
    padding: 4px 16px;
    color: ${(props): string => props.theme.textColorPrimary};

    &:not(.disabled):hover{
      background: ${(props): string => props.theme.primaryColorHover};
      color: #ffffff;
    }
    &.disabled{
        color: ${(props): string => props.theme.textColorDisabled};
    }
`;

function closeMenu(session: Session): void {
    runInAction(() => {
        session.contextMenu.visible = false;
    });
}

// 调整菜单位置：不要超出窗口
function adjustMenuPosition({ session, menu, xPos, yPos }: {
    session: Session;
    menu: HTMLDivElement;
    xPos: Position['x'];
    yPos: Position['y'];
}): void {
    const winWidth = document.documentElement.clientWidth || document.body.clientWidth;
    const winHeight = document.documentElement.clientHeight || document.body.clientHeight;
    let x = xPos;
    let y = yPos;
    if (x + menu.offsetWidth >= winWidth) {
        x -= menu.offsetWidth;
    }
    if (y + menu.offsetHeight > winHeight) {
        y -= menu.offsetHeight;
    }
    runInAction(() => {
        session.contextMenu.xPos = x;
        session.contextMenu.yPos = y;
    });
    menu.focus();
};

const MenuContainer = styled.div`
    padding: 3px 0;
    min-width: 100px;
    border-radius: ${(props): string => props.theme.borderRadiusBase};
    background-color:  ${(props): string => props.theme.contextMenuBgColor};
    position: fixed;
    z-index: 99999;
    box-shadow: ${(props): string => props.theme.boxShadowLight};
    user-select: none;
`;
interface IProps {
    session: Session;
    menuItems: MenuItemModel[];
}
const Menu = ({ session, menuItems }: IProps): JSX.Element => {
    const menuRef = useRef<HTMLDivElement>(null);
    const { contextMenu } = session;
    const handleMouseDown = (e: MouseEvent): void => {
        if ((e.target as HTMLElement)?.parentNode !== menuRef.current) {
            closeMenu(session);
        }
    };

    const handleCloseMenu = (): void => {
        closeMenu(session);
    };

    useEffect(() => {
        window.addEventListener('mousedown', handleMouseDown);
        window.addEventListener('wheel', handleCloseMenu);

        return () => {
            window.removeEventListener('mousedown', handleMouseDown);
            window.removeEventListener('wheel', handleCloseMenu);
        };
    });
    useEffect(() => {
        const menu = menuRef.current;
        if (contextMenu.visible && menu !== null) {
            adjustMenuPosition({ session, menu, xPos: contextMenu.xPos, yPos: contextMenu.yPos });
        }
    }, [contextMenu.visible, contextMenu.xPos, contextMenu.yPos]);
    return (
        contextMenu.visible && menuItems.length > 0
            ? <MenuContainer ref={menuRef} style={{ left: `${contextMenu.xPos}px`, top: `${contextMenu.yPos}px` }} tabIndex={-1} onBlur={(): void => { closeMenu(session); }} >
                {menuItems.map((item: MenuItemModel): JSX.Element => (
                    <MenuItem className={`menu-item ${item.disabled ? 'disabled' : ''}`} key={item.key}
                        onClick={(): void => {
                            if (item.disabled) {
                                return;
                            }
                            item.action();
                            closeMenu(session);
                        }}>
                        {item.label}
                    </MenuItem>))}
            </MenuContainer>
            : <></>
    );
};

export const ContextMenu = observer(Menu);
