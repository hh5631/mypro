<template>
  <div>
    <div class="produce-collapse">
      <el-form ref="mainForm" :model="basicForm" :rules="rules">
        <el-descriptions class="base_info_desc" :column="2" border>
          <el-descriptions-item>
            <template slot="label">
              标题<span v-if="isDetail !== '1'" class="redDot">*</span>
            </template>
            <el-form-item prop="title">
              <el-input
                v-model="basicForm.title"
                placeholder="请输入"
                :disabled="isDetail == '1'"
                maxlength="15"
                @change="trimFun('title', $event)"
              />
            </el-form-item>
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label"> 采购单编号 </template>
            <el-input
              v-model="basicForm.orderNo"
              disabled
              placeholder="自动生成"
            /></el-descriptions-item>
          <el-descriptions-item v-if="isDetail !== '1'" key="procurementLeader">
            <template slot="label">
              采购负责人<span v-if="isDetail !== '1'" class="redDot">*</span>
            </template>
            <el-input
              v-if="isDetail === '1'"
              v-model="basicForm.procurementLeader"
              placeholder="请输入"
              :disabled="isDetail == '1'"
              @focus="showLeaderDig"
            />
            <el-form-item v-if="isDetail !== '1'" prop="procurementLeader">
              <div style="position: relative">
                <el-select
                  v-model="leaderArr"
                  multiple
                  filterable
                  :disabled="isDetail == '1'"
                  placeholder="请输入"
                  @change="leaderChange"
                >
                  <el-option
                    v-for="item in leaderOptions"
                    :key="item.userName"
                    :label="item.label"
                    :value="item.userName"
                  />
                </el-select>
                <span
                  v-if="isDetail !== '1'"
                  class="icon-people"
                  @click="showLeaderDig"
                />
                <div
                  v-if="basicForm.procurementLeader && isDetail !== '1'"
                  class="icon-delete"
                  @click.stop="clearLeader"
                >
                  <i class="el-icon-circle-close" @click.stop="clearLeader" />
                </div>
              </div>
            </el-form-item>
          </el-descriptions-item>
          <el-descriptions-item v-if="isDetail === '1'" key="procurementLeaderName">
            <template slot="label">
              采购负责人<span v-if="isDetail !== '1'" class="redDot">*</span>
            </template>
            <span style="padding-left:15px;font-size: 14px;font-family: ArialMT;color: #333333;line-height: 16px;">{{ basicForm.procurementLeaderName }}</span>
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label"> 预期到货时间 </template>
            <div class="date_box">
              <el-date-picker
                v-model="basicForm.expectedArrivalDate"
                size="small"
                type="date"
                value-format="yyyy-MM-dd"
                :placeholder="isDetail!=='1'?'请选择':''"
                :clearable="false"
                :disabled="isDetail === '1'"
                :picker-options="pickerOptions"
              >
                <!-- <i v-if="isDetail !== '1'" class="el-icon-date data_icon" /> -->
              </el-date-picker>
            </div>
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label">
              供应商名称<span v-if="isDetail !== '1'" class="redDot">*</span>
            </template>
            <!-- <el-input v-model="supplierName" placeholder="请输入" :disabled="isDetail==='1'" @focus="showApplyDig">
              <i slot="suffix" class="el-input__icon el-icon-s-tools" />
            </el-input> -->
            <el-form-item prop="supplierId">
              <div style="position: relative">
                <el-select
                  v-model="supplierName"
                  clearable
                  filterable
                  :disabled="isDetail == '1'"
                  placeholder="请输入"
                  @change="getSupplierId"
                >
                  <el-option
                    v-for="item in applyOptions"
                    :key="item.value"
                    :label="item.label"
                    :value="item"
                  />
                </el-select>
                <span
                  v-if="isDetail !== '1'"
                  class="icon-apply"
                  @click="showApplyDig"
                />
                <span
                  v-if="basicForm.supplierId && isDetail !== '1'"
                  class="icon-delete"
                  @click="clearSupplier"
                >
                  <i class="el-icon-circle-close" />
                </span>
              </div>
            </el-form-item>

          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label"> 供应商电话 </template>
            <el-form-item prop="supplierTelPhone">
              <el-input
                v-model="basicForm.supplierTelPhone"
                :placeholder="isDetail!=='1'?'请输入':''"
                :disabled="isDetail === '1'"
                @change="trimFun('supplierTelPhone', $event)"
              >
                <!-- <span
                  v-if="isPhoneWarn"
                  slot="suffix"
                  class="warnTip"
                ><i class="el-icon-warning-outline" style="color: #ef5826" />
                  请填写正确的号码</span> -->
              </el-input>
            </el-form-item>
          </el-descriptions-item>

          <el-descriptions-item :span="2">
            <template slot="label"> 采购询价意见 </template>
            <el-input
              v-model="basicForm.enquiryComments"
              :placeholder="isDetail!=='1'?'请输入':''"
              :disabled="isDetail === '1'"
              maxlength="500"
              show-word-limit
            />
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label"> 采购说明 </template>
            <el-input
              v-model="basicForm.purchaseDescription"
              :placeholder="isDetail!=='1'?'请输入':''"
              :disabled="isDetail === '1'"
              maxlength="500"
              show-word-limit
            />
          </el-descriptions-item>
        </el-descriptions>
      </el-form>
    </div>
    <el-dialog
      title="选择采购负责人"
      :visible.sync="selectLeaderDig"
      width="800px"
      append-to-body
      :close-on-click-modal="false"
      class="selectPlan"
      @close="closeLeaderDig"
    ><selectLeader ref="selectLeader" @checkedUser="getCheckUser" />
      <div slot="footer" class="dialog-footer">
        <el-button size="small" @click="clearSelected">清空已选</el-button>
        <el-button
          size="small"
          type="primary"
          @click="submitCheckLeader"
        >保存</el-button>
      </div></el-dialog>
    <el-dialog
      title="选择供应商"
      :visible.sync="selectApplyDig"
      width="1200px"
      append-to-body
      :close-on-click-modal="false"
      class="selectPlan"
      @close="closeApplyDig"
    ><selectApply ref="selectApply" />
      <div slot="footer" class="dialog-footer">
        <!-- <el-button size="small" style="float:left" @click="addApply"><i class="iconfont icon-zengjia" />新增供应商</el-button> -->
        <el-button size="small" @click="closeApplyDig">取消</el-button>
        <el-button
          size="small"
          type="primary"
          @click="submitSupplier"
        >保存</el-button>
      </div></el-dialog>
    <el-dialog
      title="新建供应商"
      :visible.sync="addApplyDig"
      width="1200px"
      append-to-body
      :close-on-click-modal="false"
      class="selectPlan1"
      @close="closeAddApplyDig"
    ><addSupplier ref="formRef" :show="addApplyDig" />
      <div slot="footer" class="dialog-footer">
        <el-button size="small" @click="closeAddApplyDig">取消</el-button>
        <el-button size="small" type="primary">保存</el-button>
      </div></el-dialog>
  </div>
