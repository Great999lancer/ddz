'use strict';
const React = require('react');
const mui = require('material-ui'),
    Paper = mui.Paper,
    TextField = mui.TextField,
    FlatButton = mui.FlatButton;

const Transition = require('react-router').Transition;
const AppActions = require('../actions/AppActions');
const AppStore = require('../stores/AppStore');
const router = require('../router.js');

export default  React.createClass({

  getInitialState(){
    return {
      ip: '192.168.100.3',
      port: '8181',
      name: 'CUI',
      isLogin: AppStore.getIsLogin(),
      msg: AppStore.getMsg()
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
      isLogin: AppStore.getIsLogin(),
      msg: AppStore.getMsg()
    });
  },
  handleIPChange(e){
    this.setState({ip: e.target.value});
  },
  handlePortChange(e){
    this.setState({port: e.target.value});
  },
  handleNameChange(e){
    this.setState({name: e.target.value});
  },
  handleLogin(){
    AppActions.login(this.state.ip, parseInt(this.state.port), this.state.name);
  },
  render() {
    return (
      <div className="verticalCenter">
        <Paper className="login" zDepth={1}>
          <h2>男人的斗地主</h2>
          <h3>登录</h3>
          <TextField hintText="来一发地址"
                     floatingLabelText="服务器地址" value={this.state.ip} onChange={this.handleIPChange}/>
          <TextField hintText="来一发端口号"
                     floatingLabelText="端口号" value={this.state.port} onChange={this.handlePortChange}/>
          <TextField hintText="来一发昵称"
                     floatingLabelText="昵称" value={this.state.name} onChange={this.handleNameChange}/>
          <p>{this.state.msg}</p>
          <FlatButton className="loginButton" label={this.state.isLogin?'登录中……':'登录'} primary={true} onClick={this.handleLogin}/>
        </Paper>
      </div>
    );
  }
});