import React, { Component } from 'react';
import { Link, withRouter, RouteComponentProps } from 'react-router-dom';

import {List, ListItem, ListItemIcon, ListItemText} from '@material-ui/core';
import SettingsRemoteIcon from '@material-ui/icons/SettingsRemote';
import EcoIcon from '@material-ui/icons/Eco';

import { PROJECT_PATH } from '../api';

class ProjectMenu extends Component<RouteComponentProps> {

  render() {
    const path = this.props.match.url;
    return (
      <List>
        <ListItem to={`/${PROJECT_PATH}/plants/`} selected={path.startsWith(`/${PROJECT_PATH}/plants/`)} button component={Link}>
          <ListItemIcon>
            <EcoIcon />
          </ListItemIcon>
          <ListItemText primary="Plants" />
        </ListItem>
      </List>
    )
  }

}

export default withRouter(ProjectMenu);
