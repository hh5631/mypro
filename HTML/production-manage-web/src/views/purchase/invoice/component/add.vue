<template>
  <div>
    <collapse title="基本信息">
      <div class="produce-collapse">
        <el-form ref="mainForm" :model="form" :rules="rules">
          <el-descriptions class="base_info_desc" :column="2" border>
            <el-descriptions-item>
              <template slot="label">
                发票号码<span v-if="!isDisabled" class="redDot">*</span>
              </template>
              <el-form-item prop="invoiceNo">
                <el-input
                  v-model.trim="form.invoiceNo"
                  :disabled="isDisabled"
                  :placeholder="placeholderInput"
                  :class="{'numInputClass':isNumWarn}"
                  maxlength="10"
                  @input="numInput()"
                /></el-form-item>
              <!-- <span v-if="isNumWarn" slot="suffix" class="warnTip"><i class="el-icon-warning-outline" style="color:#EF5826" />{{ warnText }}</span></el-input> -->
            </el-descriptions-item>
            <el-descriptions-item>
              <template slot="label">
                发票金额(元)<span v-if="!isDisabled" class="redDot">*</span>
              </template>
              <el-form-item prop="invoiceAmount">
                <el-input
                  v-model.trim="form.invoiceAmount"
                  :disabled="isDisabled"
                  :class="{'priceInputClass':isPriceWarn}"
                  :placeholder="placeholderInput"
                  maxlength="12"
                  @blur="inputMoney($event,'invoiceAmount')"
                  @input="priceInput()"
                  @focus="priceFocus('invoiceAmount')"
                >
                  <!-- <span v-if="isPriceWarn" slot="suffix" class="warnTip"><i class="el-icon-warning-outline" style="color:#EF5826" />{{ warnText }}</span> -->
                </el-input>
              </el-form-item>
            </el-descriptions-item>
            <el-descriptions-item>
              <template slot="label">
                发票税额(元)<span v-if="!isDisabled" class="redDot">*</span>
              </template>
              <el-form-item prop="invoiceAmount">
                <el-input
                  v-model.trim="form.invoiceTaxAmount"
                  :disabled="isDisabled"
                  :class="{'taxInputClass':isTaxWarn}"
                  :placeholder="placeholderInput"
                  maxlength="12"
                  @blur="inputMoney($event,'invoiceTaxAmount')"
                  @input="taxInput()"
                  @focus="priceFocus('invoiceTaxAmount')"
                >
                  <!-- <span v-if="isTaxWarn" slot="suffix" class="warnTip"><i class="el-icon-warning-outline" style="color:#EF5826" />{{ warnText }}
                  </span> -->
                </el-input>
              </el-form-item>
            </el-descriptions-item>
            <el-descriptions-item>
              <template slot="label">
                供应商<span v-if="!isDisabled" class="redDot">*</span>
              </template>
              <el-form-item prop="supplierId" @click="showSupplier">
                <!-- <el-input
                  v-model.trim="supplierName"
                  :disabled="true"
                  :class="{'supplierInputClass':isSupplierWarn}"
                  :placeholder="placeholderSelect"
                  maxlength="20"
                  @input="supplierInput()"
                  @click.native="showSupplier"
                >
                  <i v-if="!isDisabled" slot="suffix" class="iconfont icon-bumenxuanze" style="line-height: 48px;cursor: pointer;margin-right: 12px;" @click="showSupplier" />
                </el-input> -->
                <div class="my-select-con">
                  <el-select v-model.trim="form.supplierName" :disabled="isDisabled" style="width:100%;" filterable placeholder="请选择" :class="{'supplierInputClass':isSupplierWarn}">
                    <el-option
                      v-for="item in supplierArr"
                      :key="item.value"
                      :label="item.label"
                      :value="item.value"
                    />
                  </el-select>
                  <i v-if="!isDisabled" slot="suffix" class="iconfont icon-bumenxuanze" style="font-size: 14px;line-height: 36px;cursor: pointer;" @click="showSupplier" />
                </div>
              </el-form-item>
            </el-descriptions-item>
            <el-descriptions-item>
              <template slot="label">
                票据数量
              </template>
              <el-input v-model.number="form.billQty" :disabled="isDisabled" :placeholder="placeholderInput" maxlength="2" oninput="value=value.replace(/^(0+)|[^\d]+/g,'')" />
            </el-descriptions-item>
            <el-descriptions-item>
              <template slot="label">
                开票日期<span v-if="!isDisabled" class="redDot">*</span>
              </template>
              <el-form-item prop="billingDate">
                <div v-if="!isDisabled" class="date_box hide-icon">
                  <el-date-picker
                    ref="dateSelect1"
                    v-model="form.billingDate"
                    :editable="false"
                    :disabled="isDisabled"
                    :class="{'startDateInputClass':isStartDateWare}"
                    :on-pick="startDateInput()"
                    size="small"
                    type="date"
                    value-format="yyyy-MM-dd"
                    :placeholder="placeholderSelect"
                    :clearable="false"
                  />
                  <i class="el-icon-date data_icon" @click="handleDatePicker1" />
                </div>
              </el-form-item>
              <el-input v-if="isDisabled" v-model.number="form.billingDate" disabled />
            </el-descriptions-item>
            <el-descriptions-item>
              <template slot="label">
                发票类型<span v-if="!isDisabled" class="redDot">*</span>
              </template>
              <el-form-item prop="invoiceType">
                <el-select v-if="!isDisabled" v-model.trim="form.invoiceType" :disabled="isDisabled" style="width:100%;" :placeholder="isDisabled?'':'请选择'" :class="{'typeInputClass':isTypeWarn}" @input="typeInput()">
                  <el-option
                    v-for="item in dict.type.invoice_type"
                    :key="item.value"
                    :label="item.label"
                    :value="item.value"
                  />
                </el-select>
              </el-form-item>
              <el-input v-if="isDisabled" :value="invoiceTypeText" disabled />
            </el-descriptions-item>
            <el-descriptions-item>
              <template slot="label">
                收到日期<span v-if="!isDisabled" class="redDot">*</span>
              </template>
              <el-form-item prop="receivedDate">
                <div v-if="!isDisabled" class="date_box hide-icon">
                  <el-date-picker
                    ref="dateSelect2"
                    v-model="form.receivedDate"
                    :editable="false"
                    :disabled="isDisabled"
                    :class="{'getDateInputClass':isGetDateWarn}"
                    :on-pick="getDateInput()"
                    size="small"
                    type="date"
                    value-format="yyyy-MM-dd"
                    :placeholder="isDisabled?'':'请选择'"
                    :clearable="false"
                  /><i class="el-icon-date data_icon" @click="handleDatePicker2" />
                </div>
              </el-form-item>
              <el-input v-if="isDisabled" v-model.number="form.receivedDate" disabled />
            </el-descriptions-item>
            <el-descriptions-item>
              <template slot="label">
                发票介质
              </template>
              <el-select v-if="!isDisabled" v-model.trim="form.invoiceMedia" :disabled="isDisabled" style="width:100%;" :placeholder="isDisabled?'':'请选择'">
                <el-option
                  v-for="item in dict.type.invoice_media"
                  :key="item.value"
                  :label="item.label"
                  :value="item.value"
                />
              </el-select>
              <el-input v-if="isDisabled" :value="invoiceMediaText" disabled />
            </el-descriptions-item>
            <el-descriptions-item>
              <template slot="label">
                合同编号
              </template>
              <el-form-item prop="contractNo">
                <el-input v-model.trim="form.contractNo" :disabled="isDisabled" maxlength="12" :placeholder="placeholderInput" @input="contractNoInput()">
                  <!-- <span v-if="isContractNoWarn" slot="suffix" class="warnTip"><i class="el-icon-warning-outline" style="color:#EF5826" />{{ warnText }}</span> -->
                </el-input>
              </el-form-item>

            </el-descriptions-item>
            <el-descriptions-item>
              <template slot="label">
                合同金额(元)
              </template>
              <el-input v-model.trim="form.contractAmount" :disabled="isDisabled" :placeholder="placeholderInput" maxlength="12" @blur="inputMoney($event,'contractAmount')" @focus="priceFocus('contractAmount')">
                <span v-if="isContractAmountWarn" slot="suffix" class="warnTip"><i class="el-icon-warning-outline" style="color:#EF5826" />{{ warnText }}</span>
              </el-input>
            </el-descriptions-item>
            <el-descriptions-item>
              <template slot="label">
                采购单号
              </template>
              <el-input v-model.trim="form.procurementOrderNo" :disabled="isDisabled" :placeholder="placeholderInput" maxlength="15">
                <span v-if="isProcurementOrderNoWarn" slot="suffix" class="warnTip"><i class="el-icon-warning-outline" style="color:#EF5826" />{{ warnText }}</span>
              </el-input>
            </el-descriptions-item>
            <el-descriptions-item>
              <template slot="label">
                备注
              </template>
              <el-input v-model.trim="form.remark" :disabled="isDisabled" :placeholder="placeholderInput" maxlength="100" />
            </el-descriptions-item>
          </el-descriptions>
        </el-form>
      </div>
    </collapse>
    <collapse title="附件信息" style="margin-top:10px">
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
      <selectApply ref="selectApply" />
      <div slot="footer" class="dialog-footer">
        <el-button size="small" @click="cancelSupplier">取 消</el-button>
        <el-button v-debounce size="small" type="primary" @click="submitSupplier">保 存</el-button>
      </div>
    </el-dialog>
    <!-- <el-dialog
      title="选择供应商"
      :visible.sync="selectApplyDig"
      width="1200px"
      append-to-body
      :close-on-click-modal="false"
      class="selectPlan"
      @close="closeApplyDig"
    ><selectApply ref="selectApply" /><div slot="footer" class="dialog-footer">
      <el-button size="small" style="float:left" @click="addApply"><i class="iconfont icon-zengjia" />新增供应商</el-button>
      <el-button size="small" @click="closeApplyDig">取消</el-button>
      <el-button size="small" type="primary" @click="submitForm1">保存</el-button>
    </div></el-dialog> -->
    <el-dialog
      title="新建供应商"
      :visible.sync="addApplyDig"
      width="1200px"
      append-to-body
      :close-on-click-modal="false"
      class="selectPlan1"
      @close="closeAddApplyDig"
    ><addSupplier ref="addSupplier" :show="addApplyDig" /><div slot="footer" class="dialog-footer">
      <el-button size="small" @click="closeAddApplyDig">取消</el-button>
      <el-button size="small" type="primary" @click="submitForm2">保存</el-button>
    </div></el-dialog>
  </div>
