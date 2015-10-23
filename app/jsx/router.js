'use strict';

import routes from './routes';
import Router from 'react-router';

const router = Router.create({
    routes: routes,
    location: Router.HashLocation
});

export default router;
