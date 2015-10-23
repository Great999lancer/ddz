'use strict';
import AppActions from '../actions/AppActions';
const Code = {
    //客户端
    LOGIN: 0x01,
    JOIN: 0X04,
    TABLES: 0x03,
    READY: 0x05,
    EXIT: 0x09,
    CALL: 0x06,

    //服务端
    RLOGIN: 0x51,
    RLOGINFAILURE: 0x52,
    RUSERDATA: 0x53,
    RTABLE: 0x54,
    RTABLEDATA: 0x55,
    RTABLEREFUSE: 0x56,

    RGAMECARDS: 0x58,
    RYOURCALL: 0x59,

    RMASTERCARDS: 0x80,
    RMASTERWHO: 0x5A

};

function decodeMasterWho(buf){
    return buf.readUInt8(0);
}

function decodeMasterCards(buf){
    var cards = [];
    for(let i=0; i<3; i++){
        cards.push(buf.readUInt8(i));
    }
    return cards;
}

function decodeStartCardsShow(buf){
    var show = [];
    for(let i=0; i<3; i++){
        show.push(buf.readUInt8(68+i));
    }
    return show;
}

function decodeStartCards(buf){
    var cards =[[], [], []];
    for(let i=0; i<3; i++){
        for(let j=0; j<17; j++){
            cards[i].push(buf.readUInt8(i*21 + j));
        }
    }
    return cards;
}

function decodeTableDetail(buf){
    var names = [];
    for(let i = 0; i<16 * 3; i += 16){
        names.push(buf.toString('utf8', i, i + 16).trim());
    }
    return names;
}

function decodeUser(buf){
    var name = buf.toString('utf-8', 0, 16).trim();
    var score = buf.readUInt32LE(16);
    var wins = buf.readUInt16LE(20);
    var plays = buf.readUInt16LE(22);
    var escapes = buf.readUInt16LE(24);
    var disconnects = buf.readUInt16LE(26);
    return {
        name,
        score,
        wins,
        plays,
        escapes,
        disconnects
    };
}

function decodeTable(buf){
    var tables = [];
    for(let i = 0; i<16; i += 2){
        tables.push(buf.readUInt16LE(i));
    }
    return tables;
}

function decodeMsg(buf){
    var type = buf.readUInt8(0);
    var len = buf.readUInt16BE(1);
    var data = buf.slice(3, 3+len);
    console.log('[收到消息] inferred: ', {type, len, data});
    switch(type){
        case Code.RLOGIN:
            AppActions.loginSuccess();
            let userData = decodeUser(data);
            AppActions.getUserData(userData);
            console.log('用户信息', userData);
            break;
        case Code.RLOGINFAILURE:
            AppActions.loginFailure();
            break;
        case Code.RTABLE:
            let tableData = decodeTable(data);
            AppActions.getTableInfo(tableData);
            console.log('桌子信息', tableData);
            break;
        case Code.RTABLEDATA:
            let tableDetail = decodeTableDetail(data);
            AppActions.getTableDetail(tableDetail);
            console.log('桌子详细', tableDetail);
            break;
        case Code.RTABLEREFUSE:
            AppActions.getTableRefuse();
            break;
        case Code.RGAMECARDS:
            let cards = decodeStartCards(data);
            let show = decodeStartCardsShow(data);
            console.log('牌的详情', cards);
            AppActions.getFirstCards(cards, show);
            break;
        case Code.RYOURCALL:
            AppActions.getMyCall();
            break;
        case Code.RMASTERCARDS:
            let masterCards = decodeMasterCards(data);
            console.log('地主牌', masterCards);
            AppActions.getMasterCards(masterCards);
            break;
        case Code.RMASTERWHO:
            let masterWho = decodeMasterWho(data);
            AppActions.getMasterWho(masterWho);
            break;
        default:
            break;
    }
}

var tmpBuf;         //积累Buf
var tmpLen = 3;     //报头长度
function getLen(buf){
    if(buf.length<3){
        console.log('[收到消息] 包长度过短');
        return 0;
    }
    var len = buf.readUInt16BE(1);
    console.log('[收到消息] 包长度：' + len);
    return len;
}
//接收数据
function recvData(raw){
    console.log('[收到消息] raw: ', raw);
    if(!tmpBuf){
        if(raw.length<3){
            alert('[错误] 包长度过短');
            return;
        }
        tmpBuf = raw;
        var len = getLen(tmpBuf);
        tmpLen = 3 + len;
    }

    if(tmpBuf.length<tmpLen){
        tmpBuf = bufferConcat([tmpBuf, raw]);
    }
    else{
        decodeMsg(tmpBuf);
        tmpBuf = null;
        tmpLen = 3;
    }

}

//组成buff并发送
function encodeMsgAndSend(type, data){
    //写type
    var typeBuf = new Buffer(1);
    //写len
    var len = data?data.length:0;
    var lenBuf = new Buffer(2);
    lenBuf.writeUInt16BE(len, 0);
    //写data
    typeBuf.writeUInt8(type, 0);
    var dataBuf;
    if(data) {
        dataBuf = data;
    }
    else{
        dataBuf = new Buffer(1);
    }
    var buf = bufferConcat([typeBuf, lenBuf, dataBuf], 3+len);
    console.log('[发送信息] ', buf);
    //return buf;
    client.write(buf);
}

function onEnd(){
    console.log('[client] 数据收发结束');
}

function onClose(){
    console.log('[client] 从服务器断开');
    client.removeListener('data', recvData);
    client.removeListener('end', onEnd);
    client.removeListener('close', onClose);
}

function onConnect(){
    client.on('data', recvData);
    client.on('end', onEnd);
    client.on('close', onClose);
}



var tcp = {};
tcp.connect = function(ip, port, name){
    client = net.connect(port, ip,
        function() {
            client.on('error', (err) =>{
                alert('出错了' + err.stack);
                AppActions.disconnected();
            });
            console.log('[连接] 连接上服务器!');
            //给服务端发送数据
            console.log('[发送] 登录数据');
            onConnect();
            //发送登录
            encodeMsgAndSend(Code.LOGIN, new Buffer(name));
        });
};

tcp.join = function(index){
    var buf = new Buffer(1);
    buf.writeUInt8(index, 0);
    encodeMsgAndSend(Code.JOIN, buf);
};

tcp.loadTables = function(){
    encodeMsgAndSend(Code.TABLES);
};

tcp.ready = function(type){
    var buf = new Buffer(1);
    buf.writeUInt8(type, 0);
    encodeMsgAndSend(Code.READY, buf);
};

tcp.call = function(isCall){
    var buf = new Buffer(1);
    buf.writeUInt8(isCall?1:2, 0);
    encodeMsgAndSend(Code.CALL, buf);
};

tcp.exit = function(){
    encodeMsgAndSend(Code.EXIT);
};

export default tcp;
