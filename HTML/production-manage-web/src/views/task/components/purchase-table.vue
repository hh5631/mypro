<!--
 * @Descripttion:
 * @version:
 * @Author: lirioing
 * @Date: 2022-07-04 14:54:09
 * @LastEditors: lirioing
 * @LastEditTime: 2022-09-06 13:46:31
-->
<template>
  <div>
    <!-- 采购申请表格 -->
    <el-table
      ref="table"
      border
      :data="detailData"
      style="width: 100%"
      max-height="400"
    >
      <!-- <el-table-column type="index" label="序号" width="50" fixed />
      <el-table-column
        prop="orderNoArr"
        label="关联采购计划"
        align="center"
        fixed="left"
        :show-overflow-tooltip="true"
      /> -->
      <el-table-column
        prop="materialCategory"
        label="资产类别"
        align="center"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        prop="materialName"
        label="资产名称"
        align="center"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        prop="brandName"
        label="品牌"
        align="center"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        prop="materialStandard"
        label="规格型号"
        align="center"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        prop="unitName"
        label="单位"
        align="center"
      />
      <el-table-column
        prop="planQty"
        label="采购数量"
        align="center"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        prop="unitPrice"
        label="单价(元)"
        width="80"
        align="center"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        prop="totalPrice"
        label="小计(元)"
        width="120"
        align="center"
        :show-overflow-tooltip="true"
      />
      <el-table-column prop="remark" label="备注" align="center" />
      <el-table-column
        label="附件"
        width="120"
        align="center"
      >
        <template slot-scope="scope">
          <div v-if="scope.row.fileList && scope.row.fileList.length">
            <div
              style="color: #4686ef"
              @click="showFileDig(scope.$index, scope.row.fileList)"
            >
              共{{
                scope.row.fileList && scope.row.fileList.length > 0
                  ? scope.row.fileList.length
                  : 0
              }}项
            </div>
          </div>
          <div v-else>暂无</div>
        </template>
      </el-table-column>
    </el-table>
    <el-dialog
      title="附件"
      :visible.sync="fileDig"
      width="800px"
      append-to-body
      :close-on-click-modal="false"
      class="selectPlan1"
      destroy-on-close
      @close="closeFileDig"
    >
      <editFile
        ref="editFile"
        :file-list="fileList"
        :is-detail="'1'"
        @delFile="delFileList"
        @keySearch="keySearch"
      />
      <div slot="footer" class="dialog-footer">
        <el-button
          size="small"
          style="float: left"
          @click="batchDownload"
        ><i class="iconfont icon-daoru" />批量下载</el-button>
        <!-- <el-button
          size="small"
          :disabled="isDetail === '1'"
          @click="batchDel"
        >删除</el-button> -->
        <!-- <el-button
              v-if="isDetail !== '1'"
              size="small"
              type="primary"
              @click="saveFile"
            >保存</el-button> -->
      </div>
    </el-dialog>
  </div>
</template>
<script>
import editFile from '@/views/purchase/apply/component/editFile.vue'
export default {
  components: { editFile },
  props: ['detailData'],
  data() {
    return {
      fileDig: false,
      fileList: []
    }
  },
  methods: {
    batchDownload () {
      this.$refs.editFile.batchDownload()
    },
    // 批量删除
    batchDel () {
      const arr = this.$refs.editFile.selectedFiles
      if (arr.length === 0) {
        return this.$message.error('请先选择文件')
      }
      arr.forEach(e => {
        this.delFileList(e)
      })
    },
    delFileList (row) {
      const index = this.fileList.findIndex(e => {
        return e.id === row.id
      })

      this.fileList.splice(index, 1)
    },
    // 文件列表关键字搜索
    keySearch (name) {
      if (name === '') {
        this.fileList = this.tableData[this.index].fileList
      } else {
        this.fileList = this.fileList.filter(item => item.name.toString().indexOf(name) >= 0)
      }
    },
    // 保存文件
    saveFile () {
      this.tableData[this.index].fileList = this.fileList
      this.fileDig = false
    },
    showFileDig (index, fileList) {
      this.index = index
      this.fileDig = true
      this.fileList = fileList.concat()
    },
    closeFileDig () {
      this.fileDig = false
    }
  }
}
</script>
