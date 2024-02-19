<template>
  <div>
    <div class="form-top">
      <div class="top-left">
        <div class="form-time">
          <div class="item-time">
            <div class="item-time-label">计划开始</div>
            <el-input v-model="formData.planStartTime" disabled />
          </div>
          <div class="item-time">
            <div class="item-time-label">实际开始</div>
            <el-input v-model="formData.startTime" disabled />
          </div>
          <div class="item-time">
            <div class="item-time-label">完成时间</div>
            <el-input v-model="formData.endTime" disabled />
          </div>
        </div>
        <div class="form-process">
          <div class="process-label">盘点范围</div>
          <div style="width:600px">
            <el-tooltip effect="dark" :content="formData.checkRangeName" placement="left-end" :disabled="!isShowTooltip">
              <el-input v-model="formData.checkRangeName" disabled class="overInput" @mouseenter.native="visibilityChange($event)" />
            </el-tooltip>
          </div>
        </div>
        <div class="form-process">
          <div class="process-label">盘点进度</div>
          <div style="width:600px">
            <el-progress :percentage="formData.progress" />
          </div>
        </div>
      </div>
      <div class="top-right">
        <ve-pie :data="chartData" :settings="{ type: 'pie' }" width="300px" height="200px" :extend="extend" legend-position="bottom" :legend="legend" />
      </div>
    </div>

    <div class="form-state">
      <div class="form-state-item">盈亏处理：<span>{{ formData.plusLossProcessStatus==='0'?'未处理':formData.plusLossProcessStatus==='1'?'已处理':'' }}</span></div>
      <div class="form-state-item">盘点任务：<span>{{ formData.taskStatus==='0'?'未开始':formData.taskStatus==='1'?'盘点中':formData.taskStatus==='2'?'已完成':'' }}</span></div>
    </div>
    <div>
      <el-table
        ref="table"
        v-loading="loading"
        highlight-current-row
        :data="tableData"
        style="margin-top: 10px"
        max-height="300"
        @selection-change="handleSelectionChange"
      >
        <el-table-column
          type="index"
          label="序号"
          width="50"
          align="center"
        />

        <el-table-column
          label="资产编码"
          align="center"
          :show-overflow-tooltip="true"
          prop="internalCode"
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
          :show-overflow-tooltip="true"
          prop="name"
        />
        <el-table-column
          key="brandName"
          label="品牌"
          align="center"
          prop="brandName"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          label="规格型号"
          align="center"
          :show-overflow-tooltip="true"
          prop="standard"
        />
        <el-table-column
          label="存放部门"
          align="center"
          :show-overflow-tooltip="true"
          prop="deptName"
        />
        <el-table-column
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
          :show-overflow-tooltip="true"
          prop="checkStatus"
        >
          <template slot-scope="scope">
            <div>
              <!-- 只有四种状态 -->
              <span v-if="scope.row.checkStatus==='0'">未盘</span>
              <span v-if="scope.row.checkStatus==='1'">正常</span>
              <span v-if="scope.row.checkStatus==='2'">盘盈</span>
              <span v-if="scope.row.checkStatus==='3'">盘亏</span>
              <!-- <span v-if="scope.row.checkStatus==='4'">应盘</span> -->
            </div>
          </template>
        </el-table-column>
        <el-table-column
          label="备注"
          align="center"
          :show-overflow-tooltip="true"
          prop="remark"
        />
      </el-table>
    </div>
    <!-- <div class="common-pagination">
      <pagination
        v-show="total > 0"
        :total="total"
        :page.sync="queryParams.pageNum"
        :limit.sync="queryParams.pageSize"
        @pagination="getcheckDetailList"
      />
    </div> -->
  </div></template>

