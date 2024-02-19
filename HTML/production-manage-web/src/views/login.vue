<template>
  <div class="background">
    <login-layout />
  </div>
</template>

<script>
import loginLayout from '../components/login-layout/login-layout.vue'
export default {
  name: 'Login',
  components: { loginLayout }
}
</script>

<style scoped lang="scss">
.background{
  width: 100%;
  height: 100%;
}

</style>