</template>

<script>
import collapse from './collapse'
import { saveOrUpdateInvoice } from '@/api/purchase/invoice'
import selectApply from '../../apply/component/selectApply'
import addSupplier from '../../apply/component/addSupplier'
import { formatPrice, delcommafy } from '@/utils/index'
import { getSupplierList } from '@/api/basicdata/index'
import AttachmentUpload from '@/components/AttachmentUpload'
export default {
  components: { collapse, selectApply, addSupplier, AttachmentUpload },
  dicts: ['invoice_type', 'invoice_media'],
  props: {
    formData: {
      type: Object,
      default: () => {}
    },
    isDisabled: {
      type: Boolean,
      default: false
    }
  },
  data() {
    return {
      title: '',
      form: {},
      dateRange: [],
      isNumWarn: false,
      isPriceWarn: false,
      isTaxWarn: false,
      isSupplierWarn: false,
      isStartDateWare: false,
      isTypeWarn: false,
      isGetDateWarn: false,
      isContractNoWarn: false,
      isContractAmountWarn: false,
      isProcurementOrderNoWarn: false,
      warnText: '请填写此项',
      selectApplyDig: false,
      addApplyDig: false,
      supplierArr: [],
      showSupplierDialog: false,
      supplierName: '',
      rules: {
        invoiceNo: [{ required: true, message: '请填写此项' },
          { pattern: /^[a-zA-Z0-9]+$/, message: '仅支持数字或英文' }],
        invoiceAmount: [{ required: true, message: '请填写此项' }],
        invoiceTaxAmount: [{ required: true, message: '请填写此项' }],
        supplierId: [{ required: true, message: '请填写此项' }],
        billingDate: [{ required: true, message: '请填写此项' }],
        invoiceType: [{ required: true, message: '请填写此项' }],
        receivedDate: [{ required: true, message: '请填写此项' }],
        contractNo: [{ pattern: /([\p{Han}\p{P}A-Za-z0-9])*$/, message: '支持数字、英文和符号' },
          { min: 3, max: 12, message: '长度大于3小于12' }]
      }
    }
  },
  computed: {
    invoiceMediaText() {
      const result = this.dict.type.invoice_media.find(item => item.value === this.form.invoiceMedia)
      return result ? result.label : ''
    },
    invoiceTypeText() {
      const result = this.dict.type.invoice_type.find(item => item.value === this.form.invoiceType)
      return result ? result.label : ''
    },
    placeholderInput() {
      return this.isDisabled ? '' : '请输入'
    },
    placeholderSelect() {
      return this.isDisabled ? '' : '请输入'
    }
  },
  created() {
    this.getSupplierList()
  },
  methods: {
    reset() {
      this.form = {
        invoiceNo: '', // 发票号码
        invoiceAmount: '', // 发票金额
        invoiceTaxAmount: '', // 发票税额
        supplierId: '', // 供应商,
        billQty: '', // 发票数量
        billingDate: '', // 开票日期
        invoiceType: '', // 发票类型
        invoiceMedia: '', // 发票介质
        contractNo: '', // 合同编号
        contractAmount: '', // 合同金额
        procurementOrderNo: '', // 采购单号
        remark: '', // 备注
        fileList: []
      }
      this.supplierName = ''
      this.isNumWarn = false
      this.isPriceWarn = false
      this.isTaxWarn = false
      this.isSupplierWarn = false
      this.isStartDateWare = false
      this.isTypeWarn = false
      this.isGetDateWarn = false
      this.isContractNoWarn = false
      this.isContractAmountWarn = false
      this.isProcurementOrderNoWarn = false
      this.$refs.mainForm.resetFields()
    },
    handleDatePicker1() {
      this.$refs.dateSelect1.focus()
    },
    handleDatePicker2() {
      this.$refs.dateSelect2.focus()
    },
    inputMoney(el, name) {
      // this.form[name] = this.getInputValue(el)
      this.form[name] = el.target.value && Number(el.target.value) ? formatPrice(Number(el.target.value).toFixed(2)) : ''
    },
    showApplyDig() {
      this.selectApplyDig = true
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
    // 表单校验
    validateForm() {
      return new Promise(resolve => {
        this.$refs.mainForm.validate((valid) => {
          resolve(valid)
        })
      })
      // 数字和英文正则
      // const reg1 = /^[a-zA-Z0-9]+$/
      // // 发票号码校验
      // if (!this.form.invoiceNo) {
      //   this.isNumWarn = true
      //   this.warnText = '请填写此项'
      //   return false
      // }
      // if (!reg1.test(this.form.invoiceNo)) {
      //   this.isNumWarn = true
      //   this.warnText = '仅支持数字或英文'
      //   return false
      // }
      // // 发票金额校验
      // if (!this.form.invoiceAmount) {
      //   this.isPriceWarn = true
      //   this.warnText = '请填写此项'
      //   return false
      // }
      // // 发票税额
      // if (!this.form.invoiceTaxAmount) {
      //   this.isTaxWarn = true
      //   return false
      // }
      // if (!this.form.supplierId) {
      //   this.isSupplierWarn = true
      //   this.warnText = '请选择此项'
      //   return false
      // }
      // if (!this.form.billingDate) {
      //   this.isStartDateWare = true
      //   this.warnText = '请填写此项'
      //   return false
      // }
      // if (!this.form.invoiceType) {
      //   this.isTypeWarn = true
      //   this.warnText = '请填写此项'
      //   return false
      // }
      // if (!this.form.receivedDate) {
      //   this.isGetDateWarn = true
      //   this.warnText = '请填写此项'
      //   return false
      // }
      // // 合同编号校验
      // if (/^[\u4e00-\u9fa5]+$/.test(this.form.contractNo)) {
      //   this.isContractNoWarn = true
      //   this.warnText = '支持数字、英文和符号'
      //   return false
      // }
      // if (this.form.contractNo && this.form.contractNo.length < 3) {
      //   this.isContractNoWarn = true
      //   this.warnText = '长度大于3小于12'
      //   return false
      // }
      // return true
    },

    // 提交表单
    submitForm() {
      const data = JSON.parse(JSON.stringify(this.form))
      data.invoiceAmount = data.invoiceAmount ? delcommafy(data.invoiceAmount) * 100 : ''// 金额单位为分
      data.invoiceTaxAmount = data.invoiceTaxAmount ? delcommafy(data.invoiceTaxAmount) * 100 : ''
      data.contractAmount = data.contractAmount ? delcommafy(data.contractAmount) * 100 : ''

      this.validateForm().then(da => {
        if (da) {
          saveOrUpdateInvoice(data).then((res) => {
            if (res.code === 200) {
              const obj = {
                code: res.code,
                id: this.form.id
              }
              this.$emit('successSubmit', obj)
            }
          })
        }
      })
    },
    getDetail() {
      this.form = this.formData
      this.supplierName = this.formData.supplierName
    },
    numInput() {
      if (this.form.invoiceNo) {
        this.isNumWarn = false
      }
    },
    priceInput() {
      if (this.form.invoiceAmount) {
        this.isPriceWarn = false
        this.form.invoiceAmount = delcommafy(this.form.invoiceAmount) // 金额单位为分
      }
    },
    taxInput() {
      if (this.form.invoiceTaxAmount) {
        this.isTaxWarn = false
      }
    },
    supplierInput() {
      if (this.form.supplierId) {
        this.isSupplierWarn = false
      }
    },
    startDateInput() {
      if (this.form.billingDate) {
        this.isStartDateWare = false
      }
    },
    typeInput() {
      if (this.form.invoiceType) {
        this.isTypeWarn = false
      }
    },
    getDateInput() {
      if (this.form.receivedDate) {
        this.isGetDateWarn = false
      }
    },
    contractNoInput() {
      if (this.form.contractNo) {
        this.isContractNoWarn = false
      }
    },
    contractAmountInput() {
      if (this.form.contractAmount) {
        this.isContractAmountWarn = false
      }
    },
    procurementOrderNoInput() {
      if (this.form.procurementOrderNo) {
        this.isProcurementOrderNoWarn = false
      }
    },
    // handleExceed(files, fileList) {
    //   this.$message.error(`最多选择 3 个文件`)
    //   return
    // },
    submitForm1() { },
    submitForm2() {
      this.$refs.addSupplier.submitForm()
    },
    // 获取供应商数据
    getSupplierList() {
      this.supplierArr = []
      const data = {
        pageNum: 1,
        pageSize: 10,
        enable: 0
      }

      getSupplierList(data).then(
        (res) => {
          if (res.code === 200) {
            res.data.records.forEach((e) => {
              const obj = {
                value: e.id,
                label: e.name
              }
              this.supplierArr.push(obj)
            })
          } else {
            this.$message.error(res.msg)
          }
        }
      )
    },
    priceFocus(name) {
      this.form[name] = this.form[name] ? delcommafy(this.form[name]) : ''
    },
    /** 显示供应商选择对象*/
    showSupplier() {
      this.showSupplierDialog = true
    },
    closeSupplier() {
      this.showSupplierDialog = false
    },
    cancelSupplier() {
      this.showSupplierDialog = false
    },
    /** 设置供应商id*/
    submitSupplier() {
      this.showSupplierDialog = false
      this.form.supplierId = this.$refs.selectApply.currentRow.id
      this.supplierName = this.$refs.selectApply.currentRow.name
      if (this.form.supplierId) {
        //
        this.$refs.mainForm.fields.map(i => {
          //
          if (i.prop === 'supplierId') {
            // 清空验证
            //
            i.clearValidate()
          }
        })
      }
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
.hide-icon {
  ::v-deep .el-input__prefix {
    .el-input__icon {
      display: none;
    }
  }
  ::v-deep .el-input__inner {
    padding-left: 15px;
  }
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
  ::v-deep .el-input__icon.el-range__icon.el-icon-date {
    display: none;
  }
  ::v-deep .el-range-separator{
    line-height: 40px;
  }
  ::v-deep .el-date-editor.el-input{
    width: 100%;
  }
}
// .warn{
//   ::v-deep .el-descriptions .is-bordered .el-descriptions-item__cell{
//     border: 1px solid red;
//   }
// }
.upload{
  display: flex;
  align-items: center;
}
.fileTip{
  color: #B2B5B9;
  font-size: 14px;
  padding-left: 8px;
}
.fileName{
  cursor: pointer;
}
.fileName:hover{
  color: #4686EF;
}
.file-list{
  display: flex;
  justify-content: space-between;
  margin-top: 10px;
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
    top: 14px;
    background: url("../../../../assets/images/bumen@2x.png") no-repeat;
    background-position: center;
    background-size: 100% 100%;
  }
  .icon-bumenxuanze::before{
    content: '';
  }
  // 下来箭头隐藏
  ::v-deep .el-select {
    width: 100%;
    .el-input__suffix-inner {
      display: none;
    }
  }
}
</style>
