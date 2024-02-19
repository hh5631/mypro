<!--
 * @Author: liquan 1106970756@qq.com
 * @Date: 2022-07-25 10:21:07
 * @LastEditors: liquan 1106970756@qq.com
 * @LastEditTime: 2022-07-25 10:52:29
 * @FilePath: \production-manage-web\src\components\FunctionBtn\index.vue
 * @Description:
 *
 * Copyright (c) 2022 by liquan 1106970756@qq.com, All Rights Reserved.
-->
<template>
  <el-button
    plain
    size="small"
  ><i style="color: #4686ef" class="iconfont" :class="icon" />
    <span v-show="name" class="btn-text">
      {{ name }}
    </span>
  </el-button>
</template>
<script>
export default {
  props: {
    icon: {
      type: String,
      default: ''
    },
    name: {
      type: String,
      default: ''
    }
  }
}
</script>
<style lang="scss" scoped>
.btn-text{
  vertical-align: text-top;
  margin-left: 5px;
}
</style>
