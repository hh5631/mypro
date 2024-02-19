<template>
  <div>
    <!-- 基本信息 -->
    <collapse title="基本信息">
      <el-form
        ref="form"
        :model="form"
        :rules="rules"
        label-width="100px"
        :hide-required-asterisk="isDetail"
      >
        <div>
          <div class="add-form">
            <el-form-item label="入库单号" label-width="120px" prop="orderNo">
              <el-input
                v-model.trim="form.orderNo"
                size="small"
                disabled
                maxlength="12"
                placeholder="系统自动生成"
                class="input-width"
              />
            </el-form-item>
            <el-form-item label="单据名称" prop="orderName">
              <el-input
                v-model.trim="form.orderName"
                size="small"
                :disabled="isDetail||(isEdit&&form.status==='2')"
                clearable
                maxlength="20"
                placeholder="请输入"
                class="input-width"
              />
            </el-form-item>
            <el-form-item
              label="负责人"
              :prop="isNew || isEdit ? 'chargePersonName' : ''"
            >
              <!-- <el-input
                v-model.trim="form.chargePersonName"
                size="small"
                :disabled="isDetail"
                clearable
                placeholder="请选择"
                class="input-width"
                @focus="showLeaderDig"
              >
                <i
                  v-if="!isDetail"
                  slot="suffix"
                  class="iconfont icon-jiaoseshezhi"
                  style="line-height: 36px; cursor: pointer"
                />
              </el-input> -->
              <div v-if="!isDetail" key="edit" class="my-select-con">
                <el-select
                  v-model="form.chargePersonName"
                  filterable
                  placeholder="请输入"
                  class="input-width"
                  @change="getChargePerson"
                >
                  <el-option
                    v-for="item in userOptions"
                    :key="item.userId"
                    :label="item.realName"
                    :value="item.userId"
                  />
                </el-select>
                <i slot="suffix" class="iconfont icon-jiaoseshezhi" style=";cursor: pointer;" @click.stop="showLeaderDig" />
              </div>
              <div v-else key="detail">
                <el-input
                  v-model.trim="form.chargePersonName"
                  size="small"
                  :disabled="isDetail"
                  clearable
                  placeholder="请选择"
                  class="input-width"
                />
              </div>
            </el-form-item>
          </div>
          <div class="add-form">
            <el-form-item label="创建入库单日期" label-width="120px" prop="createWarehousingDate">
              <el-date-picker
                ref="picker"
                v-model="form.createWarehousingDate"
                size="small"
                type="date"
                :disabled="isDetail"
                value-format="yyyy-MM-dd"
                placeholder="请选择"
                :picker-options="pickerOptions"
                :clearable="true"
                class="input-width date-input"
              />
            </el-form-item>
            <el-form-item label="入库类型" prop="origin">
              <el-select
                v-model="form.origin"
                :disabled="isDetail || isEdit"
                placeholder="请选择"
                :class="{ noIcon2: isDetail || isEdit }"
                clearable
                size="small"
                class="input-width"
                @change="originChange"
              >
                <el-option
                  v-for="dict in dict.type.warehousing_order_origin"
                  :key="dict.value"
                  :label="dict.label"
                  :value="dict.value"
                />
              </el-select>
            </el-form-item>
            <el-form-item label="关联单号" prop="bizOrderNo">
              <el-input
                v-model.trim="form.bizOrderNo"
                size="small"
                :disabled="!isNew || form.origin !== '0'"
                clearable
                placeholder="点击选择关联单"
                class="input-width"
              >
                <i
                  v-if="!isDetail&&isNew && form.origin === '0'"
                  slot="suffix"
                  class="iconfont icon-danchu1"
                  style="line-height: 36px; cursor: pointer;"
                  @click="AssociatedNum"
                />
              </el-input>
            </el-form-item>
          </div>
          <div class="add-form">
            <el-form-item label="所属单位/部门" label-width="120px" prop="dept">
              <el-select
                v-model="form.dept"
                maxlength="15"
                :class="{ noIcon2: isDetail }"
                placeholder="请选择"
                :disabled="isDetail"
                size="small"
                class="input-width"
              >
                <el-option
                  v-for="dict in deptData"
                  :key="dict.deptId"
                  :label="dict.deptName"
                  :value="dict.deptId"
                />
              </el-select>
            </el-form-item>
            <el-form-item label="入库仓库" prop="warehouseId">
              <el-select
                v-if="isEdit || isNew"
                key="warehouseId"
                v-model="form.warehouseId"
                :disabled="isDetail"
                placeholder="请选择"
                clearable
                size="small"
                class="input-width"
                @change="valueChange"
              >
                <el-option
                  v-for="item in warehouseData"
                  :key="item.id"
                  :label="item.name"
                  :value="item.id"
                />
              </el-select>
              <el-input
                v-else
                v-model.trim="form.warehouseName"
                size="small"
                disabled
                class="input-width"
              />
            </el-form-item>
            <el-form-item label="仓管员" prop="warehouseKeeper">
              <el-input
                v-model.trim="form.warehouseKeeper"
                size="small"
                disabled
                class="input-width"
                maxlength="4"
                placeholder="仓库选择后自动生成"
              />
            </el-form-item>
          </div>
        </div>
      </el-form>
    </collapse>
    <!-- 付款条件 -->
    <collapse title="入库明细">
      <div>
        <div v-show="(isNew || isEdit) && isOther" class="addOrRemove">
          <el-button size="small" @click="showApplyDig">
            <i class="iconfont icon-zengjia" style="cursor: pointer" />
            添加明细</el-button>
        </div>
        <el-form ref="tableForm" :model="tableFormData" :rules="tableFormRules">
          <el-table
            ref="warehouseDetailTable"
            style="width: 100%"
            border
            :header-cell-class-name="cellClass"
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
              prop="materialCategory"
              label="物料类别"
              width="120"
              align="center"
            />
            <el-table-column
              prop="materialName"
              label="物料名称"
              width="100"
              align="center"
            />
            <el-table-column
              prop="brandName"
              label="品牌"
              width="100"
              align="center"
            />
            <el-table-column
              prop="materialStandard"
              label="规格型号"
              width="100"
              align="center"
            />
            <el-table-column
              prop="unitName"
              label="计量单位"
              width="100"
              align="center"
            />
            <el-table-column
              prop="thisQty"
              label="本次入库数"
              width="100"
              align="center"
            >
              <template slot-scope="scope">
                <el-form-item
                  v-if="isNew||isEdit"
                  key="thisQty"
                  :prop="'list.' + scope.$index + '.thisQty'"
                  :rules="!isOther?tableFormRules.thisQty:tableFormRules.thisQty1"
                  class="qty-input"
                >
                  <el-input
                    v-if="isNew&&scope.row.planQty === scope.row.qty && !isOther? true:false"
                    key="thisQtyInput"
                    v-model.number.trim="scope.row.thisQty"
                    oninput="value=value.replace(/[^\d]/g,'')"
                    maxlength="6"
                    @blur="handleChange(scope.row,scope.$index)"
                  />
                  <el-input
                    v-else
                    v-model.trim="scope.row.thisQty"
                    :disabled="scope.row.planQty === scope.row.qty"
                    oninput="value=value.replace(/[^\d]/g,'')"
                    @blur="handleChange(scope.row,scope.$index)"
                  />
                </el-form-item>
                <el-form-item v-else class="qty-input">
                  <span>{{ scope.row.thisQty }}</span>
                </el-form-item>
              </template>
            </el-table-column>
            <el-table-column
              prop="price"
              label="税前单价(元)"
              width="110"
              align="center"
            >
              <template slot-scope="scope">
                <el-form-item
                  :prop="'list.' + scope.$index + '.price'"
                  :rules="tableFormRules.price"
                  class="qty-input"
                >
                  <el-input
                    v-if="isNew||isEdit"
                    key="priceInput"
                    v-model="scope.row.price"
                    oninput="value=value.replace(/[^1-9]{0,1}(\d*(?:\.\d{0,2})?).*$/g, '$1')"
                    class="must"
                    maxlength="10"
                  />
                  <span v-else>{{ scope.row.price }}</span>
                </el-form-item>
              </template>
            </el-table-column>
            <el-table-column
              prop="taxAmount"
              label="税额(元)"
              width="100"
              align="center"
            >
              <template slot-scope="scope">
                <el-form-item
                  :prop="'list.' + scope.$index + '.taxAmount'"
                  class="qty-input"
                >
                  <el-input
                    v-if="isNew||isEdit"
                    key="taxAmountInput"
                    v-model="scope.row.taxAmount"
                    maxlength="10"
                  />
                  <span v-else>{{ scope.row.taxAmount }}</span>
                </el-form-item>
              </template>
            </el-table-column>
            <el-table-column
              prop="ratePrice"
              label="金额(元)"
              width="90"
              align="center"
            >
              <template slot-scope="scope">
                <span>{{ ratePrice(scope.row) }}</span>
              </template>
            </el-table-column>
            <el-table-column
              prop="purchaseDate"
              label="购置时间"
              width="148"
              align="center"
            >
              <template slot-scope="scope">
                <el-form-item
                  :prop="'list.' + scope.$index + '.purchaseDate'"
                  :rules="tableFormRules.purchaseDate"
                  class="qty-input"
                >
                  <el-date-picker
                    v-if="isNew||isEdit"
                    ref="picker"
                    v-model="scope.row.purchaseDate"
                    size="small"
                    type="date"
                    :disabled="isDetail"
                    value-format="yyyy-MM-dd"
                    placeholder="请选择"
                    :clearable="true"
                    class="picker-input"
                  />
                  <span v-else>{{ scope.row.purchaseDate }}</span>
                </el-form-item>
              </template>
            </el-table-column>
            <el-table-column
              v-if="!isOther"
              key="planQty"
              prop="planQty"
              label="计划入库数"
              width="120"
              align="center"
            />
            <el-table-column v-if="!isNew&&!isOther" key="qty" prop="qty" label="已入库数" width="120" align="center" />
            <el-table-column
              prop="remark"
              label="备注"
              align="center"
            >
              <template slot-scope="scope">
                <el-form-item
                  class="qty-input"
                >
                  <el-input
                    v-if="isNew||isEdit"
                    key="remark"
                    v-model="scope.row.remark"
                    type="textarea"
                    :rows="2"
                  />
                  <span v-else>{{ scope.row.remark }}</span>
                </el-form-item>
              </template>
            </el-table-column>
            <el-table-column
              v-if="isNew"
              fixed="right"
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
      </div>
    </collapse>
    <collapse v-if="!isSure && isDetail" title="相关附件">
      <attachment-upload v-model="form.fileList" :readonly="true" />
    </collapse>
    <!-- 相关附件新增/编辑时显示-->
    <collapse v-if="isNew || isEdit || isSure" title="相关附件">
      <attachment-upload v-model="form.fileList" />
    </collapse>
    <!-- 操作日志 新增/编辑时不显示-->
    <collapse v-if="showLog" title="操作日志">
      <div>
        <el-table
          ref="operateTable"
          style="width: 100%"
          border
          :data="operateTableData"
          highlight-current-row
          max-height="180"
        >
          <el-table-column
            type="index"
            fixed
            label="序号"
            width="50"
            align="center"
          />
          <el-table-column
            prop="operator"
            label="操作人"
            width="150"
            align="center"
          />
          <el-table-column prop="operation" label="操作描述" align="center" />
          <el-table-column
            prop="operationTime"
            label="操作时间"
            width="250"
            align="center"
          />
        </el-table>
      </div>
    </collapse>
    <!-- 选择负责人 -->
    <el-dialog
      title="选择负责人"
      :visible.sync="selectLeaderDig"
      width="800px"
      append-to-body
      destroy-on-close
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
    <!-- 关联单据 -->
    <el-dialog
      title="选择业务"
      :visible.sync="open"
      width="1000px"
      append-to-body
      :close-on-click-modal="false"
      @close="closeDialog"
    >
      <addTable ref="addTable" />
      <div slot="footer" class="dialog-footer">
        <el-button size="small" @click="cancel">取 消</el-button>
        <el-button
          size="small"
          type="primary"
          @click="submitAddTableForm"
        >保 存</el-button>
      </div>
    </el-dialog>
    <!-- 选择明细 -->
    <el-dialog
      title="添加明细"
      :visible.sync="selectDetailDig"
      width="1200px"
      append-to-body
      :close-on-click-modal="false"
      class="selectPlan"
      @close="closeDetailDig"
    ><selectDetail ref="selectDetail" />
      <div slot="footer" class="dialog-footer">
        <!-- <el-button size="mini" style="float:left" @click="addApply"><i class="iconfont icon-zengjia" />新增供应商</el-button> -->
        <el-button size="small" @click="closeDetailDig">取消</el-button>
        <el-button
          size="small"
          type="primary"
          @click="submitDetail"
        >保存</el-button>
      </div></el-dialog>
  </div>