<script>
import { getcheckDetailNum, getcheckDetailList, completeTask } from '@/api/assetmanagement/takeStock.js'
import VePie from 'v-charts/lib/pie.common'
import { judeInputOver } from '@/utils/jri.js'
export default {
  name: 'AddWarehouseOrder',
  dicts: [],
  components: { VePie },
  props: {
    handleType: {
      type: Number,
      default: 1
    }

  },
  data () {
    return {
      formData: {
        checkRangeName: ''
      },
      loading: false,
      tableData: [],
      showShort: false, // 盘亏弹框
      isShowTooltip: false,
      extend: {
        series: {
          type: 'pie',
          radius: [0, '55%'], // 设置饼图的半径
          center: ['50%', '30%'],
          top: '10%',
          avoidLabelOverlap: true, // 防止标签重叠
          itemStyle: {
            emphasis: {
              shadowBlur: 10,
              shadowOffsetX: 0,
              shadowColor: 'rgba(0, 0, 0, 0.5)'
            },
            normal: {
              label: {
                show: true,
                // formatter: '{b} : {c} ({d}%)' //带当前图例名 + 百分比
                formatter: '{d}%', // 只要百分比
                textStyle: {
                  fontSize: 10
                }
              },
              labelLine: { show: true }

            }
          }
          // minAngle: 5 // 设置扇区最小角度
        }
      },
      legend: {
        orient: 'horizontal',
        x: 'center',
        y: 'bottom'
      },
      chartData: {
        columns: ['name', 'value'],
        rows: [
          // { name: '正常', value: 200 },
          // { name: '盘盈', value: 200 },
          // { name: '盘亏', value: 200 },
          // { name: '未盘', value: 200 }
        ] }
      // 分页
      // total: 0,
      // queryParams: {
      //   pageNum: 1,
      //   pageSize: 10
      // }
    }
  },
  watch: {

  },
  created () {

  },
  methods: {
    /** 格式化表单展现数据 */
    getFormData(data) {
      this.formData = this.$deepClone(data)
      const deptString = this.formData.deptNames ? '指定部门：' + this.formData.deptNames.join(',') + ';' : ''
      const placeString = this.formData.placeNames ? '指定位置：' + this.formData.placeNames.join(',') + ';' : ''
      const categoryString = this.formData.goodCodeNames ? '指定类别：' + this.formData.goodCodeNames.join(',') + ';' : ''
      const warehouseString = this.formData.warehouseNames ? '指定仓库：' + this.formData.warehouseNames.join(',') + ';' : ''
      const checkRangeDetail = `【指定范围】${deptString} ${placeString} ${categoryString} ${warehouseString}`
      this.formData.checkRangeName = data.checkRange === '0' ? '所有' : checkRangeDetail
      // console.log(this.formData, 'formdata')
      this.getcheckDetailNum(data.id)
      this.getcheckDetailList('6')
    },
    // 饼图数据
    getcheckDetailNum (id) {
      this.chartData.rows = []
      getcheckDetailNum({ taskId: id }).then(
        (response) => {
          if (response.code === 200) {
            this.$set(this.formData, 'progress', Number(response.data.progress))
            const arr = [
              { name: '正常', value: response.data.normal },
              { name: '盘盈', value: response.data.checkPlus },
              { name: '盘亏', value: response.data.checkLoss },
              { name: '未盘', value: response.data.notCheck }
            ]
            this.chartData.rows = arr
          } else {
            this.$message.error(response.msg)
          }
        }
      )
    },
    getcheckDetailList(status) {
      this.loading = true
      const params = {
        status: status,
        taskId: this.formData.id
        // pageNum: this.queryParams.pageNum,
        // pageSize: this.queryParams.pageSize

      }
      getcheckDetailList(params).then(
        (response) => {
          if (response.code === 200) {
            this.tableData = response.data
            // this.total = response.data.length
            this.loading = false
          } else {
            this.$message.error(response.msg)
          }
        }
      )
    },
    // 完成盘点任务
    completeTask () {
      completeTask(this.formData.id).then(
        (response) => {
          if (response.code === 200) {
            this.$modal.msgSuccess('完成任务')
            this.$emit('finishTask')
          } else {
            this.$message.error(response.msg)
          }
        }
      )
    },
    // 盘亏弹框
    showLoss() {
      this.showShort = true
    },
    // 添加盘盈资产
    addProfit() {
      this.$emit('addProfit')
    },
    // 批量处理盈亏
    batchHandle() {
      // console.log('盈亏处理')
      this.$confirm('确定批量处理盈亏资产?', '提示', {
        confirmButtonText: '确定',
        cancelButtonText: '取消',
        type: 'warning',
        title: '批量处理盈亏资产'
      }).then(() => {
        this.$message({
          type: 'success',
          message: '处理成功!'
        })
      }).catch(() => {
        // this.$message({
        //   type: 'info',
        //   message: '已取消删除'
        // })
      })
    },
    handleSelectionChange() {},
    // 超出输入框展示el-tooltip提示
    visibilityChange(event) {
      const ev = event.target.firstElementChild
      this.isShowTooltip = judeInputOver(ev)
    }

  }
}
</script>

<style lang="scss" scoped>
.form-top{
  display: flex;
  justify-content: space-between;
  margin: 10px 0;
  .top-left{
    width: 70%;
  }
  .top-right{
    width: 30%;
    height: 200px;
  }
}
.form-process{
  display: flex;
  align-items: center;
  margin: 15px 0;
  .process-label{
    margin-right: 10px;
  }
}
.form-info{
  margin: 15px 0;
}
.form-footer{
    text-align: end;
  }
  .form-time{
    display: flex;
    justify-content: space-between;
    margin: 15px 0;
    .item-time{
      display: flex;
      align-items: center;
      margin-right: 5px;
      &-label{
        width:86px;
      }
    }
  }
  .form-state{
    display: flex;
    &-item{
      margin-right: 20px;
    }
  }
  .overInput{
    width: 742px;
  //设置输入框超出长度隐藏并显示省略号
::v-deep .el-input__inner {
  overflow: hidden;
  white-space: nowrap;
  text-overflow: ellipsis;
}
}
</style>
