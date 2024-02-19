
<template>
  <el-dialog title="详情" :visible.sync="dialogVisible" width="1400px" height="800">
    <flow-designer ref="flowdesigner" />
  </el-dialog>
</template>
<script>
import FlowDesigner from '../component/preview-detail'
export default {
  components: {
    FlowDesigner
  },
  data() {
    return {
      dialogVisible: false
    }
  },
  methods: {
    initDiagram(row) {
      this.dialogVisible = true
      this.$nextTick(() => {
        this.$refs.flowdesigner.initDiagram(row)
      })
    }
  }
}
</script>
<style lang="scss" scoped>

</style>
