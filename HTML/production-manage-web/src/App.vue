<template>
  <div id="app">
    <router-view />

    <!-- <div>
      <Popup ref="popup" />
    </div> -->
  </div>
</template>

<script>
// import Popup from './components/msgDia/popup.vue'

export default {
  name: 'App',
  // components: { Popup },
  metaInfo() {
    return {
      title: this.$store.state.settings.dynamicTitle && this.$store.state.settings.title,
      titleTemplate: title => {
        return title ? `${title} - ${process.env.VUE_APP_TITLE}` : process.env.VUE_APP_TITLE
      }
    }
  }
}
</script>
<style lang="scss">
.el-tooltip__popper {
  max-width: 800px;
}
</style>