</template>

<script>
import selectLeader from './selectLeader.vue'
import selectApply from './selectApply.vue'
import addSupplier from './addSupplier.vue'
import { listUser } from '@/api/system/user'
import { getSupplierList } from '@/api/basicdata/index'
import { myTrim } from '@/utils/index'
import { validCellphone, validTelephone } from '@/utils/validate'
export default {
  components: { selectLeader, selectApply, addSupplier },
  props: {
    baseInfos: {
      type: Object,
      default: () => {}
    },
    isDetail: {
      type: String,
      default: ''
    }
  },
  data() {
    const validateSupplierTelPhone = (rule, value, callback) => {
      if (!value) {
        callback()
      } else {
        if (value.charAt(0) === '0') { // charAt查找第一个字符方法，用来判断输入的是座机还是手机号
          if (validTelephone(value)) {
            callback()
          } else {
            callback(new Error("请输入正确座机号码，格式'区号-号码'"))
          }
        } else {
          if (validCellphone(value)) {
            callback()
          } else {
            callback(new Error('请输入正确手机号'))
          }
        }
      }
    }
    return {
      title: '',
      selectLeaderDig: false,
      selectApplyDig: false,
      addApplyDig: false,
      checkedLeader: [],
      supplierName: '',
      basicForm: {
        title: '', // 标题
        orderNo: '', // 采购单编号
        procurementLeader: '', // 采购负责人
        expectedArrivalDate: '', // 预期到货时间
        supplierId: '', // 供应商
        supplierTelPhone: '', // 供应商电话
        enquiryComments: '', // 采购询价意见
        purchaseDescription: '', // 采购说明
        procurementLeaderName: ''
      },
      leaderArr: [], // 真实姓名
      leaderOptions: [],
      applyOptions: [],
      pickerOptions: {
        // 禁用当前日期之前的日期
        disabledDate(time) {
          // Date.now()是javascript中的内置函数，它返回自1970年1月1日00:00:00 UTC以来经过的毫秒数。
          return time.getTime() < Date.now() - 8.64e7
        }
      },
      queryParams: {
        enable: 0,
        pageNum: 0,
        pageSize: 99999
      },
      rules: {
        title: [{ required: true, message: '请填写此项', trigger: 'blur' }],
        procurementLeader: [{ required: true, message: '请填写此项', trigger: 'change' }],
        supplierId: [{ required: true, message: '请填写此项', trigger: 'change' }],
        supplierTelPhone: [{ validator: validateSupplierTelPhone, trigger: 'change' }]
      }
    }
  },
  watch: {
    baseInfos(val) {
      this.basicForm = {
        id: val.id,
        title: val.title, // 标题
        orderNo: val.orderNo, // 采购单编号
        procurementLeader: val.procurementLeader, // 采购负责人
        expectedArrivalDate: val.expectedArrivalDate, // 预期到货时间
        supplierId: val.supplierId, // 供应商
        supplierTelPhone: val.supplierTelPhone, // 供应商电话
        enquiryComments: val.enquiryComments, // 采购询价意见
        purchaseDescription: val.purchaseDescription, // 采购说明
        procurementLeaderName: val.procurementLeaderName // 采购说明
      }
      this.leaderArr = val.procurementLeader.split(',')
      this.supplierName = val.supplierName
    },
    'basicForm.supplierId': {
      deep: true,
      handler(n, o) {

      }
    }
  },
  created() {
  },
  mounted() {
    this.$nextTick(() => {
    // DOM 更新了
      this.$refs['mainForm'].clearValidate()
    })
    // 接口触发采购负责人的查询接口
    listUser().then((response) => {
      this.leaderOptions =
        (response.rows &&
          response.rows.map((user) => {
            return {
              label: user.realName,
              value: user.userId,
              userName: user.userName
            }
          })) ||
        []
    })
    getSupplierList(this.queryParams).then((response) => {
      this.applyOptions =
        (response.data.records &&
          response.data.records.map((applier) => {
            return {
              label: applier.name,
              value: applier.id,
              phone: applier.phone
            }
          })) ||
        []
    })
  },
  methods: {
    clearSelected() {
      this.$refs.selectLeader.$refs.rowRef.clearSelection()
    },
    clearValidate() {
      this.$refs.mainForm.resetFields()
    },
    submitSupplier() {
      // 供应商名称选择
      this.selectApplyDig = false
      this.supplierName = this.$refs['selectApply'].currentRow.name
      this.basicForm.supplierId = this.$refs['selectApply'].currentRow.id
      this.basicForm.supplierTelPhone =
        this.$refs['selectApply'].currentRow.phone

      if (this.supplierName?.length > 0) {
        this.$refs.mainForm.fields.map(i => {
          if (i.prop === 'supplierId') {
            // 清空验证
            i.clearValidate()
          }
        })
      }
    },
    getSupplierId(val) {
      this.basicForm.supplierId = val.value
      this.basicForm.supplierTelPhone = val.phone
    },
    leaderChange(val) {
      this.basicForm.procurementLeader = val.join(',')
      const leaderIdArr = []
      this.leaderArr.forEach(item => {
        this.leaderOptions.forEach(obj => {
          if (item === obj.userName) {
            leaderIdArr.push(obj.value)
          }
        })
      })
      this.basicForm.procurementLeaderId = leaderIdArr.join(',')
    },
    showLeaderDig() {
      this.selectLeaderDig = true
      this.$nextTick(() => {
        this.$refs.selectLeader.reset()
      })
    },
    closeLeaderDig() {
      this.selectLeaderDig = false
    },
    showApplyDig() {
      this.selectApplyDig = true
      // 清空供应商搜索条件
      this.$nextTick(() => {
        this.$refs.selectApply.resetQuery()
      })
    },
    closeApplyDig() {
      this.selectApplyDig = false
    },
    addApply() {
      this.addApplyDig = true
    },
    closeAddApplyDig() {
      this.addApplyDig = false
    },
    getCheckUser(items) {
      this.checkedLeader = items
    },
    // 最终保存
    submitCheckLeader() {
      const leaderUserNameArr = []
      this.checkedLeader.forEach((item) => {
        if (!this.leaderArr.includes(item.userName)) {
          this.leaderArr.push(item.userName)
          leaderUserNameArr.push(item.userName)
          this.selectLeaderDig = false
        } else {
          this.$message.error('不能选择相同的采购负责人')
        }
      })

      if (this.leaderArr?.length > 0) {
        this.$refs.mainForm.fields.map(i => {
          if (i.prop === 'procurementLeader') {
            // 清空验证
            i.clearValidate()
          }
        })
      }
      this.basicForm.procurementLeader = this.leaderArr.join(',')
      const leaderIdArr = []
      this.leaderArr.forEach(item => {
        this.leaderOptions.forEach(obj => {
          if (item === obj.userName) {
            leaderIdArr.push(obj.value)
          }
        })
      })
      this.basicForm.procurementLeaderId = leaderIdArr.join(',')
      this.$refs.selectLeader.$refs.rowRef.clearSelection()
    },
    clearLeader() {
      this.basicForm.procurementLeader = ''
      this.leaderArr = []
    },
    clearSupplier() {
      this.basicForm.supplierId = ''
      this.supplierName = ''
    },
    // 表单校验
    validateForm() {
      return new Promise(resolve => {
        this.$refs.mainForm.validate((valid) => {
          resolve(valid)
        })
      })
    },
    // 去首位空格，但可输入中间空格
    trimFun(str, val) {
      this.basicForm[str] = myTrim(val)
    }
  }
}
</script>

