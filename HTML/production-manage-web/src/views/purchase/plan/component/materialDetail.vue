<template>
  <div>
    <collapse title="物品明细">
      <template v-if="this.$route.query.isDetail === '0'||!this.$route.query.isDetail" #button>
        <el-button class="btnClass" plain size="small" @click.stop="addDetail"><i class="iconfont icon-zengjia" />添加明细</el-button>
      </template>
      <div class="materialDetail">
        <!-- :span-method="objectSpanMethod" class="table-class" show-summary :summary-method="getNodeSummaries" -->
        <el-table ref="table" :key="Math.random()" style="width: 100%;" border :data="tableData" highlight-current-row max-height="800">
          <el-table-column type="index" label="序号" width="50" />
          <el-table-column v-if="this.$route.query.isDetail !== '0'" prop="orderNo" label="关联采购申请" width="170" align="center" />
          <!-- <el-table-column prop="detailNo" label="明细编号" width="170" align="center" /> -->
          <el-table-column prop="materialCategory" label="资产类别" width="170" align="center" />
          <el-table-column prop="materialName" label="资产名称" width="180" align="center" />
          <el-table-column prop="brandName" label="品牌" width="170" align="center" />
          <el-table-column prop="materialStandard" label="规格型号" width="170" align="center" />
          <el-table-column prop="unitName" label="单位" width="180" align="center" />
          <el-table-column prop="planQty" label="计划数量" width="170" align="center" />
          <el-table-column prop="supplierName" label="参考供应商" width="170" align="center" />
          <el-table-column prop="expectedCompletionDate" label="预计完成时间" width="170" align="center" />
          <el-table-column prop="remark" label="备注" align="center" />
          <!-- <el-table-column prop="fileList" label="附件" width="120" align="center">
            <template slot-scope="scope">
              <el-popover
                v-if="scope.row.fileList && scope.row.fileList.length"
                placement="left"
                title="附件列表"
                trigger="click"
              >
                <a v-for="item in scope.row.fileList" :key="item.id" style="display:block;color:#4686EF;margin-bottom:10px" target="blank" :href="item.location">{{ item.name }}</a>
                <a slot="reference" style="color:#4686EF">共{{ scope.row.fileList.length }}项</a>
              </el-popover>
              <div v-else>暂无</div>
            </template>
          </el-table-column> -->
          <el-table-column prop="fileList" label="附件" width="120" align="center">
            <template slot-scope="scope">
              <div v-if="scope.row.fileList && scope.row.fileList.length">
                <div style="color:#4686EF" @click="showFileDig(scope.$index,scope.row.fileList)">共{{ scope.row.fileList&&scope.row.fileList.length>0? scope.row.fileList.length:0 }}项</div>
              </div>
              <div v-else>暂无</div>
            </template>

          </el-table-column>
          <el-table-column
            v-if="this.$route.query.isDetail === '0'"
            label="操作"
            width="100"
          >
            <template slot-scope="scope">
              <el-button type="text" size="small" @click="handleClick(scope.row,scope.$index)">编辑</el-button>
              <el-button type="text" size="small" @click="handleDelete(scope.row,scope.$index)">删除</el-button>
            </template>
          </el-table-column>
        </el-table>
      </div>
      <!-- <div class="totalDiv">
        <div class="total_title">合计</div>
        <div class="total_count">856</div>
      </div> -->
      <div v-if="open">
        <el-dialog
          title="添加明细"
          :visible.sync="open"
          width="1100px"
          append-to-body
          :close-on-click-modal="false"
          class="addDetail"
          @close="closeUser"
        >
          <add-detail ref="formRef" :forms="form" @updateData="updateData" @addData="addData" />
          <div slot="footer" class="dialog-footer">
            <el-button size="small" @click="cancel">取 消</el-button>
            <el-button size="small" type="primary" @click="submitForm">保 存</el-button>
          </div>
        </el-dialog>
      </div>

    </collapse>
    <!-- 附件弹窗 -->
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
      <editFile ref="editFile" :file-list="fileList" :is-detail="isDetail" @delFile="delFileList" @keySearch="keySearch" />
      <div slot="footer" class="dialog-footer">
        <el-button size="small" style="float:left" @click="batchDownload"><i class="iconfont icon-daoru" />批量下载</el-button>
        <!-- <el-button size="small" :disabled="isDetail==='1'" @click="batchDel">删除</el-button> -->
        <el-button v-if="isDetail !== '1'" size="small" type="primary" @click="saveFile">关闭</el-button>
      </div>
    </el-dialog>
  </div>
</template>

