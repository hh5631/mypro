<!-- tree-select -->
<template>
  <el-dialog :title="title" :close-on-click-modal="false" :visible.sync="IsShowDept" width="600px" append-to-body class="leader-dialog" @close="onClose">
    <div class="dept-select">
      <el-input ref="input" v-model.trim="deptName" placeholder="请输入" maxlength="15" size="mini" class="search-department">
        <i slot="suffix" class="iconfont icon-sousuo" />
      </el-input>
      <div class="head-container select-dept-tree">
        <el-tree
          ref="tree"
          :data="treeOptions"
          :props="treeProps"
          :expand-on-click-node="false"
          :filter-node-method="filterNode"
          node-key="id"
          :default-expanded-keys="expandedArr"
          :highlight-current="true"
          @node-click="handleNodeClick"
        />
      </div>
    </div>
    <div slot="footer" class="dialog-footer">
      <el-button size="small" @click="onClose">取 消</el-button>
      <el-button size="small" type="primary" @click="saveSelect">保 存</el-button>
    </div>
  </el-dialog>
</template>

<script>
// import { treeselect } from '@/api/system/dept'
export default {
  name: 'TreeSelect',
  props: {
    handleFunction: {
      type: Function,
      default: () => {}
    },
    title: {
      type: String,
      default: '选择部门'
    }
  },
  data() {
    return {
      IsShowDept: true,
      deptName: '',
      treeProps: {
        children: 'children',
        label: 'label'
      },
      treeOptions: [],
      expandedArr: [],
      selectNode: {}
    }
  },
  watch: {
    // 根据名称筛选部门树
    deptName(val) {
      this.$refs.tree.filter(val)
    }
  },
  mounted() {
    this.getTreeselect()
  },
  methods: {
    // 筛选节点
    filterNode(value, data) {
      if (!value) return true
      return data.label.indexOf(value) !== -1
    },
    handleNodeClick(data) {
      this.selectNode = data
      this.$refs.input.focus()
      this.$refs.input.blur()
    },
    getTreeselect() {
      this.handleFunction().then(response => {
        this.treeOptions = response.data || []
        // this.expandedArr = (response.data && response.data.map(item => item.id)) || []
        this.treeOptions.forEach(item => {
          this.expandedArr.push(item.id)
        })
      })
    },
    onClose() {
      this.$emit('onClose')
    },
    saveSelect() {
      this.$emit('onClose')
      this.$emit('selectNode', this.selectNode)
    }
  }
}
</script>

<style lang="scss" scoped>
.leader-dialog{
  &::v-deep .el-dialog__body{
    padding: 0;
  }
}

.dept-select{
  height: 600px;
  display: flex;
  flex-direction: column;
  padding: 16px;
}

.search-department {
  ::v-deep .el-input__inner{
      height:32px;
      padding-top:2px;
      margin-bottom: 8px
  }
  ::v-deep .el-input__suffix{
        margin-top: 8px;
      }
}

.select-dept-tree {
  flex: 1;
  overflow: auto;
  padding-bottom: 8px;
}
</style>
