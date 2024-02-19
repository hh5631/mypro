<template>
  <iframe class="ifram" frameborder="no" :src="url" />
</template>
<script>
export default {
  name: 'FilePreview',
  props: {
    url: {
      type: String,
      default: ''
    }
  },
  data() {
    return {
    }
  },
  methods: {
  }
}

</script>
<style lang="scss" scoped>
.ifram{
  width: 47vw;
  height: 500px;
  margin-left: 20px;
}
</style>
