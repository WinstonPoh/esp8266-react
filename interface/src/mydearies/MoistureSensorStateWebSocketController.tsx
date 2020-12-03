import React, { Component } from 'react';
import { ValidatorForm } from 'react-material-ui-form-validator';

import { Typography, Box, Switch, Slider } from '@material-ui/core';
import { WEB_SOCKET_ROOT } from '../api';
import { WebSocketControllerProps, WebSocketFormLoader, WebSocketFormProps, webSocketController } from '../components';
import { SectionContent, BlockFormControlLabel } from '../components';

import { LightState } from './types';

export const LIGHT_SETTINGS_WEBSOCKET_URL = WEB_SOCKET_ROOT + "lightState";

type MoistureSensorStateWebSocketControllerProps = WebSocketControllerProps<LightState>;

class MoistureSensorStateWebSocketController extends Component<MoistureSensorStateWebSocketControllerProps> {

  render() {
    return (
      <SectionContent title='Moisture WebSocket Controller' titleGutter>
        <WebSocketFormLoader
          {...this.props}
          render={props => (
            <MoistureSensorStateWebSocketControllerForm {...props} />
          )}
        />
      </SectionContent>
    )
  }

}

export default webSocketController(LIGHT_SETTINGS_WEBSOCKET_URL, 100, MoistureSensorStateWebSocketController);

type MoistureSensorStateWebSocketControllerFormProps = WebSocketFormProps<LightState>;

function valuetext(value: number) {
  return `${value}`;
}

function MoistureSensorStateWebSocketControllerForm(props: MoistureSensorStateWebSocketControllerFormProps) {
  const { data, saveData, setData } = props;

  const changeLedOn = (event: any, newValue: number | number[]) => {
    setData({ led_on: event.target.checked, led_brightness: String(newValue), max_brightness: '100' }, saveData);
    // setData({ led_on: event.target.checked }, saveData);
  }

  return (
    <ValidatorForm onSubmit={saveData}>
      <Box bgcolor="primary.main" color="primary.contrastText" p={2} mt={2} mb={2}>
        <Typography variant="body1">
          The switch below controls the LED via the WebSocket. It will automatically update whenever the LED state changes.
        </Typography>
      </Box>
      <BlockFormControlLabel
        control={
          <Slider
              defaultValue={30}
              value={parseInt(data.led_brightness)}
              onChange={changeLedOn}
              getAriaValueText={valuetext}
              aria-labelledby="discrete-slider"
              valueLabelDisplay="auto"
              step={10}
              marks
              min={10}
              max={110}
          />
      }
        // control={
        //   <Switch
        //     checked={data.led_on}
        //     onChange={changeLedOn}
        //     color="primary"
        //   />
        // }
        label="LED State?"
      />
    </ValidatorForm>
  );
}