<style lang="scss" scoped>
::v-deep .produce-collapse .el-input.is-disabled .el-input__inner{
      font-size: 14px;
      font-family: ArialMT;
      color: #333333;
      line-height: 16px;
}
.icon-delete {
  display: inline-block;
  cursor: pointer;
  width: 24px;
  height: 24px;
  position: absolute;
  right: 30px;
  bottom: 0;
  top: 7px;
  text-align: center;
}
.icon-people {
  display: inline-block;
  cursor: pointer;
  width: 24px;
  height: 24px;
  position: absolute;
  right: 6px;
  bottom: 0;
  top: calc(100% - 36px);
  background: url("../../../../assets/images/people.png") no-repeat;
  background-position: center;
  background-size: 100% 100%;
}
.icon-apply {
  display: inline-block;
  cursor: pointer;
  width: 24px;
  height: 24px;
  position: absolute;
  right: 6px;
  bottom: 0;
  top: calc(100% - 36px);
  background: url("../../../../assets/images/applyName.png") no-repeat;
  background-position: center;
  background-size: 100% 100%;
}
.base_info_desc {
  ::v-deep .el-select {
    width: 100%;
    .el-input__suffix-inner {
      display: none;
    }
    .el-input__suffix {
      // background: url('../../../../assets/images/people.png') no-repeat;
      // background-size: cover;
      // width: 24px;
      // height: 24px;
      // margin-top: 6px;
    }
  }
  ::v-deep .el-input__inner {
    border: none;

    border-radius: 0;
  }
  ::v-deep .el-descriptions--medium.is-bordered .el-descriptions-item__cell {
    padding: 6px;
  }
  .redDot {
    color: #ef5826;
  }
  .date_box {
    width: 100%;
    ::v-deep .el-range__close-icon {
      position: absolute;

      right: 34px;
      top: 54%;
      transform: translateY(-50%);
    }
    ::v-deep .el-date-editor .el-input__inner {
      border: none;
      font-size: 14px;
      font-family: ArialMT;
      color: #333333;
      line-height: 16px;

    }
  }
  ::v-deep .el-icon-user {
    background: url("../../../../assets/images/people.png") no-repeat;
    background-size: cover;
    width: 24px;
    height: 24px;
    margin-top: 6px;
  }
  ::v-deep .el-icon-user:before {
    content: "";
    font-size: 16px;
  }
  ::v-deep .el-icon-s-tools {
    background: url("../../../../assets/images/applyName.png") no-repeat;
    background-size: cover;
    width: 24px;
    height: 24px;
    margin-top: 6px;
  }
  ::v-deep .el-icon-s-tools:before {
    content: "";
    font-size: 16px;
  }
  ::v-deep .el-date-editor.el-input {
    width: 100%;
    position: relative;
  }
  ::v-deep .el-input--small .el-input__inner {
    height: 36px;
  }
  ::v-deep .el-input__prefix {
    width: 36px;
    height: 36px;
    position: absolute;
      top: 50%;
      left: 90%;
      z-index: 9;
      color: #c0c4cc;
      font-size: 18px;
      transform: translateY(-50%);
  }
  ::v-deep .el-input__icon.el-icon-date {
    display: inline-block;
  }
  ::v-deep .el-input--prefix .el-input__inner {
    padding-left: 15px;
  }
  ::v-deep .el-input__inner {
    border: none;
    border-radius: 0;
    height: 48px;
  }
  ::v-deep .el-descriptions--medium.is-bordered .el-descriptions-item__cell {
    padding: 0px;
  }
  ::v-deep
  .el-descriptions-item__cell.el-descriptions-item__label.is-bordered-label {
    padding-left: 16px;
  }
  .warnTip {
    line-height: 48px;
    color: #ef5826;
  }
  ::v-deep .el-input.is-disabled .el-input__inner,
  ::v-deep .el-range-editor.is-disabled,
  ::v-deep .el-range-editor.is-disabled input {
    background: #fff;
    cursor: default;
  }
}
.selectPlan1 {
  ::v-deep .el-dialog .el-dialog__body {
    padding-top: 0;
    max-height: 542px;
    overflow-y: auto;
    // padding-bottom: 0;
  }
}
</style>
