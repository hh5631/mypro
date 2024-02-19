<template>
  <div>
    <div class="form-top">
      <div v-if="handleType === 1" class="top-left">
        <el-button
          :type="selectedStatus === '4' ? 'primary' : ''"
          round
          @click="getcheckDetailList('4')"
        >应盘（{{ tabData.shouldCheck || 0 }}）</el-button>
        <el-button
          :type="selectedStatus === '1' ? 'primary' : ''"
          round
          @click="getcheckDetailList('1')"
        >正常（{{ tabData.normal || 0 }}）</el-button>
        <el-button
          :type="selectedStatus === '0' ? 'primary' : ''"
          round
          @click="getcheckDetailList('0')"
        >未盘（{{ tabData.notCheck || 0 }}）</el-button>
        <el-button
          :type="selectedStatus === '2' ? 'primary' : ''"
          round
          @click="getcheckDetailList('2')"
        >盘盈（{{ tabData.checkPlus || 0 }}）</el-button>
        <el-button
          :type="selectedStatus === '3' ? 'primary' : ''"
          round
          @click="getcheckDetailList('3')"
        >盘亏（{{ tabData.checkLoss || 0 }}）</el-button>
      </div>
      <div v-if="handleType === 2" class="top-left">
        <el-button
          :type="selectedStatus === '5' ? 'primary' : ''"
          round
          @click="getcheckDetailList('5')"
        >全部（{{ tabData.all || 0 }}）</el-button>
        <el-button
          :type="selectedStatus === '2' ? 'primary' : ''"
          round
          @click="getcheckDetailList('2')"
        >盘盈（{{ tabData.checkPlus || 0 }}）</el-button>
        <el-button
          :type="selectedStatus === '3' ? 'primary' : ''"
          round
          @click="getcheckDetailList('3')"
        >盘亏（{{ tabData.checkLoss || 0 }}）</el-button>
      </div>
      <div v-if="handleType === 1" class="top-right">
        <el-button @click="handleExport">导出</el-button>
        <el-button
          type="primary"
          @click="handleProfit('add')"
        >添加盘盈资产</el-button>
      </div>
      <div v-if="handleType === 2" class="top-right">
        <el-button
          v-if="selectedStatus !== '3'"
          v-debounce
          type="primary"
          :disabled="isDisabledPlus"
          @click.stop="batchHandle(1)"
        >批量处理盘盈</el-button>
        <el-button
          v-if="selectedStatus !== '2'"
          v-debounce
          type="primary"
          :disabled="isDisabledLoss"
          @click.stop="batchHandle(2)"
        >批量处理盘亏</el-button>
      </div>
    </div>
    <div v-if="handleType === 1" class="form-process">
      <div class="process-label">盘点进度</div>
      <div style="width: 90%">
        <el-progress :percentage="tabData.progress" />
      </div>
    </div>
    <div v-if="handleType === 2" class="form-info">
      <div>
        处置方式：1、盘盈资产自动添加至台账；2、盘亏资产自动添加资产处置单；3、异常资产可选（更新台账、保修、申请处置）三种处理方式；
      </div>
    </div>
    <div v-if="handleType === 2 && handleStatus && selectedStatus === '3'">
      审批状态：
      <span v-if="handleStatus === '0'">草稿</span>
      <span v-if="handleStatus === '1'">待审批</span>
      <span v-if="handleStatus === '2'">审批中</span>
      <span v-if="handleStatus === '3'">通过</span>
      <span v-if="handleStatus === '4'">不通过</span>
    </div>
    <div class="table-css">
      <el-table
        ref="table"
        v-loading="loading"
        highlight-current-row
        :data="tableData"
        style="margin-top: 10px"
        max-height="300"
        @selection-change="handleSelectionChange"
      >
        <el-table-column type="index" label="序号" width="50" align="center" />

        <el-table-column
          label="资产编码"
          align="center"
          prop="internalCode"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          label="资产类别"
          align="center"
          prop="type"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          label="资产名称"
          align="center"
          prop="name"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          label="品牌"
          align="center"
          prop="brandName"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          label="规格型号"
          align="center"
          prop="standard"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          v-if="handleType === 1"
          key="deptName"
          label="存放部门"
          align="center"
          prop="deptName"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          v-if="handleType === 1"
          key="unitName"
          label="存放位置"
          align="center"
          prop="placeName"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          key="warehouseName"
          label="使用人"
          align="center"
          prop="userName"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          label="盘点状态"
          align="center"
          prop="checkStatus"
          :show-overflow-tooltip="true"
        >
          <template slot-scope="scope">
            <div>
              <!-- 只有四种状态 -->
              <span v-if="scope.row.checkStatus === '0'">未盘</span>
              <span v-if="scope.row.checkStatus === '1'">正常</span>
              <span v-if="scope.row.checkStatus === '2'">盘盈</span>
              <span v-if="scope.row.checkStatus === '3'">盘亏</span>
              <!-- <span v-if="scope.row.checkStatus === '4'">应盘</span> -->
            </div>
          </template>
        </el-table-column>
        <el-table-column
          v-if="handleType === 1"
          label="备注"
          align="center"
          :show-overflow-tooltip="true"
          prop="remark"
        />
        <el-table-column
          v-if="handleType === 2"
          key="processStatus"
          label="处理状态"
          align="center"
          :show-overflow-tooltip="true"
        >
          <template slot-scope="scope">
            <div>
              <span v-if="scope.row.processStatus === '0'">未处理</span>
              <span v-if="scope.row.processStatus === '1'">已处理</span>
            </div>
          </template>
        </el-table-column>
        <el-table-column
          v-if="handleType === 2"
          key="processType"
          label="处置方式"
          align="center"
          :show-overflow-tooltip="true"
        >
          <div>审批处理</div>
        </el-table-column>
        <el-table-column
          v-if="handleType === 1"
          key="handle"
          label="操作"
          align="left"
          class-name="small-padding"
          width="130"
        >
          <template slot-scope="scope">
            <div>
              <el-button
                v-if="scope.row.checkStatus !== '2'"
                :disabled="scope.row.checkStatus === '1'"
                size="small"
                type="text"
                @click="startCheck(scope.row, '1')"
              >盘</el-button>
              <el-button
                v-if="scope.row.checkStatus !== '2'"
                size="small"
                type="text"
                :disabled="scope.row.checkStatus === '3'"
                @click="showLoss(scope.row)"
              >盘亏</el-button>
              <el-button
                v-if="scope.row.checkStatus === '2'"
                size="small"
                type="text"
                @click="handleProfit('edit', scope.row)"
              >编辑</el-button>
              <el-button
                v-if="scope.row.checkStatus === '2'"
                size="small"
                type="text"
                @click="handleDelete(scope.row)"
              >删除</el-button>
            </div>
          </template>
        </el-table-column>
      </el-table>
    </div>
    <el-dialog
      title="盘亏"
      :visible.sync="showShort"
      width="1000px"
      append-to-body
      :close-on-click-modal="false"
      class="selectPlan"
    ><InventoryShortages
      ref="inventoryShortages"
      @submitForm="checkLoss"
      @cancel="showShort = false"
    />
    </el-dialog>
  </div>
