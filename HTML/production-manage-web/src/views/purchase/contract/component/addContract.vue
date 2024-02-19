<template>
  <div>
    <!-- 基本信息 -->
    <collapse title="基本信息">
      <el-form ref="mainForm" class="produce-collapse" :model="form" :rules="rules">
        <el-descriptions class="base_info_desc" :column="2" border>
          <el-descriptions-item>
            <template slot="label">
              合同编号<span v-if="!isDisabled" class="redDot">*</span>
            </template>
            <el-form-item prop="contractNo">
              <el-input v-model.trim="form.contractNo" :disabled="isDisabled" placeholder="请输入" maxlength="10">
                <!-- <span v-if="isCodeWarn" slot="suffix" class="warnTip">{{ warnText }}</span> -->
              </el-input>
            </el-form-item>
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label">
              合同名称<span v-if="!isDisabled" class="redDot">*</span>
            </template>
            <el-form-item prop="contractName">
              <el-input v-model.trim="form.contractName" :disabled="isDisabled" placeholder="请输入" maxlength="12">
                <!-- <span v-if="isNameWarn" slot="suffix" class="warnTip">{{ warnText }}</span> -->
              </el-input>
            </el-form-item>
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label">
              供应商<span v-if="!isDisabled" class="redDot">*</span>
            </template>
            <el-form-item v-if="isEdit" prop="supplierName">
              <div class="my-select-con">
                <el-select
                  v-model.trim="form.supplierName"
                  filterable
                  placeholder="请输入"
                  class="input-width"
                  @change="getSupplierId"
                >
                  <el-option
                    v-for="item in supplierOptionsArr"
                    :key="item.id"
                    :label="item.name"
                    :value="item.id"
                  />
                </el-select>
                <i slot="suffix" class="iconfont icon-bumenxuanze" style=";cursor: pointer;" @click.stop="showSupplier" />
              </div>
            </el-form-item>
            <span v-else style="margin-left:15px ;color: #000000;">
              {{ form.supplierName }}
            </span>
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label">
              (甲)签订人
            </template>
            <el-input v-model.trim="form.firstParty" :disabled="isDisabled" :placeholder="!isDisabled?'请输入':''" maxlength="20" />
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label">
              (乙)签订人
            </template>
            <el-input v-model.trim="form.secondParty" :disabled="isDisabled" :placeholder="!isDisabled?'请输入':''" maxlength="20" />
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label">
              签订日期
            </template>
            <div class="date_box">
              <el-date-picker
                v-model="form.signedDate"
                :editable="false"
                :disabled="isDisabled"
                size="small"
                type="date"
                value-format="yyyy-MM-dd"
                :placeholder="!isDisabled?'请选择':''"
                :clearable="false"
              />
              <i v-if="!isDisabled" class="el-icon-date data_icon" />
            </div>
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label">
              采购单号
            </template>
            <el-input v-model.trim="form.poNo" clearable :disabled="isDisabled" :placeholder="!isDisabled?'请输入':''" maxlength="20" />
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label">
              合同金额(元)
            </template>
            <el-input
              v-model.trim="form.contractAmount"
              clearable
              :disabled="isDisabled"
              :placeholder="!isDisabled?'请输入':''"
              maxlength="12"
              @input="limitInput($event,'contractAmount')"
            />
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label">
              合同状态
            </template>
            <el-select v-if="isEdit" v-model="form.contractStatus" style="width: 100%" :placeholder="!isDisabled?'请选择':''">
              <el-option
                v-for="item in contractStatusOptions"
                :key="item.value"
                :label="item.label"
                :value="item.value"
              />
            </el-select>
            <span v-else style="padding-left: 16px;color:#000000">{{ getContractStatus(form.contractStatus) }}</span>
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label">
              创建人
            </template>
            <el-input v-model.trim="form.createBy" :disabled="isDisabled" placeholder="请输入" maxlength="20" />
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label">
              创建日期
            </template>
            <div class="date_box">
              <el-date-picker
                v-model="form.createTime"
                :editable="false"
                :disabled="isDisabled"
                size="small"
                type="date"
                value-format="yyyy-MM-dd"
              />
            </div>
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label">
              备注
            </template>
            <el-input v-model.trim="form.remark" :disabled="isDisabled" :placeholder="!isDisabled?'请输入':''" maxlength="30" />
          </el-descriptions-item>
        </el-descriptions>
      </el-form>
    </collapse>
    <!-- 付款条件 -->
    <collapse title="付款条件">
      <div v-if="isEdit" style="margin-bottom: 10px;display:flex; justify-content:flex-end;">
        <el-button type="primary" icon="el-icon-plus" size="small" @click="handleAddDetails">增加一行</el-button>
        <el-button type="danger" icon="el-icon-delete" size="small" :disabled="removeDisabled" @click="handleRemoveDetails">删除一行</el-button>
      </div>
      <el-table ref="contractTable1" v-loading="loading" border style="width: 100%" highlight-current-row :data="contractTable1" @selection-change="handleSelectionChange">
        <el-table-column v-if="isEdit" type="selection" align="center" fixed :disabled="isDisabled" />
        <el-table-column label="付款批次" align="left" prop="paymentBatch">
          <template slot-scope="scope">
            <el-input
              v-if="isEdit"
              v-model.trim="scope.row.paymentBatch"
              clearable
              :disabled="isDisabled"
              size="small"
              maxlength="5"
            />
            <span v-else>{{ scope.row.paymentBatch }}</span>
          </template>
        </el-table-column>
        <el-table-column label="付款货物/服务" sortable align="left" prop="goods" :show-overflow-tooltip="true">
          <template slot-scope="scope">
            <el-input
              v-if="isEdit"
              v-model.trim="scope.row.goods"
              clearable
              :disabled="isDisabled"
              size="small"
              maxlength="5"
            />
            <span v-else>{{ scope.row.goods }}</span>
          </template>
        </el-table-column>
        <el-table-column label="科目" align="left" prop="paymentType">
          <template slot-scope="scope">
            <el-select v-if="isEdit" v-model="scope.row.paymentType" placeholder="请选择" :disabled="isDisabled">
              <el-option v-for="item in paymentTypeOptions" :key="item.value" :label="item.label" :value="item.value" />
            </el-select>
            <span v-else>{{ scope.row.paymentType }}</span>
          </template>
        </el-table-column>
        <el-table-column label="付款比例%" align="left" prop="paymentProportion" :show-overflow-tooltip="true">
          <template slot-scope="scope">
            <el-input
              v-if="isEdit"
              v-model.trim="scope.row.paymentProportion"
              clearable
              :disabled="isDisabled"
              size="small"
              maxlength="5"
            />
            <span v-else>{{ scope.row.paymentProportion }}</span>
          </template>
        </el-table-column>
        <el-table-column label="应付金额(元)" align="left" prop="paymentAmount" :show-overflow-tooltip="true">
          <template slot-scope="scope">
            <el-input
              v-if="isEdit"
              v-model.trim="scope.row.paymentAmount"
              :disabled="isDisabled"
              clearable
              size="small"
              @input="limitPaymentAmountInput($event,scope.$index)"
            />
            <span v-else>{{ scope.row.paymentAmount }}</span>
          </template>
        </el-table-column>
        <el-table-column label="约定付款时间/条件" sortable align="left" prop="paymenTerm" :show-overflow-tooltip="true">
          <template slot-scope="scope">
            <el-input
              v-if="isEdit"
              v-model.trim="scope.row.paymenTerm"
              clearable
              :disabled="isDisabled"
              size="small"
            />
            <span v-else>{{ scope.row.paymenTerm }}</span>
          </template>
        </el-table-column>
        <el-table-column label="付款方式" align="center" prop="paymentMethod" class-name="small-padding">
          <template slot-scope="scope" inline-template>
            <el-select v-if="isEdit" v-model="scope.row.paymentMethod" placeholder="请选择" :disabled="isDisabled">
              <el-option v-for="item in paymentMethodOptions" :key="item.value" :label="item.label" :value="item.value" />
            </el-select>
            <span v-else>{{ scope.row.paymentMethod }}</span>
          </template>
        </el-table-column>
      </el-table>
    </collapse>
    <!-- 附件信息 -->
    <collapse title="附件信息">
      <attachment-upload v-model="form.fileList" :readonly="isDisabled" />
    </collapse>

    <!-- 供应商选择 -->
    <el-dialog
      title="供应商选择"
      width="1200px"
      :visible.sync="showSupplierDialog"
      append-to-body
      @close="closeSupplier"
    >
      <selectSupplier @selectSupplier="getSupplier" @cancelSupplier="cancelSupplier" />
    </el-dialog>
  </div>
