<template>
  <div class="produce-collapse">
    <el-form ref="mainForm" :model="form" :rules="rules">
      <collapse title="基本信息">
        <el-descriptions class="base_info_desc" :column="2" border>
          <el-descriptions-item :span="2" class="warn">
            <template slot="label">
              供应商名称<span v-if="!isDisable" class="redDot">*</span>
            </template>
            <el-form-item prop="name">
              <el-input
                v-model="form.name"
                :disabled="isDisable"
                :placeholder="isDisable?'':'请输入'"
                maxlength="60"
                @blur="getSupplierCode"
              />
            </el-form-item>
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label"> 供应商代码 </template>
            <el-form-item prop="code">
              <el-input
                v-model="form.code"
                :disabled="isDisable"
                readonly
                placeholder="系统根据编码规则自动生成"
              />
            </el-form-item>
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label">
              统一社会信用代码
              <!-- <span v-if="!isDisable" class="redDot">*</span> -->
            </template>
            <el-form-item prop="account">
              <el-input
                v-model="form.account"
                :disabled="isDisable"
                :placeholder="isDisable?'':'请输入'"
                :minlength="18"
                :maxlength="18"
              />
            </el-form-item>
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label">
              法人代表
            </template>
            <el-form-item prop="legalPerson">
              <el-input
                v-model="form.legalPerson"
                :disabled="isDisable"
                maxlength="15"
                :placeholder="isDisable?'':'请输入'"
              />
            </el-form-item>
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label">
              经营状态
              <!-- <span v-if="!isDisable" class="redDot">*</span> -->
            </template>
            <el-form-item prop="operatingStatus">
              <el-select
                v-model="form.operatingStatus"
                :class="[isDisable ? 'hideSuffix' : '']"
                :disabled="isDisable"
                style="width: 100%"
                :placeholder="isDisable?'':'请选择'"
              >
                <el-option
                  v-for="item in dict.type.ent_operating_status"
                  :key="item.value"
                  :label="item.label"
                  :value="item.value"
                />
              </el-select>
            </el-form-item>
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label">
              注册资金(万)
              <!-- <span v-if="!isDisable" class="redDot">*</span> -->
            </template>
            <el-form-item prop="registeredCapital">
              <el-input
                v-model="form.registeredCapital"
                class="unit-suffix"
                :disabled="isDisable"
                maxlength="11"
                :placeholder="isDisable?'':'请输入'"
              />
            </el-form-item>
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label">
              纳税人资质
              <!-- <span v-if="!isDisable" class="redDot">*</span> -->
            </template>
            <el-form-item prop="taxpayerQualification">
              <el-select
                v-model="form.taxpayerQualification"
                :class="[isDisable ? 'hideSuffix' : '']"
                :disabled="isDisable"
                style="width: 100%"
                :placeholder="isDisable?'':'请选择'"
              >
                <el-option
                  v-for="item in dict.type.taxpayers_type"
                  :key="item.value"
                  :label="item.label"
                  :value="item.value"
                />
              </el-select>
            </el-form-item>
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label"> 所属行业 </template>
            <el-form-item prop="industry">
              <el-input
                v-model.trim="form.industry"
                :disabled="isDisable"
                :placeholder="!isDisable?'请输入':''"
                maxlength="15"
                show-word-limit
              />
            </el-form-item>
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label">
              联系人
              <!-- <span v-if="!isDisable" class="redDot">*</span> -->
            </template>
            <el-form-item prop="contacts">
              <el-input
                v-model="form.contacts"
                :disabled="isDisable"
                :placeholder="isDisable?'':'请输入'"
              />
            </el-form-item>
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label">
              联系电话
              <!-- <span v-if="!isDisable" class="redDot">*</span> -->
            </template>
            <el-form-item prop="phone">
              <el-input
                v-model.trim="form.phone"
                :disabled="isDisable"
                :placeholder="isDisable?'':'请输入'"
                :maxlength="13"
                :minlength="7"
              />
            </el-form-item>
          </el-descriptions-item>
          <el-descriptions-item :span="2">
            <template slot="label">
              注册地址
              <!-- <span v-if="!isDisable" class="redDot">*</span> -->
            </template>
            <el-form-item prop="registeredAddress">
              <el-input
                v-model="form.registeredAddress"
                :disabled="isDisable"
                :placeholder="isDisable?'':'请输入'"
              />
            </el-form-item>
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label"> 单位简介 </template>
            <el-form-item prop="profile">
              <el-input
                v-model="form.profile"
                :disabled="isDisable"
                :placeholder="!isDisable?'请输入':''"
                maxlength="500"
                show-word-limit
              />
            </el-form-item>
          </el-descriptions-item>
        </el-descriptions>
      </collapse>
      <collapse title="财务信息" style="margin-top: 10px">
        <el-descriptions class="base_info_desc" :column="2" border>
          <el-descriptions-item>
            <template slot="label">
              开户行
              <!-- <span v-if="!isDisable" class="redDot">*</span> -->
            </template>
            <el-form-item prop="bankName">
              <el-input
                v-model="form.bankName"
                :disabled="isDisable"
                maxlength="15"
                :placeholder="isDisable?'':'请输入'"
              />
            </el-form-item>
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label">
              开户名
              <!-- <span v-if="!isDisable" class="redDot">*</span> -->
            </template>
            <el-form-item prop="bankAccountName">
              <el-input
                v-model="form.bankAccountName"
                :disabled="isDisable"
                :placeholder="isDisable?'':'请输入'"
              />
            </el-form-item>
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label">
              银行账户
              <!-- <span v-if="!isDisable" class="redDot">*</span> -->
            </template>
            <el-form-item prop="bankAccount">
              <el-input
                v-model="form.bankAccount"
                :disabled="isDisable"
                maxlength="20"
                :placeholder="isDisable?'':'请输入'"
              />
            </el-form-item>
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label">
              税号
              <!-- <span v-if="!isDisable" class="redDot">*</span> -->
            </template>
            <el-form-item prop="dutyParagraph">
              <el-input
                v-model="form.dutyParagraph"
                :disabled="isDisable"
                :placeholder="isDisable?'':'请输入'"
                maxlength="20"
                show-word-limit
              />
            </el-form-item>
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label">财务主管<span :style="{paddingRight: !isDisable?'16px':'10px'}" /> </template>
            <el-form-item prop="financeOfficer">
              <el-input
                v-model="form.financeOfficer"
                :disabled="isDisable"
                :placeholder="!isDisable?'请输入':''"
                maxlength="15"
                show-word-limit
              />
            </el-form-item>
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label"> 财务电话<span :style="{paddingRight: !isDisable?'60px':'50px'}" /> </template>
            <el-form-item prop="financePhone">
              <el-input
                v-model.trim="form.financePhone"
                :disabled="isDisable"
                :placeholder="!isDisable?'请输入':''"
              />
            </el-form-item>
          </el-descriptions-item>
        </el-descriptions>
      </collapse>
      <collapse title="更多信息" style="margin-top: 10px">
        <el-descriptions class="base_info_desc" :column="2" border>
          <el-descriptions-item>
            <template slot="label"> 企业类型<span style="padding-right:22px" /> </template>
            <el-form-item prop="companyType">
              <el-select
                v-model="form.companyType"
                :disabled="isDisable"
                :class="[isDisable ? 'hideSuffix' : '']"
                style="width: 100%"
                :placeholder="!isDisable?'请选择':''"
              >
                <el-option
                  v-for="item in dict.type.company_type"
                  :key="item.value"
                  :label="item.label"
                  :value="item.value"
                />
              </el-select>
            </el-form-item>
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label"> 企业分类 </template>
            <el-form-item prop="companyClassification">
              <el-select
                v-model="form.companyClassification"
                :disabled="isDisable"
                :class="[isDisable ? 'hideSuffix' : '']"
                style="width: 100%"
                :placeholder="!isDisable?'请选择':''"
              >
                <el-option
                  v-for="item in dict.type.ent_classification"
                  :key="item.value"
                  :label="item.label"
                  :value="item.value"
                />
              </el-select>
            </el-form-item>
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label"> 经营方式 </template>
            <el-form-item prop="operationMode">
              <el-select
                v-model="form.operationMode"
                :disabled="isDisable"
                :class="[isDisable ? 'hideSuffix' : '']"
                style="width: 100%"
                :placeholder="!isDisable?'请选择':''"
              >
                <el-option
                  v-for="item in dict.type.ent_operating_type"
                  :key="item.value"
                  :label="item.label"
                  :value="item.value"
                />
              </el-select>
            </el-form-item>
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label"> 人员规模 </template>
            <el-form-item prop="personnelSize">
              <el-input
                v-model="form.personnelSize"
                :disabled="isDisable"
                :placeholder="!isDisable?'请输入':''"
              />
            </el-form-item>
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label"> 营业期限 </template>
            <div class="date_box">
              <!-- <span v-if="isDisable && !dateRange[0] && !dateRange[1]">
              - -
            </span> -->
              <el-date-picker
                ref="period"
                v-model="dateRange"
                :disabled="isDisable"
                size="small"
                style="width: 200px"
                value-format="yyyy-MM-dd"
                type="daterange"
                :class="[isDisable ? 'hideSuffix' : '']"
                :range-separator="
                  isDisable && !dateRange[0] && !dateRange[1] ? '' : '~'
                "
                :start-placeholder="
                  isDisable && !dateRange[0] && !dateRange[1]
                    ? ''
                    : '开始日期'
                "
                :end-placeholder="
                  isDisable && !dateRange[0] && !dateRange[1] ? '' : '结束日期'
                "
                :clearable="false"
              />
              <i class="el-icon-date data_icon" @click="showPicker('period')" />
            </div>
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label"> 主营品牌 </template>
            <el-form-item prop="mainBrands">
              <el-input
                v-model="form.mainBrands"
                :disabled="isDisable"
                :placeholder="!isDisable?'请输入':''"
                maxlength="500"
                show-word-limit
              />
            </el-form-item>
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label"> 主营范围 </template>
            <el-form-item prop="mainProducts">
              <el-input
                v-model="form.mainProducts"
                :disabled="isDisable"
                :placeholder="!isDisable?'请输入':''"
                maxlength="500"
                show-word-limit
              />
            </el-form-item>
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label"> 经营范围 </template>
            <!-- <el-select v-model="form.businessScope" style="width:100%;" :placeholder="isDisable?'':请选择">
            <el-option
              v-for="item in options"
              :key="item.value"
              :label="item.label"
              :value="item.value"
            />
          </el-select> -->
            <el-form-item prop="businessScope">
              <el-input
                v-model="form.businessScope"
                :disabled="isDisable"
                maxlength="500"
                show-word-limit
                :placeholder="!isDisable?'请输入':''"
              />
            </el-form-item>
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label"> 企业等级 </template>
            <el-form-item prop="companyLevel">
              <el-input
                v-model="form.companyLevel"
                :disabled="isDisable"
                :placeholder="!isDisable?'请输入':''"
                maxlength="100"
                show-word-limit
              />
            </el-form-item>
          </el-descriptions-item>
          <el-descriptions-item v-if="!isDisable">
            <template slot="label"> 注册时间 </template>
            <!-- <el-input v-model="form.buildDate" :disabled="isDisable" :placeholder="isDisable?'':请输入" /> -->
            <el-form-item prop="buildDate">
              <el-date-picker
                v-model="form.buildDate"
                style="width: 100%"
                class="register-date"
                value-format="yyyy-MM-dd"
                :picker-options="buildDateOptions"
                :disabled="isDisable"
                type="date"
                :placeholder="!isDisable?'请选择':''"
              />
            </el-form-item>
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label"> 所属国家 </template>
            <el-form-item prop="country">
              <el-input
                v-model="form.country"
                :disabled="isDisable"
                :placeholder="!isDisable?'请输入':''"
                maxlength="150"
                show-word-limit
              />
            </el-form-item>
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label"> 所属城市<span style="padding-right: 64px;" /> </template>
            <el-form-item prop="city">
              <el-input
                v-model="form.city"
                :disabled="isDisable"
                :placeholder="!isDisable?'请输入':''"
                maxlength="150"
                show-word-limit
              />
            </el-form-item>
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label"> 经营地址 </template>
            <el-form-item prop="businessAddress">
              <el-input
                v-model="form.businessAddress"
                :disabled="isDisable"
                :placeholder="!isDisable?'请输入':''"
                maxlength="150"
              />
            </el-form-item>
          </el-descriptions-item>
          <el-descriptions-item>
            <template slot="label"> 单位邮编 </template>
            <el-form-item prop="zipCode">
              <el-input
                v-model.trim="form.zipCode"
                :disabled="isDisable"
                :placeholder="!isDisable?'请输入':''"
              />
            </el-form-item>
          </el-descriptions-item>
          <el-descriptions-item :span="2">
            <template slot="label"> 备注 </template>
            <el-form-item prop="remark">
              <el-input
                v-model="form.remark"
                :disabled="isDisable"
                :placeholder="!isDisable?'请输入内容':''"
                maxlength="500"
                show-word-limit
              />
            </el-form-item>
          </el-descriptions-item>
        </el-descriptions>
      </collapse>
      <collapse title="附件信息" style="margin-top: 10px">
        <attachment-upload v-model="fileList" :readonly="isDisable" />
      </collapse>
    </el-form>
  </div>
