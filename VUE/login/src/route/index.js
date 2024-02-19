
import { createRouter, createWebHistory } from 'vue-router';
import App from '../App.vue'
import Register from '../view/register.vue'
import Login from '../view/usr_login.vue'
import Mypage from '../view/mypage.vue'

const routes = [
        {
          path: '/login',
          component:Login
        },
        {
          path: '/',
          component: Login
        },
        {
          path: '/register',
          component: Register
        },
        {
          path: '/mypage',
          component: Mypage
        }
      ];

const router=createRouter(
        {

          history: createWebHistory(),
          routes: routes,
        }
)

export default router