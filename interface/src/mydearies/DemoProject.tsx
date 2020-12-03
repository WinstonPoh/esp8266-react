import React, { Component } from 'react';
import { Redirect, Switch, RouteComponentProps } from 'react-router-dom'

import { Tabs, Tab } from '@material-ui/core';

import { PROJECT_PATH } from '../api';
import { MenuAppBar } from '../components';
import { AuthenticatedRoute } from '../authentication';

import DemoInformation from './DemoInformation';
import LightStateRestController from './LightStateRestController';
import LightStateWebSocketController from './LightStateWebSocketController';
import LightMqttSettingsController from './LightMqttSettingsController';
// import MoistureSensorStateRestController from './MoistureSensorStateRestController';
import MoistureSensorStateWebSocketController from './MoistureSensorStateWebSocketController';

class DemoProject extends Component<RouteComponentProps> {

  handleTabChange = (event: React.ChangeEvent<{}>, path: string) => {
    this.props.history.push(path);
  };

  render() {
    return (
      <MenuAppBar sectionTitle="Plants">
        <Tabs value={this.props.match.url} onChange={this.handleTabChange} variant="fullWidth">
          <Tab value={`/${PROJECT_PATH}/plants/information`} label="Information" />
          <Tab value={`/${PROJECT_PATH}/plants/rest`} label="REST Controller" />
          <Tab value={`/${PROJECT_PATH}/plants/socket`} label="WebSocket Controller" />
          <Tab value={`/${PROJECT_PATH}/plants/moisturesocket`} label="Moisture WebSocket Controller" />
          <Tab value={`/${PROJECT_PATH}/plants/mqtt`} label="MQTT Controller" />
        </Tabs>
        <Switch>
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/plants/information`} component={DemoInformation} />
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/plants/rest`} component={LightStateRestController} />
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/plants/socket`} component={LightStateWebSocketController} />
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/plants/moisturesocket`} component={MoistureSensorStateWebSocketController} />
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/plants/mqtt`} component={LightMqttSettingsController} />
          <Redirect to={`/${PROJECT_PATH}/plants/information`} />
        </Switch>
      </MenuAppBar>
    )
  }

}

export default DemoProject;
