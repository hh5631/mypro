<template>
  <div class="app-container home">
    <div class="hello-box">
      <el-image
        class="hello-bg"
        :src="helloImgSrc"
        fit="cover"
      />
      <div class="copy-right">© Copyright Reserved 浙江大学嘉兴研究院版权所有 {{ version }}</div>
    </div>
  </div>
</template>

<script>
import imgSrc from '@/assets/images/hello.png'
export default {
  name: 'Index',
  data() {
    return {
      // 版本号
      version: 'V0.1.0',
      helloImgSrc: imgSrc
    }
  },
  mounted() {},
  methods: {
    goTarget(href) {
      window.open(href, '_blank')
    }
  }
}
</script>

<style scoped lang="scss">
.home {
  padding: 16px;
  height: calc(100vh - 56px);
  box-sizing: border-box;

  .hello-box {
    height: 100%;
    position: relative;
    .hello-bg{
      width:100%;
      height: 100%;
      position:absolute;
      top: 0;
      left: 0;
      z-index: 1;
    }
  }
  .copy-right {
    font-size: 14px;
    font-family: PingFangSC-Regular, PingFang SC;
    font-weight: 400;
    color: #15599f;
    text-align: center;
    position: absolute;
    left: 0;
    right: 0;
    bottom: 40px;
    z-index: 9;
  }
  blockquote {
    padding: 10px 20px;
    margin: 0 0 20px;
    font-size: 17.5px;
    border-left: 5px solid #eee;
  }
  hr {
    margin-top: 20px;
    margin-bottom: 20px;
    border: 0;
    border-top: 1px solid #eee;
  }
  .col-item {
    margin-bottom: 20px;
  }

  ul {
    padding: 0;
    margin: 0;
  }

  font-family: "open sans", "Helvetica Neue", Helvetica, Arial, sans-serif;
  font-size: 13px;
  color: #676a6c;
  overflow-x: hidden;

  ul {
    list-style-type: none;
  }

  h4 {
    margin-top: 0px;
  }

  h2 {
    margin-top: 10px;
    font-size: 26px;
    font-weight: 100;
  }

  p {
    margin-top: 10px;

    b {
      font-weight: 700;
    }
  }

  .update-log {
    ol {
      display: block;
      list-style-type: decimal;
      margin-block-start: 1em;
      margin-block-end: 1em;
      margin-inline-start: 0;
      margin-inline-end: 0;
      padding-inline-start: 40px;
    }
  }
}
</style>
