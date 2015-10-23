'use strict';

//import router from '../router';
import Dispatcher from '../core/Dispatcher';
import EventEmitter from 'eventemitter3';
import assign from 'react/lib/Object.assign';
import router from '../router';
import tcp from '../utils/tcp';

//import AppActions from '../actions/AppActions';

const CHANGE_EVENT = 'CHANGE_AppStore';

let _msg = '';
let _isLogin = false;

//个人信息
let _myInfo;
//8张桌子
let _desks;
//牌局信息
let _deskDetail = ['', '', ''];
//各家手牌
let _deskCards = [[], [], []];
//我的index
let _myIndex;
//左边那家伙的index
let _leftIndex;
//右边那家伙的index
let _rightIndex;

//是否准备
let _isReady = false;

let _myCall = false;

let _masterCards = [];

let _show = [0, 0, 0];

let _master;

const AppStore = assign({}, EventEmitter.prototype, {

    getIsLogin(){
      return _isLogin;
    },
    getMsg(){
      return _msg;
    },
    getMyInfo(){
        return _myInfo;
    },
    getDesks(){
        return _desks;
    },
    getDeskDetail(){
        return _deskDetail;
    },
    getMyIndex(){
        return _myIndex;
    },
    getLeftIndex(){
        return _leftIndex;
    },
    getRightIndex(){
        return _rightIndex;
    },
    getIsReady(){
        return _isReady;
    },
    getLeftName(){
        if(_leftIndex==null) return null;
        return   _deskDetail[_leftIndex];
    },
    getRightName(){
        if(_rightIndex==null) return null;
        return   _deskDetail[_rightIndex];
    },
    getLeftCards(){
        if(_leftIndex==null) return null;
        return _deskCards[_leftIndex];
    },
    getRightCards(){
        if(_rightIndex==null) return null;
        return _deskCards[_rightIndex];
    },
    getMyCards(){
        if(!_myIndex==null) return null;
        return _deskCards[_myIndex];
    },
    getMasterCards(){
         return _masterCards;
    },
    getMyCall(){
        return _myCall;
    },

    getLeftShow(){
        if(_leftIndex==null) return null;
        return _show[_leftIndex];
    },
    getRightShow(){
        if(_rightIndex==null) return null;
        return _show[_rightIndex];
    },
    getMyShow(){
        if(_myIndex==null) return null;
        return _show[_myIndex];
    },
    getMaster(){
        return _master;
    },

    emitChange() {
        return this.emit(CHANGE_EVENT);
    },

    addChangeListener(callback) {
        this.on(CHANGE_EVENT, callback);
    },

    removeChangeListener(callback) {
        this.off(CHANGE_EVENT, callback);
    }

});

AppStore.dispatcherToken = Dispatcher.register((payload) => {
    var action = payload.action;

    switch (action.actionType) {
        case 'LOGIN_START':
            _isLogin = true;
            _msg = '';
            AppStore.emitChange();
            break;
        case 'LOGIN_FAILURE':
            _isLogin = false;
            _msg = '用户名不存在';
            AppStore.emitChange();
            break;
        case 'LOGIN_SUCCESS':
            _isLogin = false;
            _msg = '';
            AppStore.emitChange();
            router.transitionTo('rooms');
            break;
        case 'TABLE_DATA':
            _desks = action.data;
            AppStore.emitChange();
            break;
        case 'USER_DATA':
            _myInfo = action.data;
            AppStore.emitChange();
            break;
        case 'TABLE_JOIN':
            //加入桌子
            _deskDetail = ['', '', ''];
            _deskCards = [[], [], []];
            _masterCards = [];
            _master = null;
            _show = [0, 0, 0];
            _myIndex = null;
            _leftIndex = null;
            _rightIndex = null;
            router.transitionTo('game');
            AppStore.emitChange();
            break;
        case 'TABLE_DETAIL':
            //桌子详细
            _deskDetail = action.data;
            for(let i = 0; i<3; i++){
                if(_deskDetail[i]==_myInfo.name){
                    _myIndex = i;
                    _leftIndex = (i+2)%3;
                    _rightIndex = (i+1)%3;
                    break;
                }
            }

            AppStore.emitChange();
            break;
        case 'TABLE_EXIT':
            //退出游戏
            _desks = null;
            tcp.loadTables();
            router.transitionTo('rooms');
            break;

        case 'GAME_READY':
            _isReady = true;
            AppStore.emitChange();
            break;

        case 'GAME_FIRST_CARDS':
            _deskCards = action.data;
            _show = action.show;
            AppStore.emitChange();
            break;
        case 'GAME_MY_CALL':
            _myCall = true;
            AppStore.emitChange();
            break;
        case 'GAME_CALLED':
            _myCall = false;
            AppStore.emitChange();
            break;
        case 'GAME_GETMASTERCARDS':
            _masterCards = action.data;
            AppStore.emitChange();
            break;
        case 'GAME_GETMASTERWHO':
            _master = action.data;
            for(let i = 0;i<3;i++){
                _deskCards[_master].push(_masterCards[i]);
            }
            AppStore.emitChange();
            break;
        default:
            break;
    }

});

module.exports = AppStore;
