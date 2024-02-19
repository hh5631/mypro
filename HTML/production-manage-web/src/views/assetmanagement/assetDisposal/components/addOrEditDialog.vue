<template>
  <div>
    <!-- 新增/编辑/详情弹窗 -->
    <el-dialog
      destroy-on-close
      :visible.sync="open"
      :title="title"
      width="1200px"
      append-to-body
      class="addDig"
      :close-on-click-modal="false"
      :before-close="closeDialog"
    >
      <!-- 基本信息 -->
      <collapse title="基本信息">
        <el-form
          ref="form"
          :validate-on-rule-change="false"
          :model="form"
          :rules="isEdit ? rules : {}"
          label-width="100px"
        >
          <div class="add-form">
            <el-form-item label="处置单号" prop="orderNo">
              <el-input
                v-model.trim="form.orderNo"
                size="small"
                disabled
                maxlength="12"
                placeholder="系统自动生成"
                class="input-width"
              />
            </el-form-item>
            <el-form-item label="处置类型" prop="type">
              <el-select
                v-if="isEdit"
                v-model="form.type"
                class="input-width"
                placeholder="请选择"
                clearable
                :disabled="isDisabled"
                size="small"
              >
                <el-option
                  v-for="dict in dict.type.asset_disposal_type"
                  :key="dict.value"
                  :label="dict.label"
                  :value="dict.value"
                />
              </el-select>
              <span v-else class="disposal-type">{{ matchAssetDisposalType(form.type) }}</span>
            </el-form-item>
            <el-form-item label="处置日期" prop="createTime">
              <el-date-picker v-model="form.createTime" type="date" size="small" :class="{'noIcon2':isDisabled}" disabled value-format="yyyy-MM-dd" :placeholder="addTimeNow" class="input-width" />
            </el-form-item>
            <el-form-item label="申请人" prop="applicantName">
              <el-input
                v-model.trim="form.applicantName"
                size="small"
                disabled
                maxlength="12"
                placeholder="系统自动生成"
                class="input-width"
              />
            </el-form-item>
          </div>
          <el-form-item v-if="isDisabled && form.type === '4'" key="bizNo" label="盘点单号">
            <el-input
              v-model.trim="form.bizNo"
              size="small"
              disabled
              maxlength="12"
              placeholder="系统自动生成"
              class="input-width"
            />
          </el-form-item>
          <el-form-item label="处置说明" prop="note">
            <el-input
              v-model.trim="form.note"
              type="textarea"
              :placeholder="isEdit?'请输入':''"
              :rows="2"
              :disabled="isDisabled"
              maxlength="50"
              show-word-limit
              class="input-width"
            />
          </el-form-item>
        </el-form>
      </collapse>
      <!-- 明细信息 -->
      <collapse title="资产明细">
        <div>
          <div class="addOrRemove">
            <el-button v-if="isEdit" size="small" @click="addDetail">
              <i class="iconfont icon-zengjia" style="cursor: pointer" />
              选择资产
            </el-button>
          </div>
          <el-form ref="tableForm" :model="tableFormData" :rules="tableFormRules">
            <el-table
              ref="detailTable"
              style="width: 100%"
              border
              :data="tableFormData.list"
              highlight-current-row
            >
              <el-table-column
                type="index"
                label="序号"
                width="50"
                align="center"
              />
              <el-table-column
                prop="internalCode"
                label="资产编码"
                align="center"
              />
              <el-table-column
                prop="name"
                label="资产名称"
                align="center"
              />
              <el-table-column
                prop="price"
                label="购置单价（元）"
                align="center"
                width="120"
              />
              <el-table-column
                prop="unit"
                label="计量单位"
                align="center"
              />
              <el-table-column
                prop="purchaseDate"
                label="购置日期"
                align="center"
              >
                <template slot-scope="scope">
                  <span>{{ formatPurchaseDate(scope.row.purchaseDate) }}</span>
                </template>
              </el-table-column>
              <el-table-column
                prop="price"
                label="资产原值"
                align="center"
              />
              <el-table-column
                prop="accumulatedDepreciation"
                label="累计折旧"
                align="center"
              >
                <template slot-scope="scope">
                  <span>{{ accumulatedDepreciation(scope.row) }}</span>
                </template>
              </el-table-column>
              <el-table-column
                prop="netResidualValue"
                label="资产净值"
                align="center"
              />
              <el-table-column prop="income" align="center">
                <template slot="header">
                  <span>处置收入(元)</span>
                  <span v-if="isEdit" class="star">*</span>
                </template>
                <template slot-scope="scope">
                  <el-form-item
                    :prop="'list.' + scope.$index + '.income'"
                    :rules="isEdit?tableFormRules.income:{}"
                    class="qty-input"
                  >
                    <el-input
                      v-if="isEdit"
                      v-model.trim="scope.row.income"
                      oninput="value=value.replace(/[^\d]/g,'')"
                      maxlength="6"
                    />
                    <span v-else>{{ scope.row.income }}</span>
                  </el-form-item>
                </template>
              </el-table-column>
              <el-table-column
                v-if="isEdit"
                label="操作"
                width="100"
              >
                <template slot-scope="scope">
                  <el-button
                    type="text"
                    size="small"
                    @click.native.prevent="deleteRow(scope.$index, tableData)"
                  >删除</el-button>
                </template>
              </el-table-column>
            </el-table>
          </el-form>
          <div class="totalContent">
            <span class="span1">处置总额(元)：  {{ totalDisposal }}</span>
            <span class="span2">处置收入(元)：  {{ totalIncome }}</span>
          </div>
        </div>
      </collapse>
      <collapse title="相关附件">
        <attachment-upload v-model="form.fileList" :readonly="!isEdit" />
      </collapse>
      <!-- 操作日志 新增/编辑时不显示-->
      <collapse v-if="!isEdit && form.status !==0" title="审批流程">
        <ApprovalProcess :id="form.id" ref="approvalProcess" type="type_asset_handle" />
      </collapse>
      <!-- 紧急状态 -->
      <div class="statusContent">
        <span class="spanStatus">紧急状态:</span>
        <el-radio-group v-if="isEdit" v-model="form.urgency">
          <el-radio :label="0">一般</el-radio>
          <el-radio :label="1">紧急</el-radio>
        </el-radio-group>
        <span v-else>{{
          form.urgency === 0 ? "一般" : "紧急"
        }}</span>
      </div>
      <!-- footer -->
      <div slot="footer" class="dialog-footer">
        <el-button size="small" @click="closeDialog">{{ isEdit?'取消':'关闭' }}</el-button>
        <el-button
          v-if="isEdit"
          v-debounce
          size="small"
          @click.native="save(0)"
        >存草稿</el-button>
        <el-button
          v-if="isEdit"
          v-debounce
          size="small"
          type="primary"
          @click.native="save(1)"
        >保存</el-button>
      </div>
    </el-dialog>
    <!-- 选择资产 -->
    <selectDisposalAsset
      ref="selectDisposalAsset"
      :open-detail="openDetail"
      @selected="handleSelectionDetail"
      @closeDetailDig="closeDetailDig"
    />
  </div>
