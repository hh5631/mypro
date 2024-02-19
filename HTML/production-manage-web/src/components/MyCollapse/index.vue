<!--
 * @Descripttion:
 * @version:
 * @Author: lirioing
 * @Date: 2022-07-12 09:16:55
 * @LastEditors: liquan 1106970756@qq.com
 * @LastEditTime: 2022-07-25 14:27:34
-->
<template>
  <div class="collapse">
    <el-collapse v-model="activeNames" @change="aa">
      <el-collapse-item name="basicInfo" @click="show_collapse">
        <template #title>
          <i class="my-icon iconfont icon-down" :class="{'active':isActive}" />
          <div class="content">
            <div>{{ title }}</div>
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
  color: #6A7697;
  justify-content: space-between;
}
.my-icon{
    color: #6A7697;
    margin:0 5px 0 0;
}
</style>
