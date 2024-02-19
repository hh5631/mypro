/*
 * @Author: liquan 1106970756@qq.com
 * @Date: 2022-04-22 14:40:27
 * @LastEditors: liquan 1106970756@qq.com
 * @LastEditTime: 2022-08-16 09:13:50
 * @FilePath: \production-manage-web\src\store\getters.js
 * @Description:
 *
 * Copyright (c) 2022 by liquan 1106970756@qq.com, All Rights Reserved.
 */
import store from './index'
const getters = {
  sidebar: (state) => state.app.sidebar,
  size: (state) => state.app.size,
  device: (state) => state.app.device,
  visitedViews: (state) => state.tagsView.visitedViews,
  cachedViews: (state) => state.tagsView.cachedViews,
  token: (state) => state.user.token,
  avatar: (state) => state.user.avatar,
  name: (state) => state.user.name,
  deptId: (state) => state.user.deptId,
  introduction: (state) => state.user.introduction,
  roles: (state) => state.user.roles,
  permissions: (state) => state.user.permissions,
  permission_routes: (state) => state.permission.routes,
  topbarRouters: (state) => state.permission.topbarRouters,
  defaultRoutes: (state) => state.permission.defaultRoutes,
  sidebarRouters: (state) => state.permission.sidebarRouters,
  processCategoryList: (state) => {
    if (state.flow.processCategoryList) {
      return state.flow.processCategoryList
    } else {
      store.commit('flow/SET_PC_LIST')
    }
  }
}
export default getters
