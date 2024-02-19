<template>
  <div class="top-right-btn">
    <el-row style="margin-top: 2px;">
      <!-- <el-tooltip class="item" effect="dark" :content="showSearch ? '隐藏搜索' : '显示搜索'" placement="top">
        <el-button size="small" circle icon="el-icon-search" @click="toggleSearch()" />
      </el-tooltip> -->
      <!-- <el-tooltip class="item" effect="dark" content="下载模板" placement="top">
        <el-button v-if="download" size="small" icon="el-icon-download-draft" @click="downloadDraft" />
      </el-tooltip> -->
      <el-tooltip class="item" effect="dark" content="刷新" placement="top">
        <el-button size="small" style="margin-top:12px" class="iconfont icon-shuaxin" @click="refresh()" />
      </el-tooltip>
      <!-- <el-tooltip class="item" effect="dark" content="显隐列" placement="top" v-if="columns">
        <el-button size="small" icon="el-icon-menu" @click="showColumn()" />
      </el-tooltip> -->
    </el-row>
    <el-dialog :title="title" :visible.sync="open" append-to-body>
      <el-transfer
        v-model="value"
        :titles="['显示', '隐藏']"
        :data="columns"
        @change="dataChange"
      />
    </el-dialog>
  </div>
</template>
<script>
export default {
  name: 'RightToolbar',
  props: {
    showSearch: {
      type: Boolean,
      default: true
    },
    download: {
      type: Boolean,
      default: false
    },
    columns: {
      type: Array
    }
  },
  data() {
    return {
      // 显隐数据
      value: [],
      // 弹出层标题
      title: '显示/隐藏',
      // 是否显示弹出层
      open: false
    }
  },
  created() {
    // 显隐列初始默认隐藏列
    for (const item in this.columns) {
      if (this.columns[item].visible === false) {
        this.value.push(parseInt(item))
      }
    }
  },
  methods: {
    // 搜索
    toggleSearch() {
      this.$emit('update:showSearch', !this.showSearch)
    },
    // 刷新
    refresh() {
      this.$emit('queryTable')
    },
    downloadDraft() {
      // this.download('system/user/importTemplate', {
      // }, `user_template_${new Date().getTime()}.xlsx`)
      this.$emit('downloadDraft')
    },
    // 右侧列表元素变化
    dataChange(data) {
      for (const item in this.columns) {
        const key = this.columns[item].key
        this.columns[item].visible = !data.includes(key)
      }
    },
    // 打开显隐列dialog
    showColumn() {
      this.open = true
    }
  }
}
</script>
<style lang="scss" scoped>
::v-deep .el-transfer__button {
  border-radius: 50%;
  padding: 12px;
  display: block;
  margin-left: 0px;
}
::v-deep .el-transfer__button:first-child {
  margin-bottom: 10px;
}
::v-deep .el-button{
  width: 32px;
  height: 32px;
  padding: 0;
  margin-right: 5px;
}
</style>
<style>
.el-icon-download-draft {
  background: url("../../assets/images/download-draft@2x.png") center no-repeat;
  font-size: 18px;
  background-size: cover;
}
.el-icon-download-draft:before {
  content: "替";
  font-size: 16px;
  visibility: hidden;
}
</style>
