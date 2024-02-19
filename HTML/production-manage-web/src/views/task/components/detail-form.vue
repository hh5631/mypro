<!--
 * @Author: 751371771@qq.com 751371771@qq.com
 * @Date: 2022-09-21 09:03:13
 * @LastEditors: 751371771@qq.com 751371771@qq.com
 * @LastEditTime: 2022-10-17 10:10:28
 * @FilePath: \production-manage-web\src\views\task\components\detail-form.vue
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
-->
<template>
  <div class="detail-form">
    <div class="detail-title">
      <i class="el-icon el-icon-s-order title-icon" />
      <div class="main-title">G213412341234</div>
      <div class="second-title">流水号(G213412341234)</div>
    </div>
    <el-form inline>
      <el-form-item label="申请部门">
        <div class="form-content">{{ }}</div>
      </el-form-item>
      <el-form-item label="申请人">
        <div class="form-content">{{ }}</div>
      </el-form-item>
      <el-form-item label="申请时间">
        <div class="form-content">{{ }}</div>
      </el-form-item>
      <br>
      <el-form-item label=" 紧急程度">
        <div class="form-content">{{ }}</div>
      </el-form-item>
    </el-form>
  </div>

</template>
<script>
export default {
  data() {
    return {
      formData: {}
    }
  },
  methods: {
    init(data) {
      this.formData = data
    }
  }
}
</script>
<style lang="scss" scoped>
.detail-form {
  .form-content {
    min-width: 260px;
  }
  .detail-title {
    display: flex;
    align-items: center;
    margin-bottom: 20px;
  }
  .title-icon {
    font-size: 20px;
    color: #579BFA;
  }
  .main-title {
    font-size: 16px;
    font-weight: bold;
    color: #000000;
    margin: 0 8px;
  }
}
</style>
