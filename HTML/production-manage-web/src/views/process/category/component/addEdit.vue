<!--
 * @Author: liquan 1106970756@qq.com
 * @Date: 2022-04-22 14:40:27
 * @LastEditors: liquan 1106970756@qq.com
 * @LastEditTime: 2022-09-08 16:30:22
 * @FilePath: \production-manage-web\src\views\process\category\component\addEdit.vue
 * @Description:
 *
 * Copyright (c) 2022 by liquan 1106970756@qq.com, All Rights Reserved.
-->
<template>
  <el-dialog :title="title" :visible.sync="dialogVisible" :before-close="handleClose">
    <el-form ref="categoryForm" class="add-edit-form" :model="form" :rules="rules" label-width="80px">
      <el-form-item label="名称" prop="categoryName">
        <el-input v-model.trim="form.categoryName" placeholder="请输入" clearable maxlength="15" />
      </el-form-item>
      <!-- <el-form-item label="显示顺序" prop="orderNum">
        <el-input
          v-model="form.orderNum"
          v-trim.positiveInt="form.orderNum"
          placeholder="请输入"
          clearable
          maxlength="5"
        />
      </el-form-item> -->
      <el-form-item label="备注">
        <el-input
          v-model="form.remark"
          placeholder="请输入"
          clearable
          type="textarea"
          maxlength="500"
        />
      </el-form-item>
    </el-form>
    <div slot="footer">
      <el-button size="small" @click="cancelMethod">取 消</el-button>
      <el-button
        v-loading="loading"
        type="primary"
        size="small"
        @click="addEditFun('categoryForm')"
      >保 存</el-button>
    </div>
  </el-dialog>
</template>
<script>
import { addProcessCategory, updateProcessCategory } from '@/api/process'
export default {
  data() {
    return {
      dialogVisible: false,
      form: {
        categoryName: '',
        // orderNum: '',
        remark: ''
      },
      rules: {
        categoryName: [{ required: true, message: '请输入', trigger: 'blur' }]
      },
      loading: false,
      title: '新增类别'
    }
  },
  methods: {
    addEditFun(formName) {
      this.$refs[formName].validate((valid) => {
        if (valid) {
          this.loading = true
          if (this.form.id) {
            updateProcessCategory(this.form)
              .then(() => {
                this.$emit('reload')
                this.dialogVisible = false
                this.$message.success('编辑成功!')
              })
              .finally(() => {
                this.loading = false
              })
          } else {
            addProcessCategory(this.form)
              .then(() => {
                this.$emit('reload')
                this.dialogVisible = false
                this.$message.success('新增成功!')
              })
              .finally(() => {
                this.loading = false
              })
          }
        } else {
          return false
        }
      })
    },
    openDialog(val) {
      this.form = { ...val }
      this.title = '编辑类别'
      if (!val) {
        this.title = '新增类别'
        this.form = {
          categoryName: '',
          remark: ''
        }
      }
      this.dialogVisible = true
    },
    handleClose(done) {
      done()

      this.$refs.categoryForm.fields.map(i => {
        if (i.prop === 'categoryName') {
          // 清空验证
          i.clearValidate()
        }
      })
    },
    cancelMethod() {
      this.dialogVisible = false
      this.$refs.categoryForm.fields.map(i => {
        if (i.prop === 'categoryName') {
          // 清空验证
          i.clearValidate()
        }
      })
    }
  }
}
</script>
<style lang="scss" scoped>
.add-edit-form {
  border: 0;
  margin: 0 111px;
}
</style>
