<template>
  <div>
    <!-- 基本信息 -->
    <collapse title="基本信息">
      <el-form
        ref="form"
        :model="form"
        :rules="isEdit === '0' ? rules : {}"
        label-width="100px"
        label-position="top"
      >
        <div>
          <div class="add-form">
            <el-form-item label="出库单号" prop="orderNo">
              <el-input
                v-model.trim="form.orderNo"
                disabled
                maxlength="15"
                size="small"
                placeholder="系统自动生成"
                class="input-width"
              />
            </el-form-item>
            <el-form-item label="单据名称" prop="orderName">
              <el-input
                v-model="form.orderName"
                maxlength="11"
                size="small"
                placeholder="请输入"
                class="input-width"
                :disabled="isEdit !== '0'"
              />
            </el-form-item>
            <el-form-item class="outType" label="出库类型" prop="outType">
              <el-select
                v-model="form.outType"
                :class="{ noIcon: isEdit === '2' }"
                clearable
                size="small"
                class="input-width"
                :disabled="true"
              >
                <el-option
                  v-for="dict in dict.type.outbound_order_origin"
                  :key="dict.value"
                  :label="dict.label"
                  :value="dict.value"
                />
              </el-select>
            </el-form-item>
            <el-form-item label="关联单号" prop="bizNo">
              <el-input
                v-model.trim="form.bizNo"
                placeholder="自动导入"
                size="small"
                class="input-width"
                disabled
              >
                <!-- <i
                  v-if="isEdit == 0"
                  slot="suffix"
                  class="iconfont icon-danchu1"
                  style="line-height: 36px; cursor: pointer"
                /> -->
              </el-input>
            </el-form-item>
            <el-form-item label="申领人" prop="peopleName">
              <div v-if="isEdit !== '2'" key="edit" class="my-select-con">
                <el-select
                  v-model="form.peopleName"
                  filterable
                  placeholder="请输入"
                  class="input-width"
                  :disabled="isEdit !== '0'"
                  @change="getPeopleId"
                >
                  <el-option
                    v-for="item in userOptions"
                    :key="item.userId"
                    :label="item.realName"
                    :value="item.userId"
                  />
                </el-select>
                <i v-if="isEdit === '0'" slot="suffix" class="iconfont icon-renyuanxuanze" style=";cursor: pointer;" @click.stop="showLeaderDig" />
              </div>
              <div v-else key="detail">
                <el-input
                  v-model="form.peopleName"
                  placeholder="请选择"
                  disabled
                  style="width: 200px;"
                />
              </div>
            </el-form-item>
            <el-form-item label="申领部门" prop="dept">
              <el-select
                v-if="isEdit==='0'"
                v-model="form.dept"
                placeholder="默认账号所属部门"
                clearable
                size="small"
                class="input-width"
                :disabled="isEdit !== '0'"
              >
                <el-option
                  v-for="dict in deptData"
                  :key="dict.deptId"
                  :label="dict.deptName"
                  :value="dict.deptId"
                />
              </el-select>
              <el-input
                v-else
                v-model="form.deptName"
                placeholder="默认账号所属部门"
                clearable
                size="small"
                class="input-width"
                :disabled="isEdit !== '0'"
              />
            </el-form-item>
            <el-form-item label="接收单位" prop="companyName">
              <el-input
                v-model="form.companyName"
                placeholder="默认账号所属单位"
                clearable
                size="small"
                class="input-width"
                :disabled="isEdit !== '0'"
              />
              <!-- <el-select v-model="form.company" placeholder="默认账号所属单位" clearable size="small" class="input-width" :disabled="isEdit!=='0'">
                <el-option
                  v-for="dict in deptData"
                  :key="dict.deptId"
                  :label="dict.deptName"
                  :value="dict.deptId"
                />
              </el-select> -->
            </el-form-item>
            <!-- <el-form-item v-if="isEdit==='2'||isEdit==='1'" label="申领时间" prop="warehousingTime" class="final">
              <el-input v-model.number="form.warehousingTime" disabled placeholder="系统自动生成" />
            </el-form-item> -->
            <!-- 增加存放位置选择 -->
            <el-form-item label="存放位置" prop="placeName">
              <el-select
                v-if="isEdit==='0'"
                ref="selectUpResId"
                v-model="form.placeName"
                placeholder="请选择"
                clearable
                :disabled="isEdit !== '0'"
                size="small"
                class="input-width"
              >
                <el-option
                  :key="form.id"
                  hidden
                  :label="form.label"
                  :value="form.id"
                />
                <el-tree
                  ref="treeSelect"
                  class="filter-tree"
                  :data="placeData"
                  :props="defaultProps"
                  node-key="id"
                  :expand-on-click-node="false"
                  :check-on-click-node="true"
                  :default-expanded-keys="expandedArr"
                  @node-click="handleNodeClick"
                />
              </el-select>
              <el-input v-else v-model="form.placeName" :disabled="isEdit !== '0'" class="input-width" />
            </el-form-item>
          </div>
        </div>
      </el-form>
    </collapse>
    <!-- 付款条件 -->
    <collapse title="资产明细">
      <div>
        <div v-if="isEdit === '0'" class="addOrRemove">
          <el-button size="small" @click="showApplyDig">
            <i class="iconfont icon-zengjia" style="cursor: pointer" />
            添加明细</el-button>
          <el-button size="small" @click="removeDetail">
            <i class="iconfont icon-shanchu" style="cursor: pointer" />
            删除</el-button>
        </div>
        <el-form ref="tableForm" :model="tableFormData" :rules="tableFormRules">

          <el-table
            v-if="open"
            ref="warehouseDetailTable"
            style="width: 100%"
            border
            :data="tableFormData.list"
            highlight-current-row
            max-height="200"
            :header-cell-class-name="cellClass"
            @selection-change="handleSelectionChange"
          >
            <el-table-column v-if="isEdit === '0'" type="selection" width="55" />
            <el-table-column type="index" fixed label="序号" width="50" />
            <!-- <el-table-column prop="type" label="资产类别" width="120" align="center" /> -->
            <!-- <el-table-column prop="materialCode" label="资产编号" width="120" align="center" /> -->
            <el-table-column
              prop="categoryNameLevel"
              label="资产类别"
              align="center"
            />
            <el-table-column
              prop="materialName"
              label="资产名称"
              align="center"
            />
            <el-table-column
              prop="brandName"
              label="品牌"
              width="120"
              align="center"
            />
            <el-table-column
              prop="standard"
              label="规格型号"
              width="180"
              align="center"
            />
            <el-table-column
              prop="unitName"
              label="计量单位"
              width="120"
              align="center"
            />
            <el-table-column
              prop="warehouseName"
              label="出库仓库"
              width="120"
              align="center"
            >
              <!-- <template slot-scope="scope">
                <div>
                  <el-select v-model="scope.row.location" placeholder="请选择" size="small" class="input-class" :disabled="isEdit!=='0'">
                    <el-option
                      v-for="dict in warehouseData"
                      :key="dict.id"
                      :label="dict.name"
                      :value="dict.id"
                    />
                  </el-select>
                </div>
              </template> -->
            </el-table-column>
            <el-table-column
              prop="qty"
              label="出库数量"
              width="120"
              align="center"
            >
              <template slot-scope="scope">
                <el-form-item
                  :prop="'list.' + scope.$index + '.qty'"
                  :rules="tableFormRules.qty"
                  class="qty-input"
                >
                  <el-input
                    v-model="scope.row.qty"
                    v-trim.exceptInt="scope.row.qty"
                    placeholder="请输入"
                    style="text-align: center;"
                    class="input-class"
                    :disabled="isEdit !== '0'"
                  />
                </el-form-item>
              </template>
            </el-table-column>
            <el-table-column
              prop="outboundDate"
              label="出库时间"
              width="220"
              align="center"
            >
              <template slot-scope="scope">
                <el-form-item
                  :prop="'list.' + scope.$index + '.outboundDate'"
                  :rules="tableFormRules.outboundDate"
                  class="qty-input"
                >
                  <el-date-picker
                    v-if="isEdit === '0'"
                    ref="picker"
                    v-model="scope.row.outboundDate"
                    size="small"
                    type="datetime"
                    :disabled="isEdit !== '0'"
                    value-format="yyyy-MM-dd hh:mm:ss"
                    placeholder="请选择"
                    :clearable="true"
                    class="picker-input"
                  />
                  <span v-else>{{ scope.row.outboundDate }}</span>
                </el-form-item>
              </template>
            </el-table-column>
            <el-table-column
              v-if="isEdit!=='1'"
              width="110"
              label="操作"
            >
              <template slot-scope="scope">
                <el-button
                  v-if="isEdit === '0'"
                  type="text"
                  size="small"
                  @click.native.prevent="deleteRow(scope.$index, detailList)"
                >删除</el-button>
                <el-button
                  v-if="isEdit === '2'"
                  type="text"
                  size="small"
                  @click.native.prevent="getDetailRow(scope.row)"
                >明细详情</el-button>
              </template>
            </el-table-column>

            <!--            <el-table-column
                          v-if="isEdit === '0'"
                          fixed="right"
                          label="操作"
                          width="100"
                        >
                          <template slot-scope="scope">
                            <el-button
                              type="text"
                              size="small"
                              @click.native.prevent="deleteRow(scope.$index, detailList)"
                            >删除</el-button>
                          </template>
                        </el-table-column>
                        <el-table-column
                          v-if="
                            (isEdit === '1' || isEdit === '2') &&
                              formData.warehousingStatus === '1'
                          "
                          fixed="right"
                          label="操作"
                          width="100"
                        >
                          <template slot-scope="scope">
                            <div>
                              <el-button
                                size="small"
                                type="text"
                                @click.native.prevent="getDetail(scope.row)"
                              >打印标签</el-button>
                            </div>
                          </template>
                          <el-button
                            v-if="isEdit === '2'"
                            type="text"
                            size="small"
                            @click.native.prevent="getDetailRow(scope.row)"
                          >明细详情</el-button>

                        </el-table-column>-->
          </el-table>
        </el-form>

      </div>
    </collapse>
    <!-- 相关附件新增/编辑时显示-->
    <collapse title="相关附件">
      <attachment-upload v-model="form.fileList" :readonly="isEdit === '2'" />
    </collapse>
    <!-- 操作日志 新增/编辑时不显示-->
    <collapse v-if="isEdit == '2'" title="操作日志">
      <div>
        <el-table
          ref="logList"
          style="width: 100%"
          border
          :data="logList"
          highlight-current-row
          max-height="200"
        >
          <el-table-column type="index" fixed label="序号" width="50" />
          <el-table-column
            prop="operator"
            label="操作人"
            width="120"
            align="center"
          />
          <el-table-column prop="operation" label="操作描述" align="center" />
          <el-table-column
            prop="operationTime"
            label="操作时间"
            width="160"
            align="center"
          />
        </el-table>
      </div>
    </collapse>
    <el-dialog
      title="选择申领人"
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
    <!-- 选择明细 -->
    <el-dialog
      v-if="selectDetailDig"
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
import { listDept } from '@/api/system/dept'
import { getCompanyInfo } from '@/api/system/user'
import { listWarehouse } from '@/api/basicdata/warehouse'
import { getAssetByOutIdAndMaterialId } from '@/api/warehouse/index'
import selectLeader from '../../../purchase/apply/component/selectLeader.vue'
import selectDetail from './selectDetail.vue'
import { printLabelPromise } from '../../../assetmanagement/labelPrint/print'
import AttachmentUpload from '@/components/AttachmentUpload'
import { initialPrintData, handlePrintData } from '@/utils/jri.js'
import { getPlaceTreeSelect } from '@/api/basicdata/index'
export default {
  name: 'AddWarehouseOrder',
  dicts: ['outbound_order_origin', 'outbound_order_status', 'warehouse_classification'],
  components: { collapse, selectLeader, selectDetail, AttachmentUpload },
  props: {
    formData: {
      type: Object,
      default: () => { }
    },
    detailList: {
      type: Array,
      default: () => []
    },
    logList: {
      type: Array,
      default: () => []
    },
    isDisabled: {
      type: Boolean,
      default: false
    },
    isEdit: {
      type: String,
      default: '0'
    },
    open: {
      type: Boolean,
      default: false
    },
    userData: {
      type: Array,
      default: () => []
    }
  },
  data () {
    const validateQty = (rule, value, callback) => {
      const index = rule.field.split('.')[1]
      const existNum = this.tableFormData.list[index].existNum
      if (value === '0') {
        callback('出库数须大于0')
        return
      } else if (Number(value) > existNum) {
        callback(`出库数量不得大于可领用数量${existNum}`)
      } else {
        callback()
      }
    }
    return {
      selectDetailDig: false,
      selectLeaderDig: false,
      multipleSelection: [], // 多选物料
      // 表单参数
      form: {
        id: undefined,
        orderNo: '',
        orderName: '',
        outType: '',
        bizNo: '',
        dept: '',
        people: '',
        fileList: [],
        company: '',
        companyName: '',
        warehousingStatus: '0',
        placeId: '',
        placeName: undefined
      },
      // 选择采购负责人暂存
      ChooseToPurchase: {
        peopleName: '',
        people: '',
        dept: '',
        company: '',
        companyName: ''
      },
      assetList: [],
      deptData: [],
      warehouseData: [],
      // 表单校验
      rules: {
        orderName: [
          { required: true, message: '请填写单据名称', trigger: ['blur', 'change'] }
        ],
        // outType: [
        //   { required: true, message: '请选择出库类型', trigger: ['blur', 'change'] }
        // ],
        peopleName: [
          { required: true, message: '请选择申领人', trigger: ['blur', 'input', 'change'] }
        ],
        dept: [
          { required: true, message: '请选择申领部门', trigger: ['blur', 'change'] }
        ],
        placeName: [
          { required: true, message: '请选择存放位置', trigger: ['blur', 'input', 'change'] }
        ]
      },
      tableFormRules: {
        qty: [
          {
            required: true,
            message: '请输入出库数量',
            trigger: ['blur', 'input']
          },
          {
            trigger: ['blur', 'input'],
            validator: validateQty
          }
        ],
        outboundDate: [
          {
            required: true,
            message: '请选择',
            trigger: ['blur', 'input']
          }
        ]
      },
      printData: initialPrintData,
      placeData: [], // 存放位置数组
      expandedArr: [],
      // 树形组件默认参数
      defaultProps: {
        children: 'children',
        label: 'label'
      }
    }
  },
  computed: {
    tableFormData () {
      return {
        list: this.detailList
      }
    },
    userOptions () {
      return this.userData
    }
  },
  created () {
    this.getListDept()
    this.getListWarehouse()
    this.getPlaceList()
    this.form.outType = 10
  },
  methods: {
    /** 删除行 */
    deleteRow (index, rows) {
      rows.splice(index, 1)
    },
    cellClass (row) {
      switch (this.isEdit) {
        case '0':
          return row.columnIndex === 8 || row.columnIndex === 9 ? 'must' : ''
        default:
          break
      }
    },
    /** 删除行 */
    removeDetail () {
      if (this.multipleSelection.length === 0) {
        this.$message.error('请至少选择一个物料')
        return
      }
      this.multipleSelection.forEach(e => {
        this.delList(e)
      })
    },
    // 删除数据
    delList (row) {
      this.$emit('delDetail', row)
    },
    showApplyDig () {
      this.selectDetailDig = true
      this.$nextTick(() => {
        // 清空搜索框
        this.$refs.selectDetail.queryParams.materialName = ''
        this.$refs.selectDetail.getList()
      })
    },
    closeDetailDig () {
      this.selectDetailDig = false
      this.$refs.selectDetail.$refs.table.clearSelection()
    },

    // 提交选择的物料明细
    submitDetail () {
      this.selectDetailDig = false
      const data = this.$refs.selectDetail.selectedData.map(item => {
        return {
          ...item, categoryNameLevel: item.categoryNameLevel4 || item.categoryNameLevel3 || item.categoryNameLevel2 || item.categoryNameLevel1
        }
      })
      this.$emit('addDetail', data)
    },
    showLeaderDig () {
      this.selectLeaderDig = true
    },
    closeLeaderDig () {
      this.selectLeaderDig = false
    },
    // 选择申领人
    getCheckUser (items) {
      this.ChooseToPurchase.peopleName = items[0].realName
      this.ChooseToPurchase.people = items[0].userId
      this.ChooseToPurchase.dept = items[0].deptId
      this.getCompanyInfoFn(items[0].userId)
    },
    // 保存申领人
    submitCheckLeader () {
      this.$set(this.form, 'peopleName', this.ChooseToPurchase.peopleName)
      this.$set(this.form, 'people', this.ChooseToPurchase.people)
      this.$set(this.form, 'dept', this.ChooseToPurchase.dept)
      this.$set(this.form, 'company', this.ChooseToPurchase.company)
      this.$set(this.form, 'companyName', this.ChooseToPurchase.companyName)
      this.selectLeaderDig = false
    },
    // 清空已选
    clearSelected () {
      this.$refs.selectLeader.$refs.rowRef.setCurrentRow()
      this.ChooseToPurchase = {
        peopleName: '',
        people: '',
        dept: '',
        company: '',
        companyName: ''
      }
    },
    handleSelectionChange (val) {
      this.multipleSelection = val
    },
    editWarehouse () {
      this.$refs['form'].resetFields()
      this.$refs.form.resetFields()
      this.$refs.form.clearValidate()
      this.form = this.formData
    },
    // 获取部门信息
    getListDept () {
      listDept({}).then(response => {
        this.deptData = response.data
      })
    },
    /* 获取仓库信息 */
    getListWarehouse () {
      listWarehouse({ pageSize: 999 }).then(response => {
        this.warehouseData = response.data.records
      })
    },
    getDetail (row) {
      const data = {
        materialCode: row.materialCode,
        outboundOrderId: this.form.id
      }
      getAssetByOutIdAndMaterialId(data).then(async response => {
        if (response.code === 200) {
          this.assetList = response.data
          this.printData.labels = handlePrintData(this.assetList)
          // 打印内容
          await printLabelPromise(this.printData)
        } else {
          this.$message.error(response.msg)
        }
      })
    },
    checkQty (row) {
      if (row.qty > row.existNum) {
        row.qty = undefined
        return this.$message.error(`出库数量不得大于可领用数量${row.existNum}`)
      }
    },
    getCompanyInfoFn (id) {
      // 查询人员单位信息
      getCompanyInfo({ id }).then(
        (res) => {
          if (res.code === 200) {
            this.ChooseToPurchase.company = res.data.id
            this.ChooseToPurchase.companyName = res.data.name
          }
        }
      )
    },
    getDetailRow(row) {
      this.$emit('getDetailRow', row)
    },
    getPeopleId(data) {
      getCompanyInfo({ id: data }).then(res => {
        this.form.companyName = res.data.name
      })
      this.userOptions.forEach(item => {
        if (data === item.userId) {
          this.form.peopleName = item.realName
          this.form.people = item.userId
          this.form.dept = item.deptId
        }
      })
    },
    /** 获取仓库列表*/
    getPlaceList () {
      getPlaceTreeSelect({}).then(response => {
        response.data.forEach(item => {
          this.expandedArr.push(item.id)
        })
        this.placeData = response.data
      })
    },
    /** 上级选择 */
    handleNodeClick(data) {
      this.$set(this.form, 'placeId', data.id)
      this.$set(this.form, 'placeName', data.label)
      this.$refs.selectUpResId.blur()
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
  flex-wrap: wrap;
  ::v-deep .el-form-item__label {
    padding: 0;
  }
  ::v-deep .el-form-item {
    margin-right: 35px;
  }
  .noIcon {
    ::v-deep .el-input__icon {
      display: none;
    }
  }
}
.input-width {
  ::v-deep .el-input__inner {
    width: 200px;
  }
}
.final {
  margin-left: 12px;
  ::v-deep .el-input__inner {
    width: 200px;
  }
  ::v-deep .el-form-item__label {
    padding: 0;
  }
}
.addOrRemove {
  display: flex;
  justify-content: flex-end;
  margin-bottom: 15px;
  margin-right: 10px;
}
.input-class {
  ::v-deep .el-input__inner {
    border: none;
    text-align:center ;
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
.fileName:hover {
  color: #4686ef;
}
.file-list {
  display: flex;
  justify-content: space-between;
  margin-top: 10px;
}
.popover-title {
  display: flex;
  justify-content: space-between;
  margin-bottom: 10px;
}
.asset-item {
  width: 296px;
  height: 88px;
  border-bottom: 1px solid #dddddd;
  padding: 12px;
  display: flex;
  align-items: center;
  justify-content: space-between;
  .item-left {
    width: 80%;
    display: flex;
    align-items: center;
    justify-content: space-between;
  }
}
.assetSelected {
  background: #f1f6fd;
}
.asset-qrcode {
  width: 56px;
  height: 56px;
}
.asset-icon {
  width: 20px;
  height: 20px;
  color: #4686ef;
}
.print {
  margin-top: 20px;
}
::v-deep .el-table th.must > .cell:after {
  content: " *";
  color: #ff1818;
}
.outType{
  ::v-deep .el-select {
    width: 100%;
    .el-input__suffix-inner {
      display: none;
    }
  }
}
.my-select-con{
  .icon-renyuanxuanze {
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
  width: 190px;
}
</style>
