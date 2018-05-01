import React from 'react'
import ViewBaseComponent from "./ViewBase.js"
import StarryNight from "../images/starry-night.png"

class StarryNightView extends ViewBaseComponent {

  static get_name() {
    return "Starry Night View"
  }

  render() {
    return (
      <p>
        <img src={StarryNight} />
      </p>
    )
  }
}

export default StarryNightView;
