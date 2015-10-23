/* @jsx React.DOM */

const React = require('react');
const injectTapEventPlugin = require("react-tap-event-plugin");

const mui = require('material-ui');
const router = require('./router');
const routes = require('./routes');
injectTapEventPlugin();

router.run(function (Handler) {
    React.render(<Handler/>, document.body);
});