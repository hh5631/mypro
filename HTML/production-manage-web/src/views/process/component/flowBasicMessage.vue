<template>
  <!-- 流程列表复制后新建 -->
  <el-form
    ref="form"
    class="basic-form"
    :model="form"
    :rules="rules"
    label-position="top"
  >
    <!-- 流程列表页面 -->
    <el-form-item label="流程名称" prop="processName">
      <el-input
        v-model="form.processName"
        style="width: 600px"
        placeholder="请输入"
        maxlength="15"
        clearable
      />
    </el-form-item>
    <el-form-item ref="selectData" label="流程类别" prop="categoryId">
      <el-select v-model="form.categoryId" style="width: 600px" clearable @change="changeMethod">
        <el-option
          v-for="(item, index) in processCategoryList"
          :key="index"
          :value="item.id"
          :label="item.categoryName"
        />
      </el-select>
    </el-form-item>
    <el-form-item label="流程类型" prop="type" required>
      <template #label>
        流程类型
        <el-tooltip effect="dark" content="自定义流程使用动态表单, 业务流程用于复杂表单和逻辑(固定在代码中), 使用业务名称和业务类型(均有后端提供)直接和代码匹配" placement="top-start">
          <i class="el-icon el-icon-info explain" />
        </el-tooltip>
      </template>
      <el-radio-group v-model="form.type">
        <el-radio v-for="item in tabList" :key="item.value" :label="item.value">{{ item.label }}</el-radio>
      </el-radio-group>
    </el-form-item>
    <el-form-item v-if="form.type === '0'" :key="0" ref="categoryFormData" label="关联表单" prop="formId">
      <el-cascader
        v-model="categoryForm"
        style="width: 600px"
        :options="formList"
        :props="{ expandTrigger: 'hover' }"
        filterable
        @change="handleChange"
      >
        <template slot-scope="{ data }">
          <el-popover
            v-show="data.formJson"
            placement="right"
            trigger="hover"
            width="720"
          >
            <v-form-render
              :form-json="data.formJson?JSON.parse(data.formJson):{}"
            />
            <el-button slot="reference" type="text" class="popover-btn" size="small">{{ data.label }}</el-button>
          </el-popover>
          <span v-show="!data.formJson">{{ data.label }}</span>
        </template>
      </el-cascader>
    </el-form-item>
    <el-form-item v-if="form.type === '1'" :key="1" label="业务名称" prop="bizName">
      <el-input
        v-model="form.bizName"
        placeholder="请输入"
        style="width:600px"
        clearable
      />
    </el-form-item>
    <el-form-item v-if="form.type === '1'" :key="2" label="业务类型" prop="bizType">
      <el-input
        v-model="form.bizType"
        placeholder="请输入"
        style="width:600px"
        clearable
      />
    </el-form-item>
    <!-- <el-form-item label="排序">
      <el-input
        v-model="form.orderNum"
        v-trim.positiveInt="form.orderNum"
        placeholder="请输入"
        maxlength="5"
        style="width:600px"
        clearable
      />
    </el-form-item> -->
  </el-form>
</template>
<script>
import { mapGetters } from 'vuex'
import { judgeFlowNameRepeat, judgeBizTypeRepeat } from '@/api/process'
import { getFormCascaderData } from '@/utils/process'