</template>
<script>
import collapse from '@/views/basicdata/material/component/collapse'
import ApprovalProcess from '@/views/assetmanagement/approvalProcess/approvalProcess.vue'
import AttachmentUpload from '@/components/AttachmentUpload'
import selectDisposalAsset from './selectDisposalAsset.vue'
import eventBus from '@/utils/bus.js'
import { addOrUpdateAssetHandle } from '@/api/assetmanagement/assetDisposal.js'
import { dayjsFormat, debounce } from '@/utils/index.js'
const assetDisposalType = {
  '0': '变卖',
  '1': '报废',
  '2': '捐赠',
  '4': '盘亏',
  '100': '其它'
}
export default {
  name: 'AddOrEditDialog',
  dicts: ['asset_disposal_type', 'asset_disposal_status'],
  components: { collapse, selectDisposalAsset, ApprovalProcess, AttachmentUpload },
  props: {
    open: { type: Boolean, default: false },
    title: { type: String, default: '' },
    isEdit: { type: Boolean, default: false },
    isDisabled: { type: Boolean, default: false },
    addTimeNow: { type: String, default: '' }
  },
  data() {
    return {
      // 新增弹窗表单
      form: {
        id: undefined,
        orderNo: '',
        type: '',
        status: undefined,
        applicant: '',
        applicantName: '',
        note: '',
        urgency: 0,
        fileList: []
      },
      // 表单校验
      rules: {
        type: [
          { required: true, message: '请选择类型', trigger: ['change'] }
        ]
      },
      tableFormRules: {
        income: [
          {
            required: true,
            message: '请输入处置收入',
            trigger: ['blur', 'change']
          }
        ]
      },
      // 选择资产弹窗
      openDetail: false,
      tableData: [],
      // 处置总额
      totalDisposal: undefined,
      // 处置收入
      totalIncome: 0
    }
  },
  computed: {
    tableFormData () {
      return {
        list: this.tableData
      }
    }
  },
  watch: {
    'tableData': {
      deep: true,
      immediate: true,
      handler: function (newVal, oldVal) {
        this.$nextTick(() => {
          let a = 0
          let b = 0
          newVal.forEach((e) => {
            a += Number(e.price)
            b += Number(e.income)
          })
          this.totalDisposal = a || ''
          this.totalIncome = b || 0
        })
      }
    },
    'form': {
      deep: true,
      immediate: true,
      handler: function (newVal, oldVal) {
        this.$nextTick(() => {
          if (newVal.type && newVal.type === '4') {
            this.showPanNo = true
          } else {
            this.showPanNo = false
          }
        })
      }
    }
  },
  created() {
    eventBus.$on('getAssetHandleDetail', (payload) => {
      this.form = {
        id: payload.data.id,
        orderNo: payload.data.orderNo,
        type: payload.data.type,
        createTime: payload.data.handleDate,
        status: payload.data.status,
        applicant: payload.data.applicant,
        applicantName: payload.data.applicantName,
        note: payload.data.note,
        urgency: payload.data.urgency,
        fileList: payload.data.fileList || [],
        bizNo: payload.data.bizNo
      }
      this.tableData = payload.data.handleDetailList || []
    })
  },
  beforeDestroy() {
    eventBus.$off('getAssetHandleDetail')
  },
  methods: {
    /** 选择资产明细 */
    addDetail () {
      this.openDetail = true
      this.$nextTick(() => {
        this.$refs.selectDisposalAsset.init()
      })
    },
    /** 删除明细 */
    deleteRow (index, rows) {
      rows.splice(index, 1)
    },
    /** 弹窗确认 */
    save (index) {
      this.$refs['form'].validate(valid => {
        let isSubmit = true
        if (valid) {
          if (index === 0) {
            // 存草稿
            isSubmit = false
          }
          this.form.assetHandleDetails = this.tableData.map(item => {
            return {
              assetId: item.id,
              income: item.income
            }
          })
          const data = {
            id: this.form.id,
            type: this.form.type,
            applicant: this.form.applicant,
            note: this.form.note,
            urgency: this.form.urgency,
            fileList: this.form.fileList,
            assetHandleDetails: this.form.assetHandleDetails
          }
          this.$refs['tableForm'].validate((valid) => {
            if (valid) {
              addOrUpdateAssetHandle({ isSubmit }, JSON.stringify(data)).then(
                res => {
                  if (res.code === 200) {
                    this.$emit('success', res)
                  } else {
                    this.$message.error(res.msg)
                  }
                }
              )
            }
          })
        }
      })
    },
    /** 重置表单 */
    reset () {
      this.$refs.form.resetFields()
      this.form = {
        id: undefined,
        orderNo: '',
        type: '',
        status: undefined,
        applicant: this.$store.state.user.userId,
        applicantName: this.$store.state.user.realName,
        note: '',
        urgency: 0,
        fileList: []
      }
      this.tableData = []
    },
    /** 取消 */
    closeDialog () {
      this.reset()
      this.$emit('closedialog', true)
    },
    /** 选择资产弹窗关闭 */
    closeDetailDig (payload) {
      this.openDetail = payload
    },
    /** 选中物品 */
    handleSelectionDetail (payload) {
      this.tableData = payload
    },
    /** 累计折旧计算 */
    accumulatedDepreciation(row) {
      return row.price - row.netResidualValue || 0
    },
    /** 处置类型 */
    matchAssetDisposalType(value) {
      return assetDisposalType[value]
    },
    formatPurchaseDate(value) {
      return dayjsFormat(value, 'YYYY-MM-DD')
    },
    debounce
  }
}
</script>

