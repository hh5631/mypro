<template>
  <el-card class="approvlaProcess">
    <p class="app_title">操作日志</p>
    <!-- <div class="topBar">
      <div ref="barLeft" class="left">
        <div class="left-bar" :class="{'active':activeIndex==0}" @click="activeChange(0)">审批记录</div>
        <div class="left-bar" :class="{'active':activeIndex==1}" @click="activeChange(1)">流程图</div>
        <div class="left-bar" :class="{'active':activeIndex==2}" @click="activeChange(2)">操作日志</div>
        <div class="active-border" :style="lineStyle" />
      </div>
    </div> -->
    <!-- <div class="line" /> -->
    <!-- 审批记录 -->
    <div v-show="activeIndex==0">
      <el-table ref="tableData" border :data="tableData" :span-method="codeMethod" class="table-class" highlight-current-row>
        <el-table-column prop="code" label="采购单编号" align="center" />
        <el-table-column prop="endTime" label="操作时间" align="center" />
        <el-table-column prop="taskName" label="节点名称" align="center" />
        <el-table-column prop="assigneeName" label="操作者" align="center" />
        <el-table-column prop="postName" label="职位" align="center" />
        <el-table-column prop="result" label="审批状态" align="center">
          <template slot-scope="scope">
            <div>
              <span>{{ scope.row.result==='2'?'待办':'已办理' }}</span>
            </div>
          </template>
        </el-table-column>
        <el-table-column prop="result" label="审批结果" align="center">
          <template slot-scope="scope">
            <div>
              <span>{{ scope.row.result==='1'?'同意':(scope.row.result==='2'?'无':'不同意') }}</span>
            </div>
          </template>
        </el-table-column>
        <el-table-column prop="message" label="处理意见" align="center" />
      </el-table>
    </div>
    <!-- 流程图 -->
    <div v-show="activeIndex==1" style="height:600px;">
      <process-designer
        ref="processDesigner"
      />
    </div>
    <!-- 操作日志 -->
    <div v-show="activeIndex==2">
      <el-table ref="logTableData" border :data="logTableData" class="table-class" highlight-current-row>
        <el-table-column prop="operationTime" label="操作时间" align="center" />
        <el-table-column prop="operation" label="节点名称" align="center" />
        <el-table-column prop="operator" label="操作者" align="center" />
        <el-table-column prop="position" label="职位" align="center" />
        <el-table-column prop="operation" label="操作" align="center" />
      </el-table>
    </div>
  </el-card>
</template>

<script>
import { getProcessList, getDefinitions } from '@/api/purchase/index'
import { getHighLine } from '@/api/process'
import { getLog } from '@/api/warehouse/index'
import ProcessDesigner from '../../../../bpmn/designer/ProcessDesignerPreview.vue'
export default {
  components: {
    ProcessDesigner
  },
  props: {
    id: {
      type: Number,
      default: undefined
    }
  },
  data() {
    return {
      activeIndex: 2,
      lineStyle: {},
      // 流程图
      instanceData: {
        highLine: [],
        highPoint: [],
        ido: [],
        waitingToDo: []
      },
      processInstanceId: 'c8b83448-098c-11ed-8325-ae155948e8d4',
      // 审批流程
      tableData: [],
      // 操作日志
      logTableData: []
    }
  },
  mounted() {
    this.changeLineWidth()
    this.getProcessList()
  },
  methods: {
    /** 获取审批流程数据 */
    getProcessList() {
      getProcessList(this.processInstanceId).then(res => {
        this.tableData = res.data.records
      })
    },
    /** 查看流程高亮图*/
    async getHighLine(data) {
      const res = await getHighLine({ instanceId: this.processInstanceId })
      await this.$refs.processDesigner.initDiagram(data, res.data, false)
      await this.$nextTick()
      setTimeout(() => {
        this.$refs.processDesigner.refit()
      }, 200)
    },
    /** 审批记录 */
    codeMethod({ row, column, rowIndex, columnIndex }) {
      if (columnIndex === 0) {
        if (rowIndex % 2 === 0) {
          return {
            rowspan: 2,
            colspan: 1
          }
        } else {
          return {
            rowspan: 0,
            colspan: 0
          }
        }
      }
    },
    /** 切换按钮 */
    activeChange(index) {
      this.activeIndex = index
      this.changeLineWidth()
      switch (index) {
        case 0:
          this.getProcessList()
          break
        case 1:
          this.getDefinitions()
          break
        case 2:
          this.getLogList()
          break
      }
    },
    /** 改变行宽 */
    changeLineWidth() {
      const ele = this.$refs.barLeft.getElementsByClassName('left-bar')[this.activeIndex]
      const left = (ele.offsetWidth - 48) / 2 + ele.offsetLeft + 'px'
      this.lineStyle = { left: left }
    },
    /** 获取操作日志 */
    getLogList() {
      const data = {
        bizOrderId: this.id,
        bizType: 'type_purchase_plan',
        pageNum: 1,
        pageSize: 1000
      }
      getLog(data).then(response => {
        if (response.code === 200) {
          this.logTableData = response.data.records
        } else {
          this.$message.error(response.msg)
        }
      })
    },
    /** 获取流程图 */
    getDefinitions() {
      getDefinitions(this.processInstanceId).then((da) => {
        if (da.code === 200) {
          const obj = {
            deploymentId: da.data.deploymentID,
            resourceName: da.data.resourceName
          }
          this.getHighLine(obj)
        }
      })
    }
  }
}
</script>

<style lang="scss" scoped>
.approvlaProcess{
  flex: 1;
  overflow-y:auto;
  min-height:0;
    .app_title{
      font-size: 18px;
      font-family: SourceHanSansCN-Medium-, SourceHanSansCN-Medium;
      font-weight: normal;
      color: #6A7697;
    }
    .topBar {
      display: flex;
      justify-content: space-between;
      height: 44px;
      line-height: 44px;
      background: #fff;
      .left {
        font-size: 16px;
        font-family: SourceHanSansCN-Medium, SourceHanSansCN;
        font-weight: 500;
        color: #333333;
        display: flex;
        position: relative;
        .left-bar {
          margin: 0 16px;
          cursor: pointer;
          &:first-of-type {
            margin-left: 24px;
          }
        }
        .active{
            color: #4686EF;
        }
        .active-border{
            position: absolute;
            bottom: 0;
            // left:30px;
            width: 48px;
            height: 2px;
            background: #4686EF;
            border-radius: 2px;
            transition: all 0.5s;
        }
      }
    }
    .line{
      height: 1px;
      width: 100%;
      background: #DDDDDD;
      margin: 0 0 10px 0;
    }
}

</style>