<script>
import collapse from './collapse.vue'
import addDetail from './addDetail.vue'
import editFile from '@/views/purchase/apply/component/editFile.vue'
export default {
  components: { collapse, addDetail, editFile },
  props: {
    tableDatas: {
      type: Array,
      default: null
    },
    isDetail: {
      type: String,
      default: ''
    }
  },
  data() {
    return {
      // 添加明细弹窗
      open: false,
      //
      form: {},
      tableData: [],
      delDetailIdList: [],
      fileDig: false,
      fileList: [],
      index: undefined // 物料选中行的索引,
    }
  },
  watch: {
    tableDatas: {
      handler(val, oldVal) {
        this.tableData = val
      },
      deep: true, // 加这个属性，深度监听
      immediate: true
    }
  },
  methods: {
    /** 添加明细方法 */
    addDetail() {
      this.open = true
      this.$nextTick(() => {
        this.$refs.formRef.form = {}
        this.$refs.formRef.fileList = []
        this.$refs.formRef.selectIndex = ''
        this.$refs.formRef.$refs.form.resetFields()
      })
    },
    /** 关闭添加明细弹窗 */
    closeUser() {
      // this.$refs.formRef.$refs.form.resetFields()
      this.open = false
    },
    /** 取消添加明细选择 */
    cancel() {
      this.open = false
      // this.$refs.formRef.$refs.form.resetFields()
      //
    },
    /** 编辑明细 */
    handleClick(row, index) {
      this.open = true
      this.$nextTick(() => {
        //
        this.$refs.formRef.form = { ...row }
        this.$refs.formRef.fileList = [...row.fileList]
        this.$refs.formRef.selectIndex = index
      })
    },
    /** 删除明细 */
    handleDelete(row, index) {
      const id = row.id
      this.$modal
        .confirm('确定删除该物料？', '确定删除')
        .then(() => {
          // this.tableData.forEach((item, index) => {
          //   if (item.id === id) {
          this.tableData.splice(index, 1)
          // }
          // })
          this.$message.success('删除成功')
          if (id) {
            this.delDetailIdList.push(id)
          }

          this.$emit('submitData', this.tableData, this.delDetailIdList)
        })
        .catch(() => {})
    },
    submitForm() {
      this.$refs.formRef.submitForm()
    },
    /** 监听子组件更新事件 */
    updateData(data, index, fileList) {
      // const arr = []
      // const obj = {
      //   materialId: data.materialId,
      //   expectedCompletionDate: data.expectedCompletionDate,
      //   planQty: data.planQty,
      //   supplierName: data.supplierName,
      //   remark: data.remark,
      //   id: data.id
      // }
      // this.tableData[index] = data
      // if (fileList?.length > 0) {
      //   this.tableData[index].fileList = fileList
      // }
      // if (this.tableData?.length > 0) {
      //   this.tableData.forEach((item, index) => {
      //     if (item.id === data.id) {
      //       // arr.push(obj)
      //       item = data
      //     }
      //   })
      //   if (!data.id) {
      //     this.tableData.push(data)
      //     arr.push(...this.tableData)
      //   }
      // } else {
      //   arr.push(data)
      // }
      this.tableData[index] = data
      this.tableData[index].fileList = fileList
      this.open = false
      this.$modal.msgSuccess('保存成功')
      this.$emit('submitData', this.tableData)
    },
    /** 增加明细 */
    addData(data, fileList) {
      data.fileList = fileList
      const arr = this.tableData.map(e => {
        return e.materialId
      })
      if (arr.includes(data.materialId)) {
        return this.$message.error('已经选择过该物料')
      }
      this.tableData.push(data)
      this.open = false
      this.$modal.msgSuccess('保存成功')
      this.$emit('submitData', this.tableData)
    },
    /** 附件弹窗 */
    showFileDig(index, fileList) {
      this.index = index
      this.fileDig = true
      this.fileList = fileList.concat()
    },
    closeFileDig() {
      this.fileDig = false
    },
    delFileList(row) {
      const index = this.fileList.findIndex(e => {
        return e.id === row.id
      })

      this.fileList.splice(index, 1)
    },
    // 文件列表关键字搜索
    keySearch(name) {
      const result = this.tableData[this.index].fileList
      if (name === '') {
        this.fileList = result
      } else {
        const lastResult = result.filter(item => item.name.toString().indexOf(name) >= 0)
        this.fileList = lastResult
      }
    },
    batchDownload() {
      this.$refs.editFile.batchDownload()
    },
    // 批量删除
    batchDel() {
      const arr = this.$refs.editFile.selectedFiles
      if (arr.length === 0) {
        return this.$message.error('请先选择文件')
      }
      arr.forEach(e => {
        this.delFileList(e)
      })
    },
    // 保存文件
    saveFile() {
      this.tableData[this.index].fileList = this.fileList
      this.fileDig = false
    }
  }
}
</script>

<style lang="scss" scoped>
.btnClass{
  // position: absolute;
  // right: 36px;
  // float: right;
}
.materialDetail{
  width: 100%;
  background:#ffffff;
  ::v-deep .el-table--scrollable-x .el-table__body-wrapper{
    overflow: auto;
  }
  ::v-deep .el-table__fixed-footer-wrapper tbody td.el-table__cell{
    display: none;
  }
}
.totalDiv{
  width:100%;
  border:1px solid #ddd;
  border-top:none;
  height:53px;
  display:flex;
  font-size:14px;
  .total_title{
    width:170px;
    background:#F1F2F5;
    height:100%;
    text-align:center;
    line-height:53px;
    border-right:1px solid #ddd
  }
  .total_count{
    height:100%;
    text-align:center;
    line-height:53px;
    padding-left:20px
  }
}
::v-deep .el-input__icon.el-icon-date{
  display: none;
}
::v-deep .el-input--prefix .el-input__inner{
  padding-left:15px;
}
::v-deep .el-date-editor.el-input{
  width: 100%;
}
.addDetail{
  ::v-deep .el-dialog__body{
    padding: 0;
    max-height: 542px;
    overflow-y: auto;
  }
}
.el-table {
  ::v-deep .el-table__body-wrapper {
    z-index: 200;
  }

}
.selectPlan1{
  ::v-deep .el-dialog .el-dialog__body{
    padding-top: 0;
    max-height: 542px;
    overflow-y: auto;
    // padding-bottom: 0;
  }
}
</style>
