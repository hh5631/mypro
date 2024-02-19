<template>
  <el-select v-model="value" class="lan-selector-box" placeholder="简体中文">
    <el-option
      v-for="item in options"
      :key="item.value"
      :label="item.label"
      :value="item.value"
    />
  </el-select>
</template>

<script>
export default {
  name: 'Selector',
  data() {
    return {
      options: [
        {
          value: '选项1',
          label: '简体中文'
        },
        {
          value: '选项2',
          label: '英语'
        },
        {
          value: '选项3',
          label: '日语'
        }
      ],
      value: ''
    }
  }
}

</script>

<style scoped lang="scss">
.el-input--medium .el-input__inner{
  background:orange;
}
.lan-selector-box {
 &::v-deep .el-input--medium .el-input__inner{
   background-color:transparent;
   width: 120px;
   height: 50px;
   font-size: 16px;
   color: white;
 }
  &::v-deep .el-input__inner::-webkit-input-placeholder {
    color: rgb(255, 255, 255);
  }

}
::v-deep .el-input.is-focus .el-input__inner {
  border-color: #fff;
}
::v-deep .el-input__inner:focus{
  border-color: #fff;
}
::v-deep .el-select__caret.el-input__icon.el-icon-arrow-up{
  color: #fff;
}
</style>