</template>

<script>
import collapse from '@/views/basicdata/material/component/collapse'
import selectSupplier from '@/views/basicdata/material/component/selectSupplier'
import { saveOrUpdateContract } from '@/api/purchase/index'
import AttachmentUpload from '@/components/AttachmentUpload'
export default {
  name: 'AddContract',
  components: { collapse, selectSupplier, AttachmentUpload },
  props: {
    formData: {
      type: Object,
      default: () => {}
    },
    isDisabled: {
      type: Boolean,
      default: false
    },
    isEdit: {
      type: Boolean
    },
    supplierOptions: {
      type: Array,
      default: () => []
    }
  },
  data() {
    return {
      loading: false,
      title: '',
      form: {
      },
      isCodeWarn: false,
      isNameWarn: false,
      isSupplierWarn: false,
      warnText: '请填写此项',
      contractTable1: [],
      // 选择供应商
      showSupplierDialog: false,
      // 供应商缓存数字
      contractStatusOptions: [{
        value: '0',
        label: '审核中'
      }, {
        value: '1',
        label: '已驳回'
      }, {
        value: '2',
        label: '签订中'
      }, {
        value: '3',
        label: '生效中'
      }, {
        value: '4',
        label: '已完成'
      }],
      // 付款条件删除按钮状态
      removeDisabled: false,
      payCondition: {},
      // 科目下拉选项
      paymentTypeOptions: [{
        value: '0',
        label: '全款'
      }, {
        value: '1',
        label: '预付款'
      }, {
        value: '2',
        label: '尾款'
      }, {
        value: '3',
        label: '订金'
      }, {
        value: '4',
        label: '定金'
      }, {
        value: '5',
        label: '余款'
      }],
      paymentMethodOptions: [{
        value: '0',
        label: '网银'
      }, {
        value: '1',
        label: '支票'
      }, {
        value: '2',
        label: '现金'
      }, {
        value: '3',
        label: '微信'
      }, {
        value: '4',
        label: '支付宝'
      }],
      rules: {
        contractNo: [{ required: true, message: '请填写此项' },
          { pattern: /^[a-zA-Z0-9]+$/, message: '仅支持数字或英文' }],
        contractName: [{ required: true, message: '请填写此项' }],
        supplierName: [{ required: true, message: '请填写此项' }]
      }
    }
  },
  computed: {
    supplierOptionsArr() {
      return this.supplierOptions
    }
  },
  created() {
  },
  methods: {
    /** 重置表单*/
    reset() {
      this.form = {
        contractAmount: undefined,
        contractName: '',
        contractNo: '',
        contractStatus: '',
        createTime: null,
        createBy: '',
        firstParty: '',
        secondParty: '',
        poNo: '',
        signedDate: '',
        supplierName: '',
        remark: '',
        supplierId: '',
        fileList: [],
        details: []
      }
      this.form.createTime = new Date()
      this.form.createBy = this.$store.state.user.name
      this.isCodeWarn = false
      this.isNameWarn = false
      this.isSupplierWarn = false
      this.contractTable1 = []
      this.$refs.mainForm.resetFields()
    },
    /** 表单校验*/
    validateForm() {
      // 数字和英文正则
      const reg1 = /^[a-zA-Z0-9]+$/
      if (!this.form.contractNo) {
        this.isCodeWarn = true
        this.warnText = '请填写此项'
        return false
      }
      if (!reg1.test(this.form.contractNo)) {
        this.isCodeWarn = true
        this.warnText = '仅支持数字或英文'
        return false
      }
      if (!this.form.contractName) {
        this.isNameWarn = true
        this.warnText = '请填写此项'
        return false
      }
      if (!this.form.supplierId) {
        this.isSupplierWarn = true
        this.warnText = '请填写此项'
        return false
      }
      return true
    },
    /** 编辑时设置表单对象*/
    editContract() {
      this.form = this.formData
      this.contractTable1 = this.form.details
      this.contractTable1.forEach(item => {
        item.paymentAmount = item.paymentAmount / 100
      })
    },
    /** 显示供应商选择对象*/
    showSupplier() {
      this.showSupplierDialog = true
    },
    closeSupplier() {
      this.showSupplierDialog = false
    },
    cancelSupplier(value) {
      this.showSupplierDialog = value
    },
    getSupplier(payload) {
      if (payload.id) {
        this.showSupplierDialog = false
      }
      this.form.supplierName = payload.name
      this.form.supplierId = payload.id
    },
    /** 付款条件*/
    handleAddDetails() {
      this.contractTable1.push({})
    },
    handleRemoveDetails() {
      this.contractTable1.splice(this.contractTable1.length - 1, 1)
    },
    handleSelectionChange() {},
    scientificToNumber(num) {
      const str = num.toString()
      const reg = /^(\d+)(e)([\-]?\d+)$/
      let arr
      let len
      let zero = ''

      /* 6e7或6e+7 都会自动转换数值*/
      if (!reg.test(str)) {
        return num
      } else {
        /* 6e-7 需要手动转换*/

        arr = reg.exec(str)
        len = Math.abs(arr[3]) - 1
        for (let i = 0; i < len; i++) {
          zero += '0'
        }
        return '0.' + zero + arr[1]
      }
    },
    /** 提交表单*/
    submitForm() {
      this.$refs.mainForm.validate((valid) => {
        if (valid) {
          this.form.contractAmount = this.scientificToNumber(Number(this.form.contractAmount) * 100)
          this.form.createTime = ''
          this.form.details = this.contractTable1.map(item => {
            return {
              goods: item.goods,
              mainId: item.mainId,
              paymenTerm: item.paymenTerm,
              paymentAmount: Number(item.paymentAmount) * 100,
              paymentBatch: item.paymentBatch,
              paymentMethod: item.paymentMethod,
              paymentProportion: item.paymentProportion,
              paymentType: item.paymentType
            }
          })
          if (this.form.id) {
            saveOrUpdateContract(this.form).then((res) => {
              if (res.code === 200) {
                const obj = {
                  code: res.code,
                  id: this.form.id
                }
                this.$emit('submitContractList', obj)
              }
            })
          } else {
            saveOrUpdateContract(this.form).then((res) => {
              if (res.code === 200) {
                const obj = {
                  code: res.code,
                  id: this.form.id
                }
                this.$emit('submitContractList', obj)
              }
            })
          }
        }
      })
    },
    /** 金额输入框验证 */
    limitInput(value, name) {
      this.form[name] = this.replaceInputValue(value)
    },
    limitPaymentAmountInput(value, index) {
      this.contractTable1[index].paymentAmount = this.replaceInputValue(value)
    },
    replaceInputValue(value) {
      return ('' + value) // 第一步：转成字符串
        .replace(/[^\d^\.]+/g, '') // 第二步：把不是数字，不是小数点的过滤掉
        .replace(/^0+(\d)/, '$1') // 第三步：第一位0开头，0后面为数字，则过滤掉，取后面的数字
        .replace(/^\./, '0.') // 第四步：如果输入的第一位为小数点，则替换成 0. 实现自动补全
        .match(/^\d*(\.?\d{0,2})/g)[0] || '' // 第五步：最终匹配得到结果 以数字开头，只有一个小数点，而且小数点后面只能有0到2位小数
    },
    /** 状态 */
    getContractStatus(key) {
      const map = new Map([
        ['0', '审核中'],
        ['1', '已驳回'],
        ['2', '签订中'],
        ['3', '生效中'],
        ['4', '已完成']
      ])
      return map.get(key)
    },
    getSupplierId(data) {
      console.log(data)
      this.supplierOptionsArr.forEach(item => {
        if (data === item.id) {
          this.form.supplierName = item.name
          this.form.supplierId = item.id
        }
      })
    }
  }
}
</script>

