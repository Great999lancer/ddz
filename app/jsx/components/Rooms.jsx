'use strict';

import React from 'react';
import {Paper, TextField, FlatButton} from 'material-ui';
import AppStore from '../stores/AppStore';
import MyInfo from './MyInfo.jsx';
import AppActions from '../actions/AppActions';

export default React.createClass({
    getInitialState(){
        return {
            desks: AppStore.getDesks(),
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
            desks: AppStore.getDesks(),
            info: AppStore.getMyInfo()
        });
    },
    handleRoomClick(index){
        return ()=>{
            AppActions.joinTable(index);
        }
    },

    render() {
        return (
            <div className="rooms">
                <MyInfo/>
                {
                    this.state.desks?
                    this.state.desks.map((room, i)=>
                    <Paper key={i} className="room" zDepth={1} onClick={this.handleRoomClick(i+1)}>
                        <h2>房间{i+1}</h2>
                        <h4>{room}/3</h4>
                    </Paper>):<Paper className="loading">载入中...</Paper>

                }

            </div>
        );
    }
});