<template>
  <div class="design-box">
    <el-collapse v-model="activeNames" class="collapse-box">
      <el-collapse-item name="basicInfo">
        <template #title>
          <i class="iconfont icon-down" :class="{'active': !activeNames.includes('basicInfo')}" style="margin:0 5px;color:#6A7697;" />
          <div class="content">
            <div class="content-title">基本信息</div>
          </div>
        </template>
        <el-form inline class="basic-form">
          <el-form-item label="流程名称">
            <div class="form-content">{{ row.processName }}</div>
          </el-form-item>
          <template v-if="row.type === '0'">
            <el-form-item label="表单名称">
              <div class="form-content">{{ row.formName }}</div>
            </el-form-item>
            <el-form-item label="表单类别">
              <div class="form-content">{{ categoryName }}</div>
            </el-form-item>
          </template>
          <template v-else>
            <el-form-item label="业务名称">
              <div class="form-content">{{ row.bizName }}</div>
            </el-form-item>
            <el-form-item label="业务类别">
              <div class="form-content">{{ row.bizType }}</div>

            </el-form-item>
          </template>
          <br>
          <!-- 产品要删掉备注
            <el-form-item label="备注">
            <div class="form-content">{{ row.remark }}</div>
          </el-form-item> -->
        </el-form>
      </el-collapse-item>
      <el-collapse-item name="process">
        <template #title>
          <i class="iconfont icon-down" :class="{'active':!activeNames.includes('process')}" style="margin:0 5px;color:#6A7697;" />
          <div class="content">
            <div class="content-title">流程预览</div>
          </div>
        </template>
        <process-designer
          ref="processDesigner"
          v-model="xmlString"
          style="height: 400px"
          v-bind="controlForm"
          :process-name="processName"
        />
      </el-collapse-item>
    </el-collapse>
  </div>
</template>

<script>
import ProcessDesigner from '@/bpmn/designer/ProcessDesignerPreview'
import { mapGetters } from 'vuex'
export default {
  name: 'PreviewDetail',
  components: {
    ProcessDesigner
  },
  props: ['processName'],
  data() {
    return {
      activeNames: ['basicInfo', 'process'],
      xmlString: '',
      modeler: null,
      controlForm: {
        processId: '',
        processName: '',
        simulation: true,
        labelEditing: false,
        labelVisible: false,
        prefix: 'camunda',
        headerButtonSize: 'mini'
      },
      row: {}
    }
  },
  computed: {
    ...mapGetters(['processCategoryList']),
    categoryName() {
      return this.processCategoryList.find(item => item.id === this.row.formCategoryId)?.categoryName
    }
  },
  methods: {
    // 初始化
    initDiagram(row) {
      this.row = row
      this.$refs.processDesigner.initDiagram(row)
    }
  }
}
</script>

<style lang="scss" scoped>
.design-box {
  width: 100%;
  height: 600px;
  box-sizing: border-box;
  background-color: #ffffff;
  display: inline-flex;
  grid-template-columns: 100px auto max-content;
  .basic-form {
    margin: 0 32px;
    border: 0;
  }
  .form-content {
    min-width: 200px;
    padding-left: 4px;
  }
  .collapse-box {
    width: 100%;
  }
  .active {
    transform: rotate(-90deg);
  }
  ::v-deep .el-collapse-item__arrow {
    display: none;
  }
  .content-title{
    font-size: 16px;
  }
}
</style>
