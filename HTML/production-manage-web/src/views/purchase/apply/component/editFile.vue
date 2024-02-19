<!--
 * @Descripttion:
 * @version:
 * @Author: lirioing
 * @Date: 2022-07-04 14:54:09
 * @LastEditors: liquan 1106970756@qq.com
 * @LastEditTime: 2022-09-09 09:11:29
-->
<template>
  <div class="selectLeader">
    <el-input v-model.trim="fileName" maxlength="15" placeholder="请输入搜索关键字" size="mini" clearable @keyup.enter.native="searchFun">
      <i slot="suffix" class="el-input__icon el-icon-search" @click="searchFun" />
    </el-input>
    <el-table
      ref="fileList"
      v-loading="loading"
      :row-key="getRowKeys"
      highlight-current-row
      :data="fileList"
      style="margin-top: 10px;"
      :disabled="isDetail==='1'"
      @selection-change="handleRoleSelectionChange"
    >
      <el-table-column type="selection" width="50" :reserve-selection="true" align="center" />
      <el-table-column type="index" width="70" label="序号" />
      <el-table-column key="name" label="文件名称" align="left" prop="name" :show-overflow-tooltip="true" />
      <el-table-column
        fixed="right"
        label="操作"
        width="200"
      >
        <template slot-scope="scope">
          <el-button type="text" size="small" @click="handleClick(scope.row,0)">下载</el-button>
          <el-button type="text" size="small" @click="handleClick(scope.row,1)">预览</el-button>
          <!-- <el-button v-show="isDetail!=='1'" type="text" size="small" @click="handleDel(scope.row)">删除</el-button> -->
        </template>
      </el-table-column>
    </el-table>
    <el-dialog title="预览" :visible.sync="fileDig" append-to-body width="1200">
      <filePreview :src="url" />
    </el-dialog>
  </div>
</template>

<script>
import { getRealPath } from '@/api/system/unit'
import filePreview from '@/components/FilePreview/index'
export default {
  components: { filePreview },
  props: {
    fileList: {
      type: Array,
      default: () => []
    },
    isDetail: {
      type: String,
      default: ''
    }
  },
  data() {
    return {
      loading: false,
      fileName: '',
      selectedFiles: [],
      fileDig: false,
      url: ''
    }
  },
  methods: {
    searchFun() {
      this.$emit('keySearch', this.fileName)
    },
    getRowKeys() { },
    handleRoleSelectionChange(val) {
      this.selectedFiles = val
    },
    // 下载预览
    handleClick(file, index) {
      // index==1预览0下载
      if (index) {
        this.fileDig = true
      } else {
        this.download(`/file/download/?fileName=${file?.location}`, {}, file.name)
        return
      }
      getRealPath(file?.location).then(res => {
        this.url = this.filePreview(res.data)
      })
    },
    // iframe下载文件
    downloadFile(url) {
      try {
        const fileName = url.split('/')[url.split('/').length - 1]
        // 对于<a>标签，只有 Firefox 和 Chrome（内核） 支持 download 属性
        // IE10以上支持blob但是依然不支持download
        if ('download' in document.createElement('a')) { // 支持a标签download的浏览器
          const link = document.createElement('a') // 创建a标签
          link.download = fileName// a标签添加属性
          link.style.display = 'none'
          link.href = url
          link.target = '_blank'
          document.body.appendChild(link)
          link.click() // 执行下载
          URL.revokeObjectURL(link.href) // 释放url
          document.body.removeChild(link) // 释放标签
        }
      } catch (e) {
        throw Promise.reject(e)
      }
      // const iframe = document.createElement('iframe')
      // iframe.style.display = 'none'
      // iframe.style.height = 0
      // iframe.src = url
      // document.body.appendChild(iframe)
      //
      // setTimeout(() => {
      //   iframe.remove()
      // }, 5 * 60 * 1000)
    },
    // 批量下载
    batchDownload() {
      if (this.selectedFiles.length === 0) {
        this.$message.error('请先选择文件')
        return
      }
      const arr = this.selectedFiles.map(element => {
        return element.location
      })
      this.download('/file/downloadAll', arr, '采购计划附件', 'zip')
    },
    // 删除
    handleDel(row) {
      this.$emit('delFile', row)
    },
    // 批量删除
    batchDel() {
      if (this.selectedFiles.length === 0) {
        this.$message.error('请先选择文件')
        return
      }
    }

  }
}
</script>

<style lang="scss" scoped>
.selectLeader{
  width: 100%;
  height: 100%;
  padding: 16px;
}
</style>
