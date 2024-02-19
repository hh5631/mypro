<template>
  <div class="topBar">
    <div ref="barLeft" class="left">
      <div class="left-bar" :class="{'active':activeIndex==0}" @click="activeChange(0)">所有计划</div>
      <div class="left-bar" :class="{'active':activeIndex==1}" @click="activeChange(1)">我发起的</div>
      <div class="active-border" :style="lineStyle" />
    </div>
    <div class="right">
      <el-button size="small">打印单据</el-button>
      <el-button type="primary" size="small" @click="addPlan">新建采购</el-button>
    </div>
  </div>
</template>
<script>
export default {
  data() {
    return {
      activeIndex: 0,
      lineStyle: {}
    }
  },
  mounted() {
    this.changeLineWidth()
  },
  methods: {
    activeChange(index) {
      this.activeIndex = index
      this.changeLineWidth()
    },
    changeLineWidth() {
      const ele = this.$refs.barLeft.getElementsByClassName('left-bar')[this.activeIndex]
      const left = (ele.offsetWidth - 48) / 2 + ele.offsetLeft + 'px'
      this.lineStyle = { left: left }
    },
    addPlan() {
      this.$router.push('/addPlan')
    }
  }
}
</script>
<style lang="scss" scoped>
.topBar {
  display: flex;
  justify-content: space-between;
  height: 56px;
  line-height: 56px;
  background: #fff;
  .left {
    font-size: 24px;
    font-family: SourceHanSansCN-Medium, SourceHanSansCN;
    font-weight: 500;
    color: #333333;
    display: flex;
    position: relative;
    .left-bar {
      margin: 0 16px;
      cursor: pointer;
      &:first-of-type {
        margin-left: 24px;
      }
    }
    .active{
        color: #4686EF;
    }
    .active-border{
        position: absolute;
        bottom: 0;
        width: 48px;
        height: 2px;
        background: #4686EF;
        border-radius: 2px;
        transition: all 0.5s;
    }
  }
  .right {
      margin-right: 16px;
  }
}
</style>
