<template>
  <div>
    <collapse title="采购明细" style="margin-top: 10px">
      <template #button>
        <el-button
          v-if="isDetail === '0'"
          class="btnClass"
          plain
          size="small"
          @click.stop="selectPlan"
        ><i
          class="iconfont icon-zengjia"
          style="color: #4686ef"
        />选择计划</el-button>
      </template>
      <div class="materialDetail">
        <!-- :span-method="objectSpanMethod" class="table-class" show-summary :summary-method="getNodeSummaries" -->
        <el-form ref="materialForm" :inline-message="true" :model="meterialData" :rules="rules">
          <el-table
            ref="table"
            style="width: 100%"
            border
            :data="meterialData.list"
            :header-cell-class-name="cellClass"
            class="my-table"
          >
            <el-table-column type="index" label="序号" width="50" />
            <el-table-column prop="orderNoArr" label="关联采购计划" width="200" align="center" />
            <!-- <el-table-column prop="detailNo" fixed="left" label="明细编号" width="120" align="center" /> -->
            <el-table-column prop="materialCategory" label="资产类别" align="center" />
            <el-table-column prop="materialName" label="资产名称" width="200" align="center" />
            <el-table-column prop="brandName" label="品牌" align="center" />
            <el-table-column prop="materialStandard" label="规格型号" width="120" align="center" />
            <el-table-column prop="unitName" label="单位" align="center" />
            <el-table-column prop="planQty" label="采购数量" align="center" />
            <!-- <el-table-column prop="purchaseNum" label="本次申购数量" width="120" align="center">
              <template slot-scope="scope">
                <div>
                  <el-form-item
                    :prop="'list.'+scope.$index+'.purchaseNum'"
                    :rules="rules.purchaseNum"
                  >
                    <el-input v-model="scope.row.purchaseNum" :disabled="isDetail==='1'" @input="getInput($event,scope.row)" />
                  </el-form-item>
                </div>
              </template>
            </el-table-column> -->
            <el-table-column prop="unitPrice" label="单价(元)" align="center" width="130">
              <template slot-scope="scope">
                <span v-if="isDetail === '1'">{{
                  scope.row.unitPrice / 100
                }}</span>
                <el-form-item
                  :prop="'list.'+scope.$index+'.unitPrice'"
                  :rules="rules.unitPrice"
                >
                  <el-input
                    v-if="isDetail === '0'"
                    v-model="scope.row.unitPrice"
                    :disabled="isDetail === '1'"
                    maxlength="12"
                    @input=" getTotalPrice(
                      scope.row.unitPrice,
                      scope.row.planQty,
                      scope.row,
                      scope.$index
                    )"
                  >
                    <template slot="append">
                      <span>元</span>
                    </template>
                  </el-input>
                </el-form-item>
                <!-- <el-input v-if="isDetail==='0'" v-model="scope.row.unitPrice" oninput="value=value.replace(/[^0-9.]/g,'')" :disabled="isDetail==='1'" @input="getInput($event,scope.row)" /> -->
              </template>
            </el-table-column>
            <el-table-column prop="totalPrice" label="小计(元)" align="center">
              <template slot-scope="scope">
                <div>
                  {{
                    scope.row.totalPrice
                  }}
                </div>
              </template>
            </el-table-column>
            <el-table-column prop="remark" label="备注" align="center">
              <!-- <template slot-scope="scope">
                <div>
                  <el-input
                    v-model="scope.row.remark"
                    type="textarea"
                    :rows="2"
                    placeholder="请输入内容"
                    :disabled="isDetail==='1'"
                  />
                </div>
              </template> -->
            </el-table-column>
            <el-table-column label="附件" width="120" align="center">
              <template slot-scope="scope">
                <div v-if="scope.row.fileList && scope.row.fileList.length">
                  <div
                    style="color: #4686ef"
                    @click="showFileDig(scope.$index, scope.row.fileList)"
                  >
                    共{{
                      scope.row.fileList && scope.row.fileList.length > 0
                        ? scope.row.fileList.length
                        : 0
                    }}项
                  </div>
                </div>
                <div v-else>暂无</div>
              </template>
            </el-table-column>
            <el-table-column
              v-if="isDetail==='0'"
              label="操作"
              width="100"
            >
              <template slot-scope="scope">
                <!-- <el-button type="text" size="small" @click="handleClick(scope.row)">编辑</el-button> -->
                <el-button
                  type="text"
                  size="small"
                  :disabled="isDetail === '1'"
                  @click="delPlan(scope.row)"
                >删除</el-button>
              </template>
            </el-table-column>
          </el-table>
        </el-form>
      </div>
      <div class="totalDiv">
        <div class="total_title">合计(元)</div>
        <div class="total_count">{{ allTotalPrice }}</div>
        <!-- <div v-show="!!allTotalPrice" class="total_unit">元</div> -->
      </div>
      <div class="totalDiv">
        <div class="total_title">
          紧急程度
          <!-- <span v-if="isDetail === '0'" class="redDot">*</span> -->
        </div>
        <div v-if="isDetail === '1'" class="total_right">
          {{ urgencyDegree === "0" ? "一般" : "紧急" }}
        </div>
        <div v-if="isDetail === '0'" class="total_right">
          <el-radio
            v-model="urgencyDegree"
            label="0"
            :disabled="isDetail === '1'"
          >一般</el-radio>
          <el-radio
            v-model="urgencyDegree"
            label="1"
            :disabled="isDetail === '1'"
          >紧急</el-radio>
        </div>
      </div>
    </collapse>
    <el-dialog
      title="选择计划"
      :visible.sync="selectPlanDig"
      width="1200px"
      append-to-body
      :close-on-click-modal="false"
      class="selectPlan"
      @close="closePlanDig"
    >
      <selectPlan
        v-if="selectPlanDig"
        ref="selectPlan"
        :is-detail="isDetail"
        :table-data="tableData"
        :selection-data="selectionData"
        :del-plans="delPlans"
      />
      <div slot="footer" class="dialog-footer">
        <el-button size="small" @click="clearSelectedPlan">清空已选</el-button>
        <el-button
          size="small"
          type="primary"
          @click="submitPlan"
        >保存</el-button>
      </div>
    </el-dialog>
    <el-dialog
      title="附件"
      :visible.sync="fileDig"
      width="800px"
      append-to-body
      :close-on-click-modal="false"
      class="selectPlan1"
      destroy-on-close
      @close="closeFileDig"
    >
      <editFile
        ref="editFile"
        :file-list="fileList"
        :is-detail="isDetail"
        @delFile="delFileList"
        @keySearch="keySearch"
      />
      <div slot="footer" class="dialog-footer">
        <el-button
          size="small"
          style="float: left"
          @click="batchDownload"
        ><i class="iconfont icon-daoru" />下载至本地</el-button>
        <el-button
          size="small"
          :disabled="isDetail === '1'"
          @click="batchDel"
        >删除</el-button>
        <el-button
          size="small"
          style="float: left"
          @click="batchDownload"
        ><i class="iconfont icon-daoru" />批量下载</el-button>
        <!-- <el-button
          size="small"
          :disabled="isDetail === '1'"
          @click="batchDel"
        >删除</el-button> -->
        <!-- <el-button
          v-if="isDetail !== '1'"
          size="small"
          type="primary"
          @click="saveFile"
        >保存</el-button> -->
      </div>
    </el-dialog>
  </div>
