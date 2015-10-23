'use strict';
import React from 'react';
import Home from './components/Home.jsx';
import Rooms from './components/Rooms.jsx';
import Game from './components/Game.jsx';

import {Route, RouteHandler, DefaultRoute} from 'react-router';

const App = React.createClass({
    render: function () {
        return (
            <div className="container">
                <header>
                </header>
                <RouteHandler/>
            </div>
        );
    }
});

export default (
    <Route handler={App} path="/">
        <DefaultRoute handler={Home} />
        <Route name="game" handler={Game} />
        <Route name="rooms" handler={Rooms} />
    </Route>
); //HOME