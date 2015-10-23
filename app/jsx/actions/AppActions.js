'use strict';
import Dispatcher from '../core/Dispatcher';
import tcp from '../utils/tcp';


export default {

    login(ip, port, name){
        Dispatcher.handleServerAction({
            actionType: 'LOGIN_START'
        });
        //start tcp
        tcp.connect(ip, port, name);
    },

    loginFailure(){
        Dispatcher.handleServerAction({
            actionType: 'LOGIN_FAILURE'
        });
    },
    loginSuccess(){
        Dispatcher.handleServerAction({
            actionType: 'LOGIN_SUCCESS'
        });
    },
    getUserData(data){
        Dispatcher.handleServerAction({
            actionType: 'USER_DATA',
            data
        });
    },
    getTableInfo(data){
        Dispatcher.handleServerAction({
            actionType: 'TABLE_DATA',
            data
        });
    },

    joinTable(index){
        Dispatcher.handleServerAction({
            actionType: 'TABLE_JOIN',
            index
        });
        tcp.join(index);
    },

    getTableDetail(data){
        Dispatcher.handleServerAction({
            actionType: 'TABLE_DETAIL',
            data
        });
    },

    exitTable(){
        tcp.exit();
        //载入桌子
        Dispatcher.handleServerAction({
            actionType: 'TABLE_EXIT'
        });

    },

    disconnected(){
      router.transitionTo('home');
    },
    getTableRefuse(){
        Dispatcher.handleServerAction({
            actionType: 'TABLE_EXIT'
        });
    },
    beReady(type){
        Dispatcher.handleServerAction({
            actionType: 'GAME_READY'
        });
        tcp.ready(type);
    },

    getFirstCards(cards, show){
        Dispatcher.handleServerAction({
            actionType: 'GAME_FIRST_CARDS',
            data: cards,
            show
        });
    },

    getMyCall(){
        Dispatcher.handleServerAction({
            actionType: 'GAME_MY_CALL'
        });
    },

    call(isCall){
        Dispatcher.handleServerAction({
            actionType: 'GAME_CALLED'
        });
        tcp.call(isCall);
    },

    getMasterCards(data){
        Dispatcher.handleServerAction({
            actionType: 'GAME_GETMASTERCARDS',
            data
        });
    },

    getMasterWho(data){
        Dispatcher.handleServerAction({
            actionType: 'GAME_GETMASTERWHO',
            data
        });
    }

};
