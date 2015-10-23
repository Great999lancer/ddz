'use strict';

import React from 'react';
import {Paper, TextField, FlatButton} from 'material-ui';
import AppStore from '../stores/AppStore';
import Card from './Card.jsx';

export default React.createClass({
    render() {
        return (
            <div className={`others${this.props.right?' right':''}`}>

                <div className="user">
                    <p className="name">{this.props.nickname}</p>
                    {this.props.master?'地主！':''}
                </div>
                <ul className="cards">
                    {
                        this.props.cards?
                            this.props.cards.map(
                                code => <li key={code}><Card code={code} hide={this.props.hide}/></li>

                            ):<p></p>
                    }
                </ul>
            </div>
        );
    }

});