</template>

<script>
// 采购申请添加明细组件
import collapse from '../../plan/component/collapse.vue'
import selectPlan from './selectPlan.vue'
import editFile from './editFile.vue'
import { getPlanDetailAfterDel } from '@/api/purchase/index'
export default {
  components: { collapse, selectPlan, editFile },
  props: {
    tableDatas: {
      type: Array,
      default: null
    },
    isDetail: {
      type: String,
      default: ''
    },
    mark: {
      type: String,
      default: ''
    }
  },
  data () {
    // 验证本次申购数量和待申购数量的大小
    const validatePurchaseNum = (rule, value, callback) => {
      const index = rule.field.split('.')[1]
      if (isNaN(Number(value))) {
        callback('请输入数字值')
        return
      } if (Number(value) > this.meterialData.list[index].planQty) {
        callback('本次申购数量应不超过待采购数量')
      } else {
        callback()
      }
    }
    const validatePrice = (rule, value, callback) => {
      if (value && (isNaN(Number(value)) || Number(value) === 0)) {
        callback('请输入数字值')
        return
      } else {
        callback()
      }
    }
    return {
      delIds: [],
      delPlans: [],
      radio: '',
      selectPlanDig: false,
      fileDig: false,
      tableData: [],
      selectionData: [],
      urgencyDegree: '0', // 紧急程度；0一般1紧急
      allTotalPrice: undefined,
      fileList: [],
      index: undefined, // 物料选中行的索引,
      rules: {
        purchaseNum: [
          {
            required: true,
            message: '本次申购数量应不超过待采购数量',
            trigger: ['blur', 'input'],
            validator: validatePurchaseNum
          }
        ],
        unitPrice: [{
          required: true,
          message: '请输入单价',
          trigger: ['blur', 'change']
        },
        { validator: validatePrice, message: '请输入大于0的数字', trigger: ['blur', 'change']
        }
        ]
      }
    }
  },
  computed: {
    meterialData () {
      return {
        list: this.tableData
      }
    }
  },
  watch: {
    tableDatas (val) {
      // console.log(val, 'tableDatas')
      const obj = {}
      const beforeData = []
      val.map((e) => {
        // 获取数据
        obj.id = e.id
        obj.mainId = e.mainId
        obj.materialId = e.materialId
        obj.orderNoArray = e.relationPlanNoList
        obj.orderNoArr = e.relationPlanNoList.join(',')
        obj.idArr = e.orderRelationIds
        obj.detailNo = e.detailNo
        obj.materialCategory = e.material.categoryNameLevel4 || e.material.categoryNameLevel3 || e.material.categoryNameLevel2 || e.material.categoryNameLevel1 || '无'
        // obj.materialCategory = e.material.categoryNameLevel
        obj.materialName = e.material.name
        obj.brandName = e.material.brand
        obj.materialStandard = e.material.standard
        obj.unitName = e.material.unit
        obj.planQty = e.purchaseQty
        obj.totalPrice = e.totalPrice
        if (this.isDetail === '1') {
          obj.unitPrice = e.unitPrice
          obj.totalPrice = e.totalPrice / 100
        } else {
          obj.unitPrice = e.unitPrice / 100
        }
        if (this.isDetail === '0' && this.mark === '2') {
          obj.totalPrice = e.totalPrice / 100
        }
        obj.remark = e.remark
        obj.fileList = e.fileList
        beforeData.push(JSON.parse(JSON.stringify(obj)))
      })
      this.tableData = JSON.parse(JSON.stringify(beforeData))
    },
    'tableData': {
      deep: true,
      immediate: true,
      handler: function (newVal, oldVal) {
        this.$nextTick(() => {
          let num = 0
          newVal.map((e) => {
            console.log(e.totalPrice)
            num += Number(e.totalPrice)
          })
          this.allTotalPrice = num || ''
        })
      }
    }
  },
  methods: {
    // 计算小计
    getTotalPrice (price, num, row, index) {
      // console.log(price, 'price')
      if (price) {
        // 详情
        if (this.isDetail === '1') {
          this.tableData[index].totalPrice = (price * num).toFixed(2) / 100 || ''
        }
        // 新增，编辑
        this.tableData[index].totalPrice = (price * num).toFixed(2)
      } else {
        // console.log(this.tableData[index].totalPrice, this.allTotalPrice, '88888')
        this.tableData[index].totalPrice = 0
        this.allTotalPrice = 0
      }
    },

    selectPlan () {
      this.selectPlanDig = true
      this.$nextTick(() => {
        // this.$refs.selectPlan.getList()
        this.$refs.selectPlan.getSelectId()
      })
    },
    closePlanDig () {
      this.selectPlanDig = false
    },
    showFileDig (index, fileList) {
      this.index = index
      this.fileDig = true
      this.fileList = fileList.concat()
    },
    closeFileDig () {
      this.fileDig = false
    },
    clearSelectedPlan () {
      this.$refs.selectPlan.$refs.assetTable.clearSelection()
    },
    submitPlan () {
      const myArr = this.$refs.selectPlan.selectedRow.concat(this.tableData)
      // 资产类别选择最后一个
      for (const item of myArr) {
        if (item.materialCategory) {
          const res = item.materialCategory.lastIndexOf('/')
          item.materialCategory = item.materialCategory.substring(res + 1, item.materialCategory.length)
        }
      }
      const selection = this.combineMaterial(myArr)
      // const tableArr = selection.map((e) => {
      //   const obj = {
      //     brandName: e.brandName,
      //     detailNo: e.detailNo,
      //     materialCategory: e.materialCategory,
      //     materialName: e.materialName,
      //     materialStandard: e.materialStandard,
      //     orderNoArr: e.orderNoArr,
      //     idArr: e.idArr,
      //     planQty: e.planQty,
      //     remark: e.remark,
      //     totalPrice: e.totalPrice,
      //     unitName: e.unitName,
      //     unitPrice: e.unitPrice,
      //     materialId: e.materialId,
      //     fileList: e.fileList
      //   }
      //   return JSON.parse(JSON.stringify(obj))
      // })
      this.selectPlanDig = false
      this.tableData = selection
      this.selectionData = selection
    },
    // 合并相同物料
    combineMaterial (beforeData) {
      const myArr = [...beforeData]
      const tempArr = []
      const afterData = []
      myArr.forEach((e, i, arr) => {
        if (tempArr.indexOf(e.materialId) === -1) {
          e.orderNoArray = e.orderNoArray || [e.orderNo]
          e.idArr = e.idArr || [e.id]
          afterData.push(e)
          tempArr.push(e.materialId)
        } else {
          afterData.forEach((el, j, arr) => {
            if (el.materialId === e.materialId) {
              if (e.orderNo) {
                el.orderNoArray.push(e.orderNo)
              }
              el.idArr.push(e.id)
              el.planQty += e.planQty
            }
          })
        }
      })

      afterData.map(e => {
        e.orderNoArr = e.orderNoArray ? e.orderNoArray.join(',') : ''
        // this.$set(e, 'unitPrice')
        // this.$set(e, 'totalPrice')
      })
      return [...afterData]
    },
    // 合并数组元素
    mergeArr (arr) {
      const tempArr = []
      const afterData = []
      arr.forEach((e, i, arr) => {
        if (tempArr.indexOf(e.materialId) === -1) {
          afterData.push(e)
          tempArr.push(e.materialId)
        } else {
          afterData.forEach((el, j, arr) => {
            if (el.materialId === e.materialId && el.id !== e.id) {
              el.orderNoArr = Array.from(new Set(el.orderNoArr.concat(e.orderNoArr)))
              el.planQty += e.planQty
            }
          })
        }
      })
      return afterData
    },

    delPlan (row) {
      // console.log('this.tableDatas', row.materialId, this.tableData, this.tableDatas)
      const index = this.tableData.findIndex((e) => { return e.materialId === row.materialId })
      this.tableData.splice(index, 1)
      // console.log('row===', row)
      const ids = this.selectionData.map((e) => {
        if (e.materialId === row.materialId) {
          return e.id
        }
      })
      // console.log(ids, 'ids')
      this.selectionData = this.selectionData.filter((e) => { return !ids.includes(e.id) })
      // 判断是否由父组件传过来的id，是父组件传过来的id就push进要删除的delIds
      this.tableDatas.forEach((e) => {
        if (e.materialId === row.materialId) {
          this.delIds.push(e.id)
        }
      })
      if (this.isDetail === '0' && this.tableDatas.findIndex(item => item.id === row.id) > -1) {
        this.requestDelPlan(row.idArr)
      }
    },
    // 删除后请求对应的plan
    async requestDelPlan(ids) {
      try {
        const res = await getPlanDetailAfterDel(ids)
        if (res.code === 200 && res.data) {
          res.data.forEach(dataItem => {
            const fi = this.delPlans.findIndex(delPlanItem => delPlanItem.id === dataItem.id)
            if (fi === -1) {
              this.delPlans.push(
                { ...dataItem }
              )
            }
          })
        } else {
          this.$message(res.msg)
        }
      } catch (error) {
        console.error(error)
      }
    },
    getInput (value, name, row) {
      row.totalPrice = (row.unitPrice * row.planQty).toFixed(2)
    },
    batchDownload () {
      this.$refs.editFile.batchDownload()
    },
    // 批量删除
    batchDel () {
      const arr = this.$refs.editFile.selectedFiles
      if (arr.length === 0) {
        return this.$message.error('请先选择文件')
      }
      arr.forEach(e => {
        this.delFileList(e)
      })
    },
    delFileList (row) {
      const index = this.fileList.findIndex(e => {
        return e.id === row.id
      })

      this.fileList.splice(index, 1)
    },
    // 保存文件
    saveFile () {
      this.tableData[this.index].fileList = this.fileList
      this.fileDig = false
    },
    // 文件列表关键字搜索
    keySearch (name) {
      const result = this.tableData[this.index].fileList
      if (name === '') {
        this.fileList = result
      } else {
        const lastResult = result.filter(item => item.name.toString().indexOf(name) >= 0)
        this.fileList = lastResult
      }
    },
    cellClass (row) {
      if (row.columnIndex === 8 && this.isDetail === '0') {
        return 'must'
      }
    }
  }
}
</script>

<style lang="scss" scoped>
::v-deep .el-input-group__append {
  background-color: #fff;
  padding: 0 5px;
}
.my-table{
  .el-form-item{
    margin-bottom: 0px;
  }
}
</style>
<style lang="scss" scoped>
.totalDiv {
  width: 100%;
  border: 1px solid #ddd;
  border-top: none;
  height: 53px;
  display: flex;
  font-size: 14px;
  .total_title {
    width: 170px;
    background: #f1f2f5;
    height: 100%;
    text-align: center;
    line-height: 53px;
    border-right: 1px solid #ddd;
  }
  .total_unit {
    height: 100%;
    text-align: center;
    line-height: 53px;
    padding: 0 10px;
  }
  .total_count,
  .total_right {
    height: 100%;
    text-align: center;
    line-height: 53px;
    padding-left: 20px;
  }
}
.selectPlan {
  ::v-deep .el-dialog__body {
    padding: 0;
    max-height: 542px;
    overflow-y: auto;
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
.icon-daochu {
  color: #4686ef;
  font-size: 24px;
}
.redDot {
  color: red;
}
::v-deep .el-table th.must > .cell:after {
  content: " *";
  color: #ff1818;
}
</style>
