<template>
  <section ref="treeSection" class="tree-container">
    <el-input
      v-model.trim="keywords"
      placeholder="请输入关键字搜索"
      clearable
      size="small"
      suffix-icon="el-icon-search"
      class="filter-input"
      maxlength="15"
    />
    <el-tree
      ref="tree"
      :data="data"
      :props="defaultProps"
      :expand-on-click-node="false"
      :filter-node-method="filterNode"
      default-expand-all
      highlight-current
      node-key="id"
      class="tree-scroll"
      @node-click="handleNodeClick"
    />
  </section>
</template>
<script>
export default {
  props: {
    // eslint-disable-next-line vue/require-prop-types
    data: {
      default: () => {
        return []
      }
    },
    defaultProps: {
      type: Object,
      default: () => {
        return {
          children: 'children',
          label: 'label'
        }
      }
    }
  },
  data() {
    return {
      keywords: ''
    }
  },
  watch: {
    // 根据关键字筛选
    keywords(val) {
      this.$refs.tree.filter(val)
    }
  },
  mounted() {
    const height = this.$refs.treeSection.parentElement.offsetHeight > 200
      ? (this.$refs.treeSection.parentElement.offsetHeight - 48) + 'px' : '100%'
    this.$nextTick(() => {
      this.$refs.tree.$el.style.height = height
    })
  },
  methods: {
    /* 搜索事件  */
    filterNode(value, data) {
      if (!value) return true
      return data.label.indexOf(value) !== -1
    },
    /* 节点单击事件 */
    handleNodeClick(data) {
      this.$emit('nodeSelect', data)
    }
  }
}
</script>
<style lang='scss' scoped>
.tree-container {
  .filter-input {
    margin-bottom: 16px;
  }
  .tree-scroll {
    height: calc(100% - 48px);
    overflow: auto;
    width: 100%;
  }
  ::v-deep .el-tree-node__content {
    padding: 6px;
    height: auto;
  }
  ::v-deep
    .el-tree--highlight-current
    .el-tree-node.is-current
    > .el-tree-node__content {
    background-color: #d7e6ff;
  }
  ::v-deep .el-tree-node__label {
    font-size: 16px;
  }
}
// .left .el-tree-node__content {
//   font-size: 16px;
//   padding: 5px 0;
//   height: auto;
// }
/* .el-tree-node:focus > .el-tree-node__content{
    background-color: #D7E6FF;
  } */
</style>
