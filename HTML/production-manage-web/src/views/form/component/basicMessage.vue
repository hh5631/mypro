<template>
  <el-form ref="form" class="basic-form" :model="form" :rules="rules" label-width="80px" label-position="top">
    <el-form-item label="表单名称" prop="formName">
      <el-input
        v-model="form.formName"
        style="width:600px"
        placeholder="请输入"
        maxlength="15"
        clearable
      />
    </el-form-item>
    <el-form-item label="表单类型" prop="categoryId">
      <el-select v-model="form.categoryId" style="width:600px" clearable>
        <el-option
          v-for="(item, index) in processCategoryList"
          :key="index"
          :value="item.id"
          :label="item.categoryName"
        />
      </el-select>
    </el-form-item>
    <!-- <el-form-item label="排序" prop="orderNum">
      <el-input
        v-model="form.orderNum"
        v-trim.positiveInt="form.orderNum"
        placeholder="请输入"
        style="width:600px"
        maxlength="5"
        clearable
      />
    </el-form-item> -->
    <!-- <el-form-item label="备注" prop="status">
      <el-input
        v-model="form.menuName"
        placeholder="请输入"
        clearable
        size="mini"
        type="textarea"
      />
    </el-form-item> -->
  </el-form>
</template>
<script>
import { mapGetters } from 'vuex'
import { judgeFormNameRepeat } from '@/api/process'
import FormValidators from '../../../vform/utils/validators'
export default {
  props: ['formName'],
  data() {
    return {
      form: {
        categoryId: '',
        formName: ''
        // orderNum: ''
      },
      rules: {
        formName: [
          { required: true, trigger: 'blur', message: '请输入' },
          { validator: FormValidators.chineseAndLetterAndNumber, message: '只能输入中英文或者数字', trigger: 'blur' },
          { required: true, trigger: 'change', validator: this.judgeFormNameRepeat }
        ],
        categoryId: [
          { required: true, message: '请选择', trigger: ['blur', 'change'] }
        ]
      }
    }
  },
  computed: {
    ...mapGetters(['processCategoryList'])
  },
  methods: {
    judgeFormNameRepeat(rule, value, callback) {
      if (!value) {
        callback(new Error('请输入'))
        return
      }
      if (this.formName === value) {
        callback()
        return
      }
      judgeFormNameRepeat(value).then(da => {
        if (da.data) {
          callback(new Error('表单名称不能相同'))
        } else {
          callback()
        }
      })
    },
    submitForm() {
      this.$refs['form'].validate((valid) => {
        if (valid) {
          this.$emit('next', this.form)
        } else {
          return false
        }
      })
    }
  }
}
</script>
<style lang="scss" scoped>
.basic-form{
  padding: 20px;
  width: 600px;
  margin: auto;
}
</style>
