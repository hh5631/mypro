<!--
 * @Descripttion:
 * @version:
 * @Author: lirioing
 * @Date: 2022-07-18 16:37:34
 * @LastEditors: lirioing
 * @LastEditTime: 2022-08-30 09:50:13
-->
<template>
  <div>
    <el-form ref="form" :model="form" :rules="rules" label-width="300px" label-position="top">
      <div class="add-form">
        <div>
          <el-form-item label="仓库名称" prop="name">
            <el-input v-model.trim="form.name" maxlength="15" placeholder="请输入" class="input-width" />
          </el-form-item>
          <el-form-item label="显示顺序" prop="sort">
            <el-input v-model.number="form.sort" maxlength="11" placeholder="请输入" />
          </el-form-item>
          <el-form-item label="仓库类型" prop="type">
            <el-select v-model="form.type" style="width: 500px" placeholder="请选择" clearable size="small">
              <el-option
                v-for="dict in dict.type.warehouse_type"
                :key="dict.value"
                :label="dict.label"
                :value="dict.value"
              />
            </el-select>
          </el-form-item>
          <el-form-item label="联系电话" prop="contactNumber">
            <el-input v-model.trim="form.contactNumber" class="input-width" placeholder="请输入" />
          </el-form-item>
          <el-form-item label="产权单位" prop="propertyRightUnit">
            <el-input v-model.trim="form.propertyRightUnit" class="input-width" placeholder="请输入" maxlength="50" />
          </el-form-item>
          <el-form-item label="所属城市" prop="city">
            <el-input v-model.trim="form.city" class="input-width" placeholder="请输入" maxlength="15" />
          </el-form-item>
          <el-form-item label="状态" prop="status">
            <el-radio-group v-model="form.enable">
              <el-radio
                v-for="dict in dict.type.sys_normal_disable"
                :key="dict.value"
                :label="dict.value"
              >{{ dict.label }}</el-radio>
            </el-radio-group>

          </el-form-item>
        </div>
        <div>
          <el-form-item label="仓库代码" prop="code">
            <el-input v-model.trim="form.code" class="input-width" maxlength="15" placeholder="请输入" />
          </el-form-item>
          <el-form-item label="仓库用途" prop="purpose">
            <el-select v-model="form.purpose" style="width: 500px" placeholder="请选择" clearable size="small">
              <el-option
                v-for="dict in dict.type.warehouse_classification"
                :key="dict.value"
                :label="dict.label"
                :value="dict.value"
              />
            </el-select>
          </el-form-item>
          <el-form-item label="管理员" prop="storeMan">
            <el-input v-model.trim="form.storeMan" clearable placeholder="请选择" class="input-width" @focus="showLeaderDig">
              <i slot="suffix" class="iconfont icon-jiaoseshezhi" style="line-height: 36px;cursor: pointer;" />
            </el-input>
          </el-form-item>
          <el-form-item label="使用单位" prop="userUnit">
            <el-input v-model.trim="form.userUnit" class="input-width" maxlength="50" placeholder="请输入" />
          </el-form-item>
          <el-form-item label="仓库地址" prop="address">
            <el-input v-model="form.address" class="input-width" maxlength="50" type="text" placeholder="请输入" />
          </el-form-item>
          <el-form-item label="占地面积" prop="coveredArea">
            <el-input v-model.number="form.coveredArea" class="input-width" placeholder="请输入" />
          </el-form-item>
          <el-form-item label="使用状态" prop="status">
            <el-radio-group v-model="form.status">
              <el-radio
                v-for="dict in dict.type.operation_status"
                :key="dict.value"
                :label="dict.value"
              >{{ dict.label }}</el-radio>
            </el-radio-group>
          </el-form-item>

        </div>
      </div>

      <el-form-item label="备注" prop="remark">
        <el-input v-model="form.remark" type="textarea" placeholder="请输入" maxlength="500" />
      </el-form-item>
    </el-form>
    <div slot="footer" class="dialog-footer">
      <el-button size="small" @click="cancel">取 消</el-button>
      <el-button size="small" type="primary" @click="submitForm">保 存</el-button>
    </div>
    <!-- 选择管理员 -->
    <el-dialog
      title="选择管理员"
      :visible.sync="selectLeaderDig"
      width="800px"
      append-to-body
      :close-on-click-modal="false"
      class="selectPlan"
      @close="closeLeaderDig"
    ><selectLeader ref="selectLeader" @checkedUser="getCheckUser" /><div slot="footer" class="dialog-footer">
      <el-button size="small" @click="clearSelected">清空已选</el-button>
      <el-button size="small" type="primary" @click="submitCheckLeader">保存</el-button>
    </div></el-dialog>
  </div>
