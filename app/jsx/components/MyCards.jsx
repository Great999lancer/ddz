'use strict';

import React from 'react';
import {Paper, TextField, FlatButton} from 'material-ui';
import AppStore from '../stores/AppStore';
import Card from './Card.jsx';

export default React.createClass({



    getInitialState(){
      return {selected:[]}
    },

    handleSelect(i){
        return ()=>{
            let selected = this.state.selected;
            let found = false;
            for(let j = 0;j<selected.length; j++){

                if(selected[j]===i)
                {
                    found = true;
                    selected[j] = selected.splice(j, 1);
                    this.setState({selected});
                    break;
                }
            }
            if(!found){
                selected.push(i);
                this.setState({selected});
            }
        }
    },
    render() {
        return (
            <div className="myCards">
                <ul className="cards">
                    {
                        this.props.cards?
                            this.props.cards.map(
                                (code, i) => {
                                    let selected = this.state.selected;
                                    let needSelect = false;
                                    for(let j =0;j<selected.length; j++){
                                        if(selected[j]===i)
                                        {
                                            needSelect = true;
                                            break;
                                        }
                                    }

                                    return <li key={code} className={needSelect?'selected':''} onClick={this.handleSelect(i)}><Card code={code} hide={this.props.hide}/></li>
                                }
                            ):<p>还没开始</p>
                    }

                </ul>
            </div>
        );
    }

});