import React from 'react'
import ReactDOM from 'react-dom'
import keybinding from './app/keybinding'

import {
    Window,
    Text,
    View,
    StackedView,
    Button
} from './js-qt-native/react/lib/core'

const App = () => {
    const [ state, setState ] = React.useState({
        current: null
    })

    const goAdvance = (evt) => {
        setState({
            current: 'panel::advance'
        })
    }
    const goSearch = async (evt) => {
        setState({
            current: 'panel::search'
        })
    }

    return <React.Fragment>
    {/*
        <StackedView id="panels" current={state.current}>
            <View id="panel::search">
                <Text>search</Text>
                <Button text="go advance" onClick={goAdvance}></Button>
            </View>
            <View id="panel::advance">
                <Text>advance search</Text>
                <Button text="go search" onClick={goSearch}></Button>
            </View>
        </StackedView>
    */}
    </React.Fragment>
}

ReactDOM.render(<App/>, document.querySelector('#root'));