export default {
  data() {
    return {
      type: 1, // type 2为编辑 , undefined为新增 1另存为
      oldProcessName: '', // 编辑时保存旧的流程名字  (用于编辑时如果未改变 ,则无需远程验证该项是否已经存在)
      bizType: '', // 编辑时保存旧的流程类型 (用于编辑时如果未改变 ,则无需远程验证该项是否已经存在)
      form: {
        categoryId: '',
        processName: '',
        formId: '',
        orderNum: '',
        bizName: '',
        bizType: '',
        type: '0'

      },
      rules: {
        processName: [{
          required: true, message: '请输入中文英文或者数字',
          trigger: 'blur',
          validator: function(rule, value, callback) {
            // 中文英文或者数字
            // const reg = /^[\u4e00-\u9fa5\u3001\A-\Z\a-\d\(\)]+$/
            const reg = /^[\u4e00-\u9fa5_a-zA-Z0-9]+$/
            if (reg.test(value) === false) {
              callback(new Error('请输入中文英文或者数字'))
            } else {
              callback()
            }
          }
        },
        { required: true, trigger: 'change', validator: this.judgeFlowNameRepeat }
          // { required: true, trigger: 'change' }
        ],
        categoryId: [{ required: true, message: '请选择', trigger: 'blur' }],
        formId: [{ required: true, message: '请输入', trigger: 'blur' }],
        bizName: [{ required: true, message: '请输入', trigger: 'blur' }],
        bizType: [{ required: true, message: '请输入', trigger: 'blur' },
          { required: true, trigger: 'change', validator: this.judgeBizTypeRepeat }],
        orderNum: [{
          validator: function(rule, value, callback) {
            const reg = /^\+?[1-9]\d*$/
            if (reg.test(value) === false) {
              callback(new Error('请输入正整数'))
            } else {
              callback()
            }
          },
          trigger: ['blur']
        }]
      },
      loading: false,
      formList: [],
      categoryForm: [],
      tabList: [{ value: '0', label: '自定义流程' }, { value: '1', label: '业务流程' }]
    }
  },
  computed: {
    ...mapGetters(['processCategoryList'])
  },
  created() {
    this.getForm()
  },
  methods: {
    judgeFlowNameRepeat(rule, value, callback) {
      // 后端需要一个不带()的名称
      // const state = value.split('(')[0]
      if (!value) {
        callback(new Error('请输入'))
        return
      }
      // 如果是编辑状态 且名字和之前一样 ,则跳过远程检测
      if (this.type === 2 && this.oldProcessName === value) {
        callback()
        return
      }
      judgeFlowNameRepeat(value).then(da => {
        if (da.data) {
          callback(new Error('流程名称不能相同'))
        } else {
          callback()
        }
      })
    },
    judgeBizTypeRepeat(rule, value, callback) {
      if (!value) {
        callback(new Error('请输入'))
        return
      }
      // 如果是编辑状态 且名字和之前一样 ,则跳过远程检测
      if (this.type === 2 && this.bizType === value) {
        callback()
        return
      }
      judgeBizTypeRepeat(value).then(da => {
        if (da.data) {
          callback(new Error('业务类型不能相同'))
        } else {
          callback()
        }
      })
    },
    init(val, type) {
      this.type = type
      if (val) {
        this.categoryForm = [val.formCategoryId, val.formId]
        this.form = {
          categoryId: val.categoryId,
          // type=2
          processName: val.processName,
          formId: val.formId,
          orderNum: val.orderNum,
          type: val.type,
          bizName: val.bizName,
          bizType: val.bizType
        }

        if (type === 2) {
          this.oldProcessName = val.processName
          this.bizType = val.bizType
          this.form.processName = val.processName
        } else if (this.form.processName) {
          this.form.processName = ''
        }
      } else {
        this.categoryForm = []
        this.form = {
          categoryId: '',
          processName: '',
          formId: '',
          orderNum: '',
          bizName: '',
          bizType: '',
          type: '0'
        }
      }
    },
    submitForm() {
      // vform模板值
      this.$refs['form'].validate(valid => {
        if (valid) {
          let formJson = {}
          for (var i of this.formList) {
            if (i.value === this.categoryForm[0]) {
              for (var j of i?.children) {
                if (j.value === this.categoryForm[1]) {
                  formJson = JSON.parse(j.formJson)
                  break
                }
              }
              break
            }
          }
          // console.log('this.form', this.form)
          // console.log('formJson', formJson)
          this.$emit('finish', this.form, formJson)
        } else {
          return false
        }
      })
    },
    getForm() {
      getFormCascaderData().then(da => {
        this.formList = da
      })
    },
    handleChange(val) {
      this.form.formId = val[1]
      // console.log('======val', val)
      if (val) {
        // console.log('this.$refs.categoryFormData', this.$refs.categoryFormData)
        if (this.$refs.categoryFormData.prop === 'formId') {
          // 清空验证
          this.$refs.categoryFormData.clearValidate()
        }
      }
    },
    changeMethod(val) {
      if (val) {
        if (this.$refs.selectData.prop === 'categoryId') {
          // 清空验证
          this.$refs.selectData.clearValidate()
        }
      }
    }
  }
}
</script>
<style lang="scss" scoped>
.basic-form {
  padding: 20px;
  width: 640px;
  margin: auto;
  .explain {
    margin: 0 4px;
    cursor: pointer;
  }
}

.popover-btn {
  color: #606266;
  width: 100%;
  text-align: left;
}
</style>