<style lang="scss" scoped>
.addDig {
  ::v-deep .el-dialog .el-dialog__body {
    padding-top: 0;
    max-height: 542px;
    overflow-y: auto;
    // padding-bottom: 0;
  }
  ::v-deep .el-table--border{
    border-bottom:1px solid #EBEEF5;
  }
}
.addOrRemove {
  display: flex;
  justify-content: flex-end;
  margin-bottom: 15px;
  margin-right: 10px;
}
.add-form {
  display: flex;
  // justify-content:space-around;
  // flex-wrap:wrap;
  ::v-deep .el-form-item__label {
    padding-right: 10px;
  }
}
.input-width {
  ::v-deep .el-input__inner {
    width: 180px;
  }
}
.noIcon2 {
  ::v-deep .el-input__icon.el-icon-date {
    display: none;
  }
  ::v-deep .el-input__inner {
    padding-left: 16px;
  }
}
.statusContent {
  padding: 10px;
  margin-top: 10px;
  .spanStatus {
    margin-right: 15px;
    color: #515a6e;
    font-weight: 500;
  }
}
.star {
  color: #f56c6c;
  margin-left: 10px;
}
.totalContent {
  display: flex;
  font-weight: 500;
  margin: 10px 0;
  .span1 {
    padding-left: 80px;
  }
  .span2 {
    padding-left: 200px;
  }
}
.qty-input {
  margin-bottom: 0;
  ::v-deep .el-form-item__error {
    position: relative;
  }
}
.disposal-type {
  display:inline-block;
  width:180px;
  color: #999999;
  padding-left: 15px;
	background-color: #F5F7FA;
	border-color: #E4E7ED;
	cursor: not-allowed;
}
</style>
