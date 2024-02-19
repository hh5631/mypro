<!-- 流程图的公用组件 -->
<template>
  <el-tabs v-model="activeName" class="tabs" @tab-click="handleClick">
    <el-tab-pane v-if="reviewShow" label="审批记录" name="first">
      <el-table border :data="reviewData" class="table-class" highlight-current-row>
        <!-- 表单项通过组件props属性进行动态配置 -->
        <el-table-column v-for="(item , index) in columns1" :key="index" :prop="item.prop" :label="item.label" align="center" />
      </el-table>
    </el-tab-pane>
    <el-tab-pane v-if="processShow" label="流程图" name="second">
      <process-designer
        ref="processDesigner"
      />
    </el-tab-pane>
    <el-tab-pane v-if="actionsShow" label="操作日志" name="third">
      <el-table border :data="actionsData" :span-method="codeMethod" class="table-class" highlight-current-row>
        <!-- 表单项通过组件props属性进行动态配置 -->
        <el-table-column v-for="(item , index) in columns2" :key="index" :prop="item.prop" :label="item.label" align="center" />
      </el-table>
    </el-tab-pane>
  </el-tabs>
</template>

<script>
import ProcessDesigner from '@/bpmn/designer/ProcessDesignerPreview.vue'
import { getProcessList, getDefinitions } from '@/api/purchase/index'
import { getHighLine } from '@/api/process'
import { getLog } from '@/api/warehouse/index'
export default {
  name: 'ReviewProcess',
  components: { ProcessDesigner },
  props: {
    id: {
      type: Number,
      default: undefined
    },
    processInstanceId: {
      type: Number,
      default: undefined
    },
    // clolumns 传递格式如下 [{prop:'',label:''}]
    columns1: {
      type: Array,
      default: () => {
        return []
      }
    },
    columns2: {
      type: Array,
      default: () => {
        return []
      }
    },
    reviewShow: {
      type: Boolean,
      default: true
    },
    processShow: {
      type: Boolean,
      default: true
    },
    actionsShow: {
      type: Boolean,
      default: true
    },
    bizType: {
      type: String,
      default: ''
    }
  },
  data() {
    return {
      activeName: 'first',
      reviewData: [],
      actionsData: []
    }
  },
  mounted() {
    this.getList()
  },
  methods: {
    handleClick(value) {
      switch (value) {
        case 'first':
          this.getList()
          break
        case 'second':
          break
        case 'third':
          this.getLogList()
          break
      }
    },
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
    getList() {
      getProcessList(this.processInstanceId).then(res => {
        this.reviewData = res.data.records || []
      })
    },
    getLogList() {
      const data = {
        bizOrderId: this.id,
        bizType: this.bizType,
        pageNum: 1,
        pageSize: 1000
      }
      getLog(data).then(response => {
        if (response.code === 200) {
          this.actionsData = response.data.records || []
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
    },
    /** 查看流程高亮图*/
    async getHighLine(data) {
      const res = await getHighLine({ instanceId: this.processInstanceId })
      await this.$refs.processDesigner.initDiagram(data, res.data, false)
      await this.$nextTick()
      setTimeout(() => {
        this.$refs.processDesigner.refit()
      }, 200)
    }
  }
}
</script>

<style lang="scss" scoped>
.tabs{
  flex: 1;
  padding:20px;
  overflow-y:auto;
  background:#fff;
  box-shadow: 0 2px 12px 0 rgb(0 0 0 / 10%);
}
.table-class{
  margin-top: 10px;
}
</style>