<style lang="scss" scoped>
.base_info_desc{
  ::v-deep .el-input__inner{
    border: none;
    border-radius: 0;
    height: 48px;
  }
  ::v-deep .el-descriptions--medium.is-bordered .el-descriptions-item__cell{
    padding: 0px;
  }
  ::v-deep .el-descriptions-item__cell.el-descriptions-item__label.is-bordered-label {
    padding-left: 16px;
  }
  .warnTip{
    line-height: 48px;
    color:#EF5826;
  }
  ::v-deep .el-input.is-disabled .el-input__inner,::v-deep .el-range-editor.is-disabled,::v-deep .el-range-editor.is-disabled input{
    background: #fff;
    cursor: default;
  }
}
.numInputClass
,.priceInputClass
,.taxInputClass
,.supplierInputClass
,.startDateInputClass
,.typeInputClass
,.getDateInputClass
,.phoneClass
,.registeredAddressClass
,.bankNameClass
,.bankAccountNameClass
,.bankAccountClass
,.dutyParagraphClass{
  ::v-deep .el-input__inner{
    border: 1px solid #EF5826;
  }
}
.redDot{
  color: #EF5826;
}
.date_box {
  width: 100%;
  position: relative;
  // width: fit-content;
  .data_icon {
    position: absolute;
    top: 50%;
    right: 17px;
    z-index: 9;
    color: #c0c4cc;
    font-size: 14px;
    transform: translateY(-50%);
  }
  ::v-deep .el-input__icon.el-icon-date {
    display: none;
  }
  ::v-deep .el-input__inner{
    padding-left: 16px;
  }
  ::v-deep .el-range-separator{
    line-height: 40px;
  }
  ::v-deep .el-date-editor.el-input{
    width: 100%;
  }
}
.file-list{
  display: flex;
  justify-content: space-between;
  margin-top: 10px;
}
.fileName{
  cursor: pointer;
}
.fileName:hover{
  color: #4686EF;
}
.my-select-con{
  .icon-bumenxuanze {
    display: inline-block;
    cursor: pointer;
    width: 22px;
    height: 22px;
    position: absolute;
    right: 6px;
    bottom: 0;
    top:5px
  }
  ::v-deep .el-select {
    width: 100%;
    .el-input__suffix-inner {
      display: none;
    }
  }
}
</style>