</template>
<script>
import { saveOrUpdate } from '@/api/basicdata/warehouse'
export default {
  dicts: ['sys_normal_disable', 'warehouse_type', 'storekeeper', 'warehouse_classification', 'operation_status'],
  data() {
    return {
      selectLeaderDig: false,
      // 表单参数
      form: {
        id: undefined,
        name: undefined,
        sort: undefined, // 显示顺序
        type: undefined,
        contactNumber: undefined,
        propertyRightUnit: undefined,
        city: undefined,
        status: '0',
        code: undefined,
        purpose: undefined,
        storeMan: undefined,
        userUnit: undefined,
        address: undefined,
        coveredArea: undefined, // 占地面积
        enable: '0',
        remark: undefined
      },
      peopleInfo: {},
      // 表单校验
      rules: {
        name: [
          { required: true, message: '请填写仓库名称', trigger: 'blur' },
          {
            validator: function(rule, value, callback) {
              if (/^[\u4e00-\u9fa5a-zA-Z]+$/.test(value) === false) {
                callback(new Error('请输入汉字或英文'))
              } else {
                // 校验通过
                callback()
              }
            },
            trigger: ['blur', 'change']
          }
        ],
        sort: [
          { required: true, message: '显示顺序不能为空', trigger: 'blur' },
          { type: 'number', message: '显示顺序必须为大于1的数字' },
          {
            validator: function(rule, value, callback) {
              if (value < 1) {
                callback(new Error('显示顺序必须为大于1的数字'))
              } else {
                // 校验通过
                callback()
              }
            },
            trigger: ['blur', 'change']
          }
        ],
        contactNumber: [
          { min: 11, max: 13, message: '联系电话长度在 7到 13 个字符', trigger: 'blur' }
        ],
        code: [
          { required: true, message: '请填写仓库代码', trigger: 'blur' },
          { min: 3, message: '职位代码长度最少为3位', trigger: 'blur' },
          {
            validator: function(rule, value, callback) {
              if (/^[a-zA-Z0-9]+$/.test(value) === false) {
                callback(new Error('请输入数字或英文'))
              } else {
                // 校验通过
                callback()
              }
            },
            trigger: ['blur', 'change']
          }
          // {
          //   validator: function(rule, value, callback) {
          //     judgeExist(value).then(res => {
          //       if (res.data) {
          //         callback(new Error('仓库代码不能重复'))
          //       } else {
          //         callback()
          //       }
          //     })
          //   },
          //   trigger: 'blur'
          // }
        ],
        purpose: [
          { required: true, message: '请选择仓库用途', trigger: 'blur' }
        ],
        storeMan: [{ required: true, message: '请选择管理员', trigger: ['blur', 'change'] }],
        userUnit: [
          { required: true, message: '请填写仓库名称', trigger: 'blur' },
          {
            validator: function(rule, value, callback) {
              if (/^[\u4e00-\u9fa5]+$/.test(value) === false) {
                callback(new Error('请输入汉字'))
              } else {
                // 校验通过
                callback()
              }
            },
            trigger: ['blur', 'change']
          }
        ],
        coveredArea: [
          { type: 'number', message: '占地面积必须为大于1的数字' },
          {
            validator: function(rule, value, callback) {
              if (value && value < 1) {
                callback(new Error('占地面积必须为大于1的数字'))
              } else {
                // 校验通过
                callback()
              }
            },
            trigger: ['blur', 'change']
          }
        ],
        address: [
          { required: true, message: '请填写仓库名称地址', trigger: 'blur' }
        ],
        city: [{
          validator: function(rule, value, callback) {
            if (/^[\u4e00-\u9fa5a-zA-Z]+$/.test(value) === false) {
              callback(new Error('请输入汉字或英文'))
            } else {
              // 校验通过
              callback()
            }
          },
          trigger: ['blur', 'change']
        }],
        // status: [{ required: true, message: '请选择使用状态', trigger: 'blur' }],
        // enable: [{ required: true, message: '请选择状态', trigger: 'blur' }],
        remark: [

        ]
      }
    }
  },
  methods: {
    /** 提交按钮 */
    submitForm: function() {
      this.$refs['form'].validate(valid => {
        if (valid) {
          saveOrUpdate(this.form).then(res => {
            if (res.code === 200) {
              this.$modal.msgSuccess('保存成功')
              this.$emit('addSuccess')
            } else {
              this.$message.error(res.msg)
            }
          })
        }
      })
    },
    // 取消按钮
    cancel() {
      this.$emit('cancelAdd')
      this.reset()
    },
    // 表单重置
    reset() {
      this.form = {
        id: undefined,
        name: undefined,
        sort: undefined, // 显示顺序
        type: undefined,
        contactNumber: undefined,
        propertyRightUnit: undefined,
        city: undefined,
        status: '0',
        code: undefined,
        purpose: undefined,
        storeMan: undefined,
        userUnit: undefined,
        address: undefined,
        coveredArea: undefined, // 占地面积
        enable: '0',
        remark: undefined
      }
      this.resetForm('form')
    },
    /** 选择管理员 */
    closeLeaderDig() {
      this.selectLeaderDig = false
    },
    getCheckUser(items) {
      this.peopleInfo = items
    },
    showLeaderDig() {
      this.selectLeaderDig = true
      this.$refs.selectLeader.$refs.rowRef.setCurrentRow('')
      this.$refs.selectLeader.$refs.dept.setCurrentKey()
    },
    clearSelected() {
      this.$refs.selectLeader.$refs.rowRef.setCurrentRow('')
      this.$refs.selectLeader.$refs.dept.setCurrentKey()
    },
    submitCheckLeader() {
      this.form.storeMan = this.peopleInfo[0].realName
      this.selectLeaderDig = false
    }
  }
}

</script>
<style lang="scss" scoped>
.add-form{
  display:flex;
  justify-content:space-between
}
.input-width{
  ::v-deep .el-input__inner{
width: 500px;
  }
}
</style>
