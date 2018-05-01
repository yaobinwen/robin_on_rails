import React from 'react'
import ViewBaseComponent from "./ViewBase.js"
import logo from '../logo.svg';
import '../App.css'

class DefaultReactView extends ViewBaseComponent {

  static get_name() {
    return "React Default View"
  }

  render() {
    return (
      <div className="App">
        <header className="App-header">
          <img src={logo} className="App-logo" alt="logo" />
          <h1 className="App-title"> Welcome to React </h1>
        </header>
        <p className="App-intro">
          To get started, edit <code>src/App.js</code> and save to reload.
        </p>
      </div>
    )
  }
}

export default DefaultReactView