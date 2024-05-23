import { Typography } from 'antd';
import React from 'react';
import { Container } from '../Common';
import { useTranslation } from 'react-i18next';
interface HitItem {
    title: string;
    contentList: string[];
}
const Help = ({ style = {} }: {style?: object}): JSX.Element => {
    const { t } = useTranslation('communication');
    const hit: HitItem[] = t('GuidanceContent', { returnObjects: true });
    return (<div style={{
        lineHeight: '2.5rem',
        height: '100%',
        padding: '0 5px 0 15px',
        ...style,
    }}>
        <Container
            type={'headerfixed'}
            title={t('Guidance')}
            content={
                <div >
                    {
                        hit?.map((item, index) => (
                            <p key={index}>
                                <span className={'h5'}>{item.title}</span>
                                <p className={'left20'}>
                                    {
                                        item.contentList?.map((content, contentindex) =>
                                            (<span key={contentindex}>{content}<br/></span>))
                                    }
                                </p>
                            </p>
                        ))
                    }
                </div>}
        />
        <div style={{ display: 'none' }}>
            <Typography.Title level={5} >
                <div>Advisor</div>
            </Typography.Title>
            <div className={'left20'}>
                There is a low efficiency issue in the current cluster communication, Waiting take up most of
                the time in communication action for rank 3 . The max wait ratio is: 0.92 .
                <br/>
                The reason is that the the ranks are not synchronized. Rank 2 has a lot longer computation
                time than other ranks.
            </div>
        </div>
    </div>);
};

export default Help;
