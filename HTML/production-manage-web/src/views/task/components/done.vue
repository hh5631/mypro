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
      <el-table-column label="发起人" align="left" prop="initiatorName" />
      <el-table-column label="开始时间" align="left" prop="startTime" />
      <el-table-column label="结束时间" align="left" prop="endTime" />
      <el-table-column align="left" label="操作" width="200">
        <template slot-scope="scope">
          <el-button size="small" type="text" icon="el-icon-edit" @click="handle(scope.row)">详情 </el-button>
        </template>
      </el-table-column>
    </el-table>
  </div>
</template>
<script>
export default {
  props: ['data'],
  methods: {
    handle(row) {
      this.$emit('handle', row)
    }
  }
}
</script>
