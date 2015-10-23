'use strict';

import React from 'react';
import {Paper, TextField, FlatButton} from 'material-ui';
import AppStore from '../stores/AppStore';

export default React.createClass({
    getInitialState(){
        return {
            info: AppStore.getMyInfo()
        }
    },
    componentWillMount(){
        AppStore.addChangeListener(this._onAppChange);
    },
    componentWillUnmount(){
        AppStore.removeChangeListener(this._onAppChange);
    },
    _onAppChange(){
        this.setState({
            info: AppStore.getMyInfo()
        });
    },


    render() {
        return (
            <div className="info">
                {
                    this.state.info?
                            <Paper zDepth={3}>
                                <span className="data">用户名:{this.state.info.name}</span>
                                <span className="data">分数:{this.state.info.score}</span>
                                <span className="data">胜场次:{this.state.info.wins}</span>
                                <span className="data">总场次:{this.state.info.plays}</span>
                                <span className="data">逃跑次:{this.state.info.escapes}</span>
                                <span className="data">断线次:{this.state.info.disconnects}</span>
                            </Paper>:
                        <Paper className="loading">载入中...</Paper>
                }

            </div>
        );
    }
});