</template>

<script>
import collapse from '../../plan/component/collapse.vue'
import {
  saveOrUpdateSupplier,
  getSupplierCode
} from '@/api/basicdata/index.js'
import FormValidators from '@/vform/utils/validators'
import AttachmentUpload from '@/components/AttachmentUpload'
export default {
  components: { collapse, AttachmentUpload },
  dicts: [
    'company_type',
    'ent_classification',
    'ent_operating_status',
    'taxpayers_type',
    'ent_operating_type'
  ],
  props: {
    formData: {
      type: Object,
      default: () => {}
    }
  },
  data() {
    return {
      rules: {
        name: [{ required: true, message: '请输入供应商名称' },
          { validator: FormValidators.chineseAndLetter, message: '必须为汉字或英文' }
        ],
        account: [
          { min: 18, max: 18, message: '统一社会信用代码长度必须为18位' },
          { validator: FormValidators.upperLetterAndNumber, message: '必须为大写字母或数字' }
        ],
        legalPerson: [
          { validator: FormValidators.chineseAndLetter, message: '必须为汉字或英文' }],
        registeredCapital: [
          { validator: FormValidators.number, message: '必须为数字' }],
        bankName: [
          { validator: FormValidators.chinese, message: '必须为中文' }],
        mainProducts: [
          { validator: FormValidators.chinese, message: '必须为中文' }],
        businessScope: [
          { validator: FormValidators.chinese, message: '必须为中文' }],
        dutyParagraph: [
          { validator: FormValidators.letterAndNumber, message: '必须为字母或数字' }],
        contacts: [
          { validator: FormValidators.chineseAndLetter, message: '必须为汉字或英文' }],
        financeOfficer: [{ validator: FormValidators.chineseAndLetter, message: '必须为汉字或英文' }],
        country: [{ validator: FormValidators.chineseAndLetter, message: '必须为汉字或英文' }],
        city: [{ validator: FormValidators.chineseAndLetter, message: '必须为汉字或英文' }],
        financePhone: [{ validator: FormValidators.allPhone, message: '请输入正确的手机号或者电话号码' }],
        industry: [{ validator: FormValidators.chineseAndLetter, message: '必须为汉字或英文' }],
        phone: [
          { validator: FormValidators.allPhone, message: '请输入正确的手机号或者电话号码' }
        ],
        bankAccountName: [
          { validator: FormValidators.chineseAndLetter, message: '必须为汉字或英文' }
        ],
        bankAccount: [
          { validator: FormValidators.int, message: '必须为数字' }
        ],
        zipCode: [{ validator: FormValidators.zipCode, message: '必须是数字且长度为6' }
        ]
      },
      title: '',
      form: {
        code: undefined,
        name: undefined,
        account: undefined,
        bankAccount: undefined,
        bankAccountName: undefined,
        bankName: undefined,
        buildDate: undefined,
        businessAddress: undefined,
        businessScope: undefined,
        businessTermEndDate: undefined,
        businessTermStartDate: undefined,
        city: undefined,
        operationMode: undefined,
        companyClassification: undefined,
        companyLevel: undefined,
        companyType: undefined,
        contacts: undefined,
        country: undefined,
        dutyParagraph: undefined,
        fileList: [],
        // dateRange: [],
        financeOfficer: undefined,
        financePhone: undefined,
        industry: undefined,
        legalPerson: undefined,
        mainBrands: undefined,
        mainProducts: undefined,
        operatingStatus: undefined,
        personnelSize: undefined,
        phone: undefined,
        profile: undefined,
        registeredAddress: undefined,
        registeredCapital: '',
        remark: undefined,
        taxpayerQualification: undefined,
        zipCode: undefined
      },
      buildDateOptions: {
        disabledDate(time) {
          return time.getTime() >= Date.now() - 24 * 3600 * 1000
        }
      },
      dateRange: [],
      isDisable: false,
      fileList: []
    }
  },
  computed: {
    // dateRange() {
    //   return [this.formData.businessTermStartDate, this.formData.businessTermEndDate]
    // }
  },
  created () {
    // this.getSupplierCode()
  },

  methods: {
    showPicker(name) {
      this.$refs[name].focus()
    },
    reset() {
      this.fileList = []
      this.form.fileList = []
      this.dateRange = []
      this.$refs.mainForm.resetFields()
      this.isDisable = false
    },
    // 自动生成供应商编码
    getSupplierCode() {
      if (!this.form.name) {
        return
      }
      getSupplierCode({ name: this.form.name }).then((res) => {
        if (res.code === 200) {
          this.form.code = res.data
        }
      })
    },
    submitForm() {
      this.$refs.mainForm.validate((v) => {
        if (v) {
          this.form.fileList = []
          //
          if (this.dateRange?.length > 0) {
            this.form.businessTermStartDate = this.dateRange[0]
            this.form.businessTermEndDate = this.dateRange[1]
          }
          this.fileList.forEach((item, index) => {
            this.form.fileList.push(this.fileList[index])
          })

          // 如果是新增 ,默认状态设置为开启
          if (!this.form.enable) {
            this.form.enable = '0'
          }

          saveOrUpdateSupplier(this.form).then((res) => {
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
    edit() {
      for (const key in this.formData) {
        this.form[key] = this.formData[key]
      }
      this.fileList = this.formData.fileList
      if (this.form.businessTermStartDate && this.form.businessTermEndDate) {
        this.dateRange = [
          this.formData.businessTermStartDate,
          this.form.businessTermEndDate
        ]
      } else {
        this.dateRange = []
      }
      // if (this.dateRange?.length) {
      //   this.dateRange.push(this.formData.businessTermStartDate, this.form.businessTermEndDate)
      // }
      this.isDisable = false
    },
    getDetail() {
      for (const key in this.formData) {
        this.form[key] = this.formData[key]
      }
      this.fileList = this.formData.fileList
      if (this.form.businessTermStartDate && this.form.businessTermEndDate) {
        this.dateRange = [
          this.formData.businessTermStartDate,
          this.form.businessTermEndDate
        ]
      } else {
        this.dateRange = []
      }
      this.isDisable = true
    }
  }
}
</script>

<style lang="scss">
.hideSuffix {
  .el-input__suffix {
    display: none;
  }
}
.hideSuffix + .el-icon-date {
  display: none;
}
.base_info_desc {
  .el-descriptions-item__cell.el-descriptions-item__label.is-bordered-label {
    font-size: 14px;
    font-family: SourceHanSansCN-Bold, SourceHanSansCN;
    font-weight: bold;
    color: #333333;
    // text-shadow: 0px 2px 8px rgba(0,0,0,0.3000);
  }
}
.register-date {
  width:100%;
  .el-input__inner {
    padding-left: 13px;
  }
  .el-input__prefix {
    // display: none;
    left: inherit;
    right: 5px;
  }
  .el-icon-circle-close{
    margin-right:20px;
  }
}
</style>
<style lang="scss" scoped>
.produce-collapse {
  ::v-deep .el-form-item {
    margin-bottom: 0;
  }
  ::v-deep .el-form-item__error {
    top: auto;
    bottom: 0;
  }
  ::v-deep .el-input.is-disabled .el-input__inner {
    color: #000000;
  }
  ::v-deep .el-date-editor.el-range-editor.is-disabled .el-range-input {
    color: #000000;
  }
  ::v-deep .el-date-editor.el-range-editor.is-disabled .el-range-separator {
    color: #000000;
  }
  ::v-deep .el-collapse-item .content-title {
    font-size: 16px;
    font-family: SourceHanSansCN-Medium, SourceHanSansCN;
    font-weight: 500;
    color: #6a7697;
  }
}
.base_info_desc {
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
    font-size: 14px;
    font-family: SourceHanSansCN-Bold, SourceHanSansCN;
    font-weight: bold;
    color: #333333;
    line-height: 21px;
    // text-shadow: 0px 2px 8px rgba(0,0,0,0.3000);
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
.nameInputClass,
.accountInputClass,
.legalPersonInputClass,
.registeredCapitalInputClass,
.contactsClass,
.phoneClass,
.registeredAddressClass,
.bankNameClass,
.bankAccountNameClass,
.bankAccountClass,
.dutyParagraphClass {
  ::v-deep .el-input__inner {
    border: 1px solid #ef5826;
  }
}
.redDot {
  color: #ef5826;
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
  ::v-deep .el-range-separator {
    line-height: 40px;
  }
}
.fileTip {
  color: #b2b5b9;
  font-size: 14px;
  padding-left: 8px;
}
.fileName {
  cursor: pointer;
}
.fileName:hover{
  color: #4686EF;
}
// .warn{
//   ::v-deep .el-descriptions .is-bordered .el-descriptions-item__cell{
//     border: 1px solid red;
//   }
// }
.produce-collapse {
  .icon-zengjia {
    color: #7689d1;
    margin-right: 5px;
    font-size: 18px;
  }
}
.addAccessoryBtn {
  box-sizing: border-box;
  width: 104px;
  height: 32px;
  padding: 7px;
}
.file-list {
  display: flex;
  justify-content: space-between;
  margin-top: 10px;
}
.unit-suffix{
  ::v-deep .el-input__suffix{
    line-height:50px
  }
}
</style>
