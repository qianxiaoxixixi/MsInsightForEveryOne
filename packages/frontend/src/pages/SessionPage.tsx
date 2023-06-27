import { observer } from 'mobx-react-lite';
import React, { useEffect } from 'react';
import { BottomPanel } from '../components/BottomPanel';
import { ChartContainer } from '../components/ChartContainer';
import { Session } from '../entity/session';
import { DragDirection, useDraggableContainer } from '../utils/useDraggableContainer';

export const BOTTOM_HEIGHT = 332;
export const SessionPage = observer(function SessionPage({ session }: { session: Session }) {
    const [ view, handleOpen ] = useDraggableContainer({ draggableWH: BOTTOM_HEIGHT, dragDirection: DragDirection.bottom, open: false });
    useEffect(() => {
        if (session.selectedUnitKeys.length > 0 && (session.selectedRange !== undefined || session.selectedData !== undefined)) {
            handleOpen(true);
        }
    }, [ session.selectedUnitKeys, session.selectedRange, session.selectedData ]);

    return view({
        mainContainer: <ChartContainer session={session} interactive />,
        draggableContainer: <BottomPanel session={session} />,
        id: 'SessionPage',
    });
});
