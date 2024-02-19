<template>
  <div class="common-title-bar-box">
    <div class="title-box">
      <div class="title-title">
        {{ title }}
        <slot name="secondTitle" />
      </div>
      <div class="right-btn">
        <slot name="titleBtn" />
      </div>
    </div>
    <slot />
  </div>

</template>

<script>
export default {
  name: 'CommonTitleBar',
  computed: {
    title() {
      const matched = this.$route.matched.filter(item => item.meta && item.meta.title)
      return matched[matched.length - 1].meta.title
    }
  }
}
</script>
<style lang="scss" scoped>
.common-title-bar-box {
  .title-box {
    display: flex;
    justify-content: space-between;
    width: 100%;
    background: #fff;
    padding-left:24px;
    color: #333333;
  }
  .title-title {
    display: flex;
    height: 64px;
    font-size: 20px;
    font-family: SourceHanSansCN-Medium, SourceHanSansCN;
    font-weight: 500;
    color: #333333;
    line-height: 64px;
  }
  .right-btn {
    margin-right: 20px;
    line-height: 64px;
    height: 64px;
  }
}
</style>