</template>

<script>
import collapse from '@/views/basicdata/material/component/collapse'
import addTable from '@/views/warehouse/warehousing/component/addTable'
import selectLeader from '@/views/purchase/apply/component/selectLeader.vue'
import { addWarehousingOrder, confirmWarehousingOrder, updateWarehousingOrder, getLog } from '@/api/warehouse/index'
import { getWarehouse } from '@/api/basicdata/warehouse'
import selectDetail from './selectDetail.vue'
import { listDeptTwoLevel } from '@/api/system/dept'
import AttachmentUpload from '@/components/AttachmentUpload'
export default {
  name: 'AddWarehouseOrder',
  dicts: ['warehouse_classification', 'warehousing_order_origin'],
  components: { collapse, addTable, selectLeader, selectDetail, AttachmentUpload },
  props: {
    formData: {
      type: Object,
      default: () => {}
    },
    tableData: {
      type: Array,
      default: () => []
    },
    warehouseData: {
      type: Array,
      default: () => []
    },
    isDetail: {
      type: Boolean// 详情
    },
    isEdit: {
      type: Boolean// 编辑
    },
    isNew: {
      type: Boolean// 新增
    },
    isSure: {
      type: Boolean// 确认入库
    },
    showLog: {
      type: Boolean
    },
    userData: {
      type: Array,
      default: () => []
    }
  },
  data () {
    const validateQty = (rule, value, callback) => {
      const index = rule.field.split('.')[1]
      if (isNaN(Number(value))) {
        callback('请输入数字值')
        return
      }
      if (Number(value) === 0 && this.tableData[index].qty < this.tableData[index].planQty) {
        callback('请输入大于0的数')
        return
      }
      callback()
    }
    const validateQty1 = (rule, value, callback) => {
      if (isNaN(Number(value))) {
        callback('请输入数字值')
        return
      }
      if (Number(value) === 0) {
        callback('请输入大于0的数')
        return
      }
      callback()
    }
    const validatePrice = (rule, value, callback) => {
      if (isNaN(Number(value))) {
        callback('请输入数字值')
        return
      } else {
        callback()
      }
    }
    return {
      selectLeaderDig: false,
      selectDetailDig: false,
      deptData: [],
      // 表单参数
      form: {
        chargePerson: this.$store.state.user.userId,
        chargePersonName: this.$store.state.user.realName,
        bizOrderNo: undefined,
        createBy: undefined,
        dept: undefined,
        orderName: undefined,
        orderNo: undefined,
        origin: undefined,
        receiver: undefined,
        type: undefined,
        warehouseId: undefined,
        warehouseKeeper: undefined,
        warehousingTime: undefined,
        bizOrderId: '',
        createWarehousingDate: '',
        fileList: [],
        thisQty: undefined
      },
      // 表单校验
      rules: {
        orderName: [
          { required: true, message: '请填写单据名称', trigger: ['blur', 'change'] }
        ],
        dept: [
          { required: true, message: '请选择所属单位/部门', trigger: ['blur', 'change'] }
        ],
        chargePersonName: [
          { required: true, message: '请选择负责人', trigger: ['blur', 'change'] }
        ],
        origin: [
          { required: true, message: '请选择入库类型', trigger: ['blur', 'change'] }
        ],
        warehouseId: [
          { required: true, message: '请选择仓库', trigger: ['blur', 'change'] }
        ],
        createWarehousingDate: [
          { required: true, message: '请选择创建时间', trigger: ['blur', 'change'] }
        ]
      },
      open: false,
      // 操作日志
      operateTableData: [],
      operateParams: {
        bizOrderId: '',
        bizType: 'type_warehousing_order',
        pageNum: 1,
        pageSize: 10
      },
      addData: [],
      // 判断是否为其他入库
      isOther: false,
      tableFormRules: {
        price: [
          {
            required: true,
            message: '请输入单价',
            trigger: ['blur', 'input'],
            validator: validatePrice
          }
        ],
        thisQty: [
          {
            required: true,
            message: '请输入数量',
            trigger: ['blur', 'input']
          },
          {
            trigger: ['blur', 'input'],
            validator: validateQty
          },
          {
            trigger: ['blur', 'input'],
            validator: (rule, value, callback) => {
              if ((Number(value) > this.temObj.planQty) && !this.isOther) {
                callback('本次入库数不能大于计划入库数')
                return
              } else if (Number(value) + this.temObj.qty > this.temObj.planQty && !this.isOther) {
                callback('总入库数不能大于计划入库数')
                return
              } else {
                callback()
              }
            }
          }
        ],
        thisQty1: [
          {
            required: true,
            message: '请输入数值',
            trigger: ['blur', 'input']
          },
          {
            trigger: ['blur', 'input'],
            validator: validateQty1
          }
        ],
        purchaseDate: [
          {
            required: true,
            message: '请选择日期',
            trigger: ['blur', 'input']
          }
        ]
      },
      temObj: {},
      pickerOptions: { // 禁用当前日期之前的日期
        disabledDate(time) {
        // Date.now()是javascript中的内置函数，它返回自1970年1月1日00:00:00 UTC以来经过的毫秒数。
          return time.getTime() >= Date.now() - 8.64e6
        }
      }
    }
  },
  computed: {
    tableFormData () {
      return {
        list: this.tableData
      }
    },
    userOptions () {
      return this.userData
    }
  },
  watch: {
    // 侦听器本质上是一个函数
    'form.origin' (newVal, oldVal) {
      if (newVal === '10') {
        this.isOther = true
      } else {
        this.isOther = false
      }
    }
  },

  created () {
    this.getListDept()
  },
  methods: {
    /** 获取部门信息 */
    getListDept () {
      listDeptTwoLevel({}).then(response => {
        if (response.data.length > 0) {
          this.deptData = response.data
        }
      })
    },
    /** 获取操作日志 */
    getLog () {
      getLog(this.operateParams).then(
        res => {
          this.operateTableData = res.data.records || []
        }
      )
    },
    /** 删除行 */
    deleteRow (index, rows) {
      rows.splice(index, 1)
    },
    /** 关联单号*/
    AssociatedNum () {
      this.open = true
      this.$nextTick(() => {
        this.$refs.addTable.getList()
      })
    },
    /** 点击编辑时赋值 */
    editWarehouse (type) {
      this.form = this.formData
      if (type && this.form.status === '2') { // bug4038部分入库状态下-编辑页面-本次入库数量不应该显示上次入库填写的数量
        this.form.detailVOS.map((e) => {
          if (e.qty === e.planQty) {
            e.thisQty = 0
          } else {
            e.thisQty = ''
          }
        })
      }
      this.operateParams.bizOrderId = this.formData.id
      this.getLog()
    },
    /** 表单提交 */
    submit () {
      this.$refs['form'].validate(valid => {
        if (valid) {
          if (!this.form.id) {
            if (this.tableData.length === 0) {
              if (this.isOther) {
                this.$message.error('请选择至少一条明细')
              } else {
                this.$message.error('请选择关联单号')
              }
              return
            }
            this.form.detailList = this.tableData.map(item => {
              if (isNaN(item.taxAmount)) {
                item.taxAmount = 0
              }
              return {
                materialId: item.materialId,
                planQty: item.planQty || 0,
                thisQty: Number(item.thisQty),
                qty: item.qty,
                taxAmount: Number(item.taxAmount).toFixed(2),
                price: item.price,
                remark: item.remark,
                purchaseDate: item.purchaseDate
              }
            })
            this.$refs['tableForm'].validate((valid) => {
              if (valid) {
                const isComfirm = this.form.detailList.some((item) => {
                  return item.thisQty > item.planQty
                })
                if (isComfirm && !this.isOther) {
                  this.$message.error('本次入库数不得大于计划入库数')
                  return false
                }
                console.log('jj', this.form)
                addWarehousingOrder(this.form).then(res => {
                  this.$emit('submitSuccess', res)
                  this.resetForm('form')
                })
              }
            })
          } else {
            const detailList = this.tableData.map(item => {
              if (item.qty === item.planQty && !this.isOther) {
                item.thisQty = 0
              }
              if (isNaN(item.taxAmount)) {
                item.taxAmount = 0
              }
              return {
                bizDetailId: item.bizDetailId,
                brandName: item.brandName,
                id: item.id,
                location: item.location,
                mainId: item.mainId,
                materialCategory: item.materialCategory,
                materialCode: item.materialCode,
                materialId: item.materialId,
                materialName: item.materialName,
                materialStandard: item.materialStandard,
                planQty: item.planQty || 0,
                qty: item.qty || 0,
                remark: item.remark,
                unitName: item.unitName,
                thisQty: Number(item.thisQty),
                taxAmount: Number(item.taxAmount).toFixed(2),
                price: item.price,
                purchaseDate: item.purchaseDate
              }
            })
            this.form.detailList = detailList
            this.$refs['tableForm'].validate((valid) => {
              if (valid) {
                const isComfirm = detailList.some((item) => {
                  return item.thisQty > item.planQty
                })
                if (isComfirm && !this.isOther) {
                  this.$message.error('本次入库数不得大于计划入库数')
                  return false
                }
                if (this.isDetail) {
                  confirmWarehousingOrder(this.form).then(res => {
                    if (res.code === 200) {
                      this.$emit('sureSuccess', res)
                    } else {
                      this.$message.error(res.msg)
                    }
                  })
                } else {
                  console.log(this.form)
                  updateWarehousingOrder(this.form).then(res => {
                    if (res.code === 200) {
                      this.$emit('warehousingSuccess', res)
                    } else {
                      this.$message.error(res.msg)
                    }
                  })
                }
              }
            })
          }
        }
      })
    },
    /** 表单重置 */
    reset () {
      this.resetForm('form')
    },
    /** 对话框关闭 */
    closeDialog () {
      this.open = false
    },
    cancel () {
      this.open = false
    },
    /** 点击确定后获取关联单号数据*/
    submitAddTableForm () {
      this.open = false
      const obj = JSON.parse(this.$refs.addTable.currentData)
      this.addData = obj.tableArr
      this.form.bizOrderNo = obj.row.orderNo
      this.form.bizOrderId = obj.row.id
      this.$emit('add', this.addData)
    },
    /** 选择负责人 */
    closeLeaderDig () {
      this.selectLeaderDig = false
    },
    getCheckUser (items) {
      this.form.chargePersonName = items[0].realName
      this.form.chargePerson = items[0].userId
    },
    showLeaderDig () {
      this.selectLeaderDig = true
      this.$nextTick(() => {
        this.$refs.selectLeader.$refs.rowRef.clearSelection()
      })
    },
    clearSelected () {
      this.$refs.selectLeader.$refs.rowRef.clearSelection()
    },
    submitCheckLeader () {
      this.selectLeaderDig = false
    },
    /** 本次入库数校验 */
    qtyCheck () {
      this.tableData.forEach(item => {
        if (item.qty < 0) {
          this.$message.error('入库数量必须为正数')
          return false
        }
      })
    },
    showApplyDig () {
      this.selectDetailDig = true
    },
    closeDetailDig () {
      this.selectDetailDig = false
      this.$refs.selectDetail.$refs.table.clearSelection()
    },
    // 提交选择的物料明细
    submitDetail () {
      this.selectDetailDig = false
      const data = this.$refs.selectDetail.selectedData
      this.$emit('addDetail', data)
    },
    originChange (value) {
      if (value === '10') {
        this.isOther = true
      } else {
        this.isOther = false
      }
      this.$emit('addDetail', [])
      this.form.bizOrderId = ''
      this.form.bizOrderNo = ''
    },
    /** 自动添加仓管员 */
    valueChange (value) {
      if (value) {
        getWarehouse(value).then(res => {
          this.form.warehouseKeeper = res.data.storeMan
        })
      } else {
        this.form.warehouseKeeper = ''
      }
    },
    ratePrice (row) {
      const result = Number(row.price) * Number(row.thisQty)
      if (isNaN(result)) {
        return 0
      }
      return result.toFixed(2)
    },
    /* 临时变量用于判断输入值*/
    handleChange(row, index) {
      const result = this.tableFormData.list[index]
      this.temObj = result
    },
    /** 设置列必选星号 */
    cellClass (row) {
      if (this.isNew && row.column.label === '税前单价(元)' || this.isEdit && row.column.label === '税前单价(元)') {
        return 'must'
      }
      if (this.isNew && row.column.label === '购置时间' || this.isEdit && row.column.label === '购置时间') {
        return 'must'
      }
      if ((this.isNew || this.isEdit) && row.column.label === '本次入库数') {
        return 'must'
      }
    },
    getChargePerson(data) {
      this.userOptions.forEach(item => {
        if (data === item.userId) {
          this.form.chargePerson = item.userId
          this.form.chargePersonName = item.realName
        }
      })
    }
  }
}
</script>

