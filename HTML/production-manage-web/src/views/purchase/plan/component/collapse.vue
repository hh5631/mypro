<!--
 * @Descripttion:
 * @version:
 * @Author: lirioing
 * @Date: 2022-07-04 14:54:09
 * @LastEditors: lirioing
 * @LastEditTime: 2022-08-09 14:28:55
-->
<template>
  <div class="collapse">
    <el-collapse v-model="activeNames" @change="aa">
      <el-collapse-item name="basicInfo" @click="show_collapse">
        <template #title>
          <i class="iconfont icon-down" :class="{'active':isActive}" style="margin:0 5px;color:#6A7697;" />
          <div class="content">
            <div class="content-title">{{ title }}</div>
            <div class="btn">
              <slot name="button" />
            </div>
          </div>
        </template>
        <slot />
      </el-collapse-item>
    </el-collapse>
  </div>
</template>

<script>
export default {
  name: 'Collapse',
  props: {
    title: {
      type: String,
      default: ''
    }
  },
  data() {
    return {
      isActive: false,
      activeNames: ['basicInfo']
    }
  },
  methods: {
    show_collapse() {

      // this.isActive = !this.isActive
    },
    aa() {
      this.isActive = !this.isActive
      this.$forceUpdate()
    }
  }
}
</script>

<style lang="scss" scoped>
 .active {
  transform: rotate(-90deg);
}
 ::v-deep .el-collapse-item__arrow {
  display: none
 }
 ::v-deep .el-collapse-item__header{
   width: 100%;
   border-bottom: none;
 }
.content{
  width: 100%;
  display: flex;
  justify-content: space-between;
}
.content-title{
font-size: 18px;
font-weight: 500;
color: #6A7697;
}
</style>
