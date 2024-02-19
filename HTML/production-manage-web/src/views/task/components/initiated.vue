<template>
  <div style="flex: 1;">
    <el-table :data="data" style="width: 100%" height="100%">
      <el-table-column label="序号" align="left" type="index" />
      <el-table-column label="业务类型" align="left" prop="bizName">
        <template slot-scope="scope">
          <div>
            {{ scope.row.type>0?scope.row.bizName:'自定义流程' }}
          </div>
        </template>
      </el-table-column>
      <el-table-column label="业务标题" align="left" prop="title" />
      <el-table-column label="状态" align="left" prop="status">
        <span slot-scope="scope">
          {{ returnStatus(scope.row) }}
        </span>
      </el-table-column>
      <!-- <el-table-column label="流程KEY" align="center" prop="processKey" /> -->
      <!-- <el-table-column label="发起人" align="center" prop="createBy" /> -->
      <el-table-column label="发起时间" align="left" prop="startTime" />
      <el-table-column label="完成时间" align="left" prop="endTime" />
      <el-table-column label="流程名称" align="left" prop="processName" />
      <el-table-column label="流程版本" align="left" prop="version" />
      <el-table-column align="left" label="操作" width="200">
        <template slot-scope="scope">
          <el-button size="small" type="text" @click="lookProcess(scope.row)">查看 </el-button>
          <el-button v-show="scope.row.status==1" size="small" type="text" @click="withDraw(scope.row)">撤回 </el-button>
          <el-button v-show="scope.row.status==0 || scope.row.status==98" size="small" type="text" @click="toVoid(scope.row)">作废 </el-button>
          <el-button v-show="scope.row.status==98" size="small" type="text" @click="reStart(scope.row)">重新发起 </el-button>
        </template>
      </el-table-column>
    </el-table>
    <!-- 发起流程弹框 -->
    <el-dialog title="发起流程" :visible.sync="dialogFormVisible" destroy-on-close width="80%">
      <v-form-render
        ref="vFormRef"
        :form-json="formJson"
        :form-data="formData"
      />
      <div slot="footer" class="dialog-footer">
        <el-button
          icon="el-icon-close"
          size="small"
          @click="dialogFormVisible=false"
        >取 消</el-button>
        <el-button
          type="primary"
          size="small"
          icon="el-icon-check"
          @click="handleSubmit('vFormRef')"
        >保 存</el-button>
      </div>
    </el-dialog>
  </div>
</template>
<script>
import { getFormDetail } from '@/api/process'
import { startProcess } from '@/utils/process'
export default {
  props: ['data'],
  dicts: ['camunda_process_status'],
  data() {
    return {
      dialogFormVisible: false,
      formJson: {},
      formData: {}
    }
  },
  created() {
  },
  methods: {
    returnStatus(row) {
      for (var i of this.dict.type.camunda_process_status) {
        if (i.value === row.status) {
          return i.label
        }
      }
    },
    lookProcess(val) {
      this.$emit('handle', val)
    },
    withDraw(val) {
      this.$emit('withdraw', val)
    },
    toVoid(val) {
      this.$emit('tovoid', val)
    },
    reStart(val) {
      this.selectTask = val
      this.dialogFormVisible = true
      getFormDetail(val.formId).then(da => {
        this.formJson = JSON.parse(da.data.formJson)
        this.formData = JSON.parse(val.formDataJson)
      })
    },
    handleSubmit(ref) {
      startProcess(this, ref, this.selectTask).then(() => {
        this.$emit('reload')
        this.dialogFormVisible = false
      })
    }
  }
}
</script>