</template>

<script>
import InventoryShortages from './inventoryShortages.vue'
import {
  getcheckDetailNum,
  getcheckDetailList,
  startCheck,
  delCheckDetailPlus,
  batchCheckDetailPlus,
  batchCheckDetailLoss,
  getHandleStatus
} from '@/api/assetmanagement/takeStock.js'
export default {
  name: 'AddWarehouseOrder',
  dicts: [],
  components: { InventoryShortages },
  props: {
    handleType: {
      type: Number,
      default: 1
    }
  },
  data() {
    return {
      loading: false,
      selectedStatus: '4', // 未盘0正常1盘盈2盘亏3应盘4全部5
      handleStatus: '',
      taskId: '',
      tabData: {},
      tableData: [],
      showShort: false, // 盘亏弹框
      isDisabledPlus: true,
      isDisabledLoss: true,
      checkNo: ''
    }
  },
  watch: {},
  created() {},
  methods: {
    init(row, type) {
      if (type) {
        // 传type按钮切换到最初的状态，不传表示按钮保持现有的选中状态
        this.selectedStatus = this.handleType === 1 ? '4' : '5'
      }

      this.taskId = row.id
      this.getcheckDetailNum(row.id)
      this.getcheckDetailList(this.selectedStatus)
    },
    getcheckDetailNum(id) {
      getcheckDetailNum({ taskId: id }).then((response) => {
        if (response.code === 200) {
          this.tabData = response.data
          this.tabData.progress = Number(this.tabData.progress)
          const all =
            Number(response.data.checkLoss) + Number(response.data.checkPlus)
          this.$set(this.tabData, 'all', all)
        } else {
          this.$message.error(response.msg)
        }
      })
    },
    getcheckDetailList(status) {
      this.loading = true
      this.selectedStatus = status
      const params = {
        status: status,
        taskId: this.taskId
      }
      getcheckDetailList(params).then((response) => {
        if (response.code === 200) {
          this.loading = false
          this.tableData = response.data
          // 有数据且处理状态有未处理才可以点击批量处理盘盈
          if (this.tableData.length < 0) return
          if (this.selectedStatus !== '3') {
            const hasArr = this.tableData.filter((e) => {
              return e.checkStatus === '2'
            })
            if (
              hasArr.length > 0 &&
              hasArr.some((e) => {
                return e.processStatus === '0'// 0未处理1已处理
              })
            ) {
              this.isDisabledPlus = false
            } else {
              this.isDisabledPlus = true
            }
          }
          // 有数据且审批状态未开始才可以点击批量盘亏
          if (this.selectedStatus !== '2') {
            const hasArr = this.tableData.filter((e) => {
              return e.checkStatus === '3'
            })
            if (hasArr.length > 0 && !this.handleStatus) {
              this.isDisabledLoss = false
            } else {
              this.isDisabledLoss = true
            }
          }
        } else {
          this.$message.error(response.msg)
        }
      })
    },
    getHandleStatus(checkNo) {
      this.checkNo = checkNo
      const params = {
        bizNo: this.checkNo
      }
      getHandleStatus(params).then((response) => {
        if (response.code === 200) {
          this.handleStatus = response.data
        } else {
          this.$message.error(response.msg)
        }
      })
    },
    // 盘
    startCheck(row, status) {
      const data = {
        detailId: row.id,
        status: status,
        remark: row.remark || ''
      }
      startCheck(data).then((response) => {
        if (response.code === 200) {
          this.$modal.msgSuccess('操作成功')
          this.showShort = false
          const row = { id: this.taskId }
          this.init(row)
        } else {
          this.$message.error(response.msg)
        }
      })
    },
    // 盘亏弹框
    showLoss(row) {
      this.showShort = true
      this.$nextTick(() => {
        this.$refs.inventoryShortages.assetForm = JSON.parse(
          JSON.stringify(row)
        )
      })
    },
    checkLoss(data) {
      this.startCheck(data, '3')
    },
    // 添加盘盈资产
    handleProfit(type, row) {
      if (type === 'add') {
        this.$emit('addProfit', this.taskId)
      } else {
        this.$emit('editProfit', row)
      }
    },
    // 批量处理盈亏
    batchHandle(type) {
      this.$confirm(
        type === 1 ? '确定批量处理盘盈资产?' : '确定批量处理盈亏资产?',
        '提示',
        {
          confirmButtonText: '确定',
          cancelButtonText: '取消',
          type: 'warning',
          center: true
        }
      )
        .then(() => {
          const Api = type === 1 ? batchCheckDetailPlus : batchCheckDetailLoss
          Api({ taskId: this.taskId }).then((response) => {
            if (response.code === 200) {
              this.$modal.msgSuccess('操作成功')
              if (type === 2) {
                this.getHandleStatus(this.checkNo)
              }
              this.getcheckDetailList(this.selectedStatus)
            } else {
              this.$message.error(response.msg)
            }
          })
        })
        .catch(() => {})
    },
    handleSelectionChange() {},
    /** 删除操作 */
    handleDelete(row) {
      const { id, name } = row
      this.$modal
        .confirm('确定删除"' + name + '"?', '确定删除')
        .then(function () {
          return delCheckDetailPlus({ detailId: id })
        })
        .then(() => {
          const row = { id: this.taskId }
          this.init(row)
          this.$modal.msgSuccess('删除成功')
        })
        .catch(() => {})
    },
    /* 导出 */
    handleExport() {
      this.download(
        '/biz/check-detail/export/?taskId=' + this.taskId,
        {},
        `盘点明细_${this.parseTime(new Date(), '{y}-{m}-{d}')}.xlsx`
      )
    }
  }
}
</script>

<style lang="scss" scoped>
.form-top {
  display: flex;
  justify-content: space-between;
  margin: 20px 0;
  .top-left {
    display: flex;
  }
  .top-right {
    display: flex;
  }
}
.form-process {
  display: flex;
  align-items: center;
  .process-label {
    margin-right: 10px;
  }
}
.form-info {
  margin: 15px 0;
}
.form-footer {
  text-align: end;
}
.table-css {
  margin-top: 30px;
}
</style>
