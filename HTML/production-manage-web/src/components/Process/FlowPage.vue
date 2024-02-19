<template>
  <el-dialog
    fullscreen
    title=""
    :visible.sync="dialogVisible"
    :append-to-body="true"
  >
    <div>
      <div class="flow-content">
        <div class="flow-flex">
          <div class="flow-step">
            <el-steps :active="active" finish-status="success">
              <el-step title="基础信息" />
              <el-step title="表单设计" />
            </el-steps>
          </div>

          <div class="right-btn">
            <el-button
              v-show="active != 0"
              size="small"
              @click="stepAhead()"
            >上一步</el-button>
            <el-button
              v-show="active != 1"
              size="small"
              @click="stepNext()"
            >下一步</el-button>
            <el-button
              v-show="active == 1"
              size="small"
              type="primary"
              @click="submit()"
            >完成</el-button>
          </div>
        </div>
        <slot name="form" />
        <slot name="section" />
      </div>
    </div>
  </el-dialog>
</template>
<script>
export default {
  data() {
    return {
      active: 0,
      dialogVisible: false,
      title: '新增'
    }
  },
  methods: {
    openDialog() {
      this.active = 0
      this.dialogVisible = true
    },
    closeDialog() {
      this.dialogVisible = false
    },
    stepAhead() {
      this.active = 1
      this.$emit('stepAhead', this.active)
    },
    stepNext() {
      this.$emit('stepNext', this.active)
    },
    submit() {
      // const _that = this
      this.$modal
        .confirm(`是否继续？`)
        .then(() => {
          this.$emit('submit')
        })
        .catch(() => {})
    }
  }
}
</script>
<style lang="scss" scoped>
.flow-content {
  width: 100vw;
  height: calc(100vh - 32px);
  overflow: hidden;
  .flow-flex {
    display: flex;
    padding: 0 20px;
    .flow-step {
      width: 80%;
    }
  }
  .right-btn {
    display: flex;
    margin-left: 20px;
    ::v-deep .el-button {
      // margin: auto;
      margin-right: 20px;
      height: 34px;
    }
  }
}
// ::v-deep .el-dialog {
//   width: 100%;
//   margin-top: 0 !important;
// }
::v-deep .el-dialog__header {
  border-bottom: none;
}
::v-deep .el-dialog__body {
  padding: 0;
}
</style>
