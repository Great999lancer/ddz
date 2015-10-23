'use strict';

import React from 'react';
import {Paper, TextField, FlatButton} from 'material-ui';

const type = ['♠', '♥', '♣', '♦'];
const amount = ['3', '4', '5', '6', '7', '8', '9', '10', 'J', 'Q', 'K', 'A', '2'];
export default React.createClass({


    render() {
        if(this.props.hide){
            return (
                <Paper className="hide"></Paper>
            )
        }


        let words;
        let color;
        let code = this.props.code;
        if(code===0x5e){
            words = '小怪';
            color = 'grey'
        }
        else if(code===0x5f){
            words = '大怪';
        }
        else {
            let index = (code / 16)|0;
            if((index/2)|0){
                color = 'red'
            }
            let num = amount[(code % 16) - 1];
            words = type[index - 1] + num;
        }
        return (
            <Paper><span className={color}>️{words}</span></Paper>
        )

    }
});