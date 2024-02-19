<template>
  <div>
    <el-breadcrumb class="app-breadcrumb" separator-class="iconfont icon-left2">
      <transition-group name="breadcrumb">
        <el-breadcrumb-item v-for="(item,index) in levelList" :key="item.path">
          <span v-if="item.redirect === 'noRedirect' || index == levelList.length - 1" class="no-redirect">{{ item.meta.title }}</span>
          <a v-else @click.prevent="handleLink(item)">{{ item.meta.title }}</a>
        </el-breadcrumb-item>
      </transition-group>
    </el-breadcrumb>
  </div>

</template>

<script>
export default {
  data() {
    return {
      levelList: null
    }
  },
  watch: {
    $route(route) {
      // if you go to the redirect page, do not update the breadcrumbs
      if (route.path.startsWith('/redirect/')) {
        return
      }
      this.getBreadcrumb()
    }
  },
  created() {
    this.getBreadcrumb()
  },
  methods: {
    getBreadcrumb() {
      // only show routes with meta.title
      const matched = this.$route.matched.filter(item => item.meta && item.meta.title)
      // const first = matched[0]
      const title = matched[matched.length - 1].meta.title
      this.$emit('title', title)

      // if (!this.isDashboard(first)) {
      //   matched = [{ path: '/index', meta: { title: '首页' }}].concat(matched)
      // }

      this.levelList = matched.filter(item => item.meta && item.meta.title && item.meta.breadcrumb !== false)
      // this.levelList[0].meta.title = '个人工作台'
    },
    isDashboard(route) {
      const name = route && route.name
      if (!name) {
        return false
      }
      return name.trim() === 'Index'
    },
    handleLink(item) {
      const { redirect, path } = item
      if (redirect) {
        this.$router.push(redirect)
        return
      }
      this.$router.push(path)
    }
  }
}
</script>

<style lang="scss" scoped>
.app-breadcrumb.el-breadcrumb {
  display: inline-block;
  font-size: 14px;
  line-height: 60px;
  // margin-left: 8px;

  .no-redirect {
    color: #333333;
    cursor: text;
    font-size: 18px;
    font-family: SourceHanSansCN-Regular, SourceHanSansCN;
    font-weight: 400;
  }
}
</style>
