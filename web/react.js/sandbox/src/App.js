import React, { Component } from 'react';
import './App.css';
import DefaultReactView from './components/DefaultReactView.js'
import StarryNightView from './components/StarryNightView.js'

class App extends Component {
  render() {
    var now = new Date()
    let sec = now.getSeconds()
    let currViewClass = sec % 2 === 0 ? DefaultReactView : StarryNightView
    let currView = React.createElement(currViewClass)

    return (
      <div>
        <h1> {currViewClass.get_name()} </h1>
        <hr />
        {currView}
      </div>
    );
  }
}

export default App;
