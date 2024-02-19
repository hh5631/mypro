<!--
 * @Author: liquan 1106970756@qq.com
 * @Date: 2022-07-25 13:55:12
 * @LastEditors: liquan 1106970756@qq.com
 * @LastEditTime: 2022-07-25 16:14:35
 * @FilePath: \production-manage-web\src\components\StockForm\index.vue
 * @Description:
 *
 * Copyright (c) 2022 by liquan 1106970756@qq.com, All Rights Reserved.
-->
<template>
  <div>
    <el-form ref="form" :model="formData" :rules="rules" size="mini">
      <el-row :gutter="24">
        <el-col
          v-for="(item, index) in colums"
          :key="index"
          :span="item.span || 6"
        >
          <el-form-item
            :label="item.label"
            :prop="item.prop"
            :class="{ readOnly: item.readonly }"
          >
            <slot :name="item.value">
              <el-input
                v-model="formData[item.value]"
                :readonly="item.readonly"
              />
            </slot>
          </el-form-item>
        </el-col>
      </el-row>
    </el-form>
  </div>
</template>
<script>
export default {
  props: {
    // 表单值
    data: {
      type: Object,
      default: () => {
        return {}
      }
    },
    // 表单字段
    /**
     * @description:
     * @return
     * {
     * label,//字段名
     * value,//字段
     * prop,//表单验证字段
     * span,//宽
     * readonly//只读true/flase
     * }
     */
    colums: {
      type: Array,
      default: () => {
        return []
      }
    },
    rules: {
      type: Object,
      default: () => {
        return {}
      }
    }
  },
  data() {
    return {
      formData: {}
    }
  },
  watch: {
    data: {
      handler(val) {
        this.formData = { ...val }
      },
      deep: true,
      immediate: true
    }
  },
  methods: {
    validate() {
      this.$refs.form.validate((valid) => {
        if (valid) {
          this.$emit('validate', this.formData)
        } else {
          this.$emit('validate', false)
        }
      })
    }
  }
}
</script>
<style lang="scss">
.readOnly {
  .el-input__inner {
    background: #f5f8fc;
  }
}
</style>
