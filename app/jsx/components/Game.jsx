'use strict';

import React from 'react';
import {Paper, TextField, FlatButton} from 'material-ui';
import AppStore from '../stores/AppStore';
import AppActions from '../actions/AppActions';
import MyCards from './MyCards.jsx';
import Cards from './Cards.jsx';
import Card from './Card.jsx';
export default React.createClass({
    getInitialState(){
        return {
            info: AppStore.getMyInfo(),
            myIndex: AppStore.getMyIndex(),
            isReady: AppStore.getIsReady(),
            leftName: AppStore.getLeftName(),
            rightName: AppStore.getRightName(),
            leftCards: AppStore.getLeftCards(),
            rightCards: AppStore.getRightCards(),
            myCards: AppStore.getMyCards(),
            myCall: AppStore.getMyCall(),
            masterCards: AppStore.getMasterCards(),
            leftShow: AppStore.getLeftShow(),
            rightShow: AppStore.getRightShow(),
            master: AppStore.getMaster(),
            left: AppStore.getLeftIndex(),
            right: AppStore.getRightIndex()

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
            info: AppStore.getMyInfo(),
            myIndex: AppStore.getMyIndex(),
            isReady: AppStore.getIsReady(),
            leftName: AppStore.getLeftName(),
            rightName: AppStore.getRightName(),
            leftCards: AppStore.getLeftCards(),
            rightCards: AppStore.getRightCards(),
            myCards: AppStore.getMyCards(),
            myCall: AppStore.getMyCall(),
            masterCards: AppStore.getMasterCards(),
            leftShow: AppStore.getLeftShow(),
            rightShow: AppStore.getRightShow(),
            master: AppStore.getMaster(),
            left: AppStore.getLeftIndex(),
            right: AppStore.getRightIndex()

        });
    },
    handleExit(){
        AppActions.exitTable();
    },
    handleNormalReady(){
        AppActions.beReady(1);
    },
    handleManlyNormalReady(){
        AppActions.beReady(2);

    },
    handleCall(){
        AppActions.call(true);
    },
    handleNoCall(){
        AppActions.call(false);
    },
    render() {
        return (
            <div className="game">

                {
                    this.state.masterCards&&this.state.masterCards.length?
                        <div className="masterCards">
                            {this.state.masterCards.map((code) => <li><Card key={code} code={code} hide={this.state.master!==null} /></li>)}
                        </div>:null
                }



                <FlatButton className="exit" label="退出" onClick={this.handleExit}/>
                {
                    this.state.leftName?<Cards nickname={this.state.leftName} cards={this.state.leftCards} hide={this.state.leftShow} master={this.state.master === this.state.left}/>:null
                }
                {
                    this.state.rightName? <Cards right={true} nickname={this.state.rightName} cards={this.state.rightCards} hide={this.state.rightShow} master={this.state.master === this.state.left}/>:null
                }

                <div className="main">
                    {
                        this.state.info ?
                        <ul className="myInfo">
                            {this.state.master===this.state.myIndex?<li className="name">地主！</li>:null}
                            <li className="name">{this.state.info.name}</li>
                            <li>分数:{this.state.info.score}</li>
                        </ul>:
                        <ul className="myInfo">
                            没有个人资料
                        </ul>
                    }

                    <div className="cardsArea">
                        {
                            this.state.isReady?null:
                                <div className="actions">
                                    <FlatButton label="准备" className="primary" onClick={this.handleNormalReady}/>
                                    <FlatButton label="男人的明牌准备" onClick={this.handleManlyNormalReady}/>
                                </div>
                        }
                        {
                            this.state.myCall?<div className="actions">
                                <FlatButton label="男人的抢地主" className="primary" onClick={this.handleCall}/>
                                <FlatButton label="不抢" onClick={this.handleNoCall}/>
                            </div>:null
                        }
                        <MyCards cards={this.state.myCards}/>

                    </div>

                </div>

            </div>
        );
    }
});