<style lang="scss" >
.qty-input {
  margin-bottom: 0;
  .el-form-item__error {
    position: relative;
  }
}
</style>
<style lang="scss" scoped>
.add-form {
  display: flex;
  justify-content: space-around;
  flex-wrap: wrap;
  .date-input {
    width: 200px;
  }
  ::v-deep .el-form-item__label {
    padding-right: 10px;
  }
  ::v-deep .el-input_inner {
    height: 32px;
  }
  .noIcon {
    ::v-deep .el-input__icon.el-icon-date {
      display: none;
    }
    ::v-deep .el-input__inner {
      padding-left: 16px;
    }
  }
  .noIcon2 {
    ::v-deep .el-input__icon {
      display: none;
    }
    ::v-deep .el-input__inner {
      padding-left: 16px;
    }
  }
}
.input-width {
  ::v-deep .el-input__inner {
    width: 200px;
  }
}
.addOrRemove {
  display: flex;
  justify-content: flex-end;
  margin-bottom: 15px;
  margin-right: 10px;
}
::v-deep .el-table--scrollable-x .el-table__body-wrapper {
  overflow-x: auto;
  padding-bottom: 20px;
}
.qty-input {
  margin-bottom: 0;
}
.fileTip {
  color: #b2b5b9;
  font-size: 14px;
  padding-left: 8px;
}
.fileName {
  cursor: pointer;
}
.fileName:hover {
  color: #4686ef;
}
.file-list {
  display: flex;
  justify-content: space-between;
  margin-top: 10px;
}
::v-deep .el-table th.must > .cell:after {
  content: " *";
  color: #ff1818;
}
// disabled隐藏下拉图标
::v-deep .is-disabled{
  .el-icon-arrow-up:before{
  content: "";

}
}
.my-select-con{
  .icon-jiaoseshezhi {
    display: inline-block;
    cursor: pointer;
    width: 22px;
    height: 22px;
    position: absolute;
    right: 6px;
    bottom: 0;
    top:0
  }
  ::v-deep .el-select {
    width: 100%;
    .el-input__suffix-inner {
      display: none;
    }
  }
}
.picker-input {
  width: 130px;
}
</style>
