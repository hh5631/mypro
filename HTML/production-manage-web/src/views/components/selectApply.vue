<!--
 * @Author: liquan 1106970756@qq.com
 * @Date: 2022-07-21 09:09:54
 * @LastEditors: liquan 1106970756@qq.com
 * @LastEditTime: 2022-08-18 16:09:14
 * @FilePath: \production-manage-web\src\views\components\selectApply.vue
 * @Description:
 *
 * Copyright (c) 2022 by liquan 1106970756@qq.com, All Rights Reserved.
-->
<template>
  <div class="selectLeader">
    <el-input v-model.trim="title" style="width:40%" maxlength="15" placeholder="请输入关键词搜索" size="mini" />
    <el-button size="small" @click="submitForm">重置</el-button>
    <el-button size="small" style="margin-left:16px" class="search-btn" @click="cancel">搜索</el-button>
    <el-table
      ref="rowRef"
      v-loading="loading"
      :row-key="getRowKeys"
      highlight-current-row
      :data="userList"
      style="margin-top: 10px;"
      @selection-change="handleRoleSelectionChange"
    >
      <el-table-column type="selection" width="50" :reserve-selection="true" align="center" />
      <el-table-column type="index" width="100" label="序号" />
      <el-table-column key="userName" label="供应商名称" align="center" prop="userName" :show-overflow-tooltip="true" />
      <el-table-column key="userName" label="供应商代码" align="center" prop="userName" :show-overflow-tooltip="true" />
      <el-table-column key="jobNumber" label="供应商联系人" align="center" prop="jobNumber" />
      <el-table-column key="jobNumber" label="供应商电话" align="center" prop="jobNumber" />
    </el-table>
  </div>
</template>

<script>
export default {
  data() {
    return {
      title: '',
      addApplyDig: false
    }
  },
  methods: {
    addApply() {
      this.addApplyDig = true
    }
  }
}
</script>

<style lang="scss" scoped>
.selectLeader{
  width: 100%;
  height: 100%;
  padding: 16px;
  overflow-y: scroll;
}
</style>
