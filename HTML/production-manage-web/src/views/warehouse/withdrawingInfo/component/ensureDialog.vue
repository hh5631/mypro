<template>
  <div>
    <!-- 基本信息 -->
    <collapse title="基本信息">
      <el-form
        ref="form"
        :validate-on-rule-change="false"
        :model="form"
        :rules="isEdit ? rules : {}"
        label-width="100px"
        label-position="top"
      >
        <el-row class="form-row" :gutter="40">
          <el-col :span="6">
            <el-form-item label="退库单号" prop="orderNo">
              <el-input
                v-if="isEdit"
                v-model="form.orderNo"
                size="small"
                disabled
                placeholder="系统自动生成"
                class="input-width"
              />
              <el-input
                v-else
                v-model="form.orderNo"
                size="small"
                :disabled="isDetail"
                class="input-width"
              />
            </el-form-item>
            <el-form-item label="退库人" prop="userName">
              <el-input
                v-model.trim="form.userName"
                size="small"
                disabled
                class="input-width"
              />
            </el-form-item>
          </el-col>
          <el-col :span="6">
            <el-form-item label="退库标题" prop="title">
              <el-input
                v-model="form.title"
                disabled
                size="small"
                clearable
                maxlength="20"
                placeholder="请输入"
                class="input-width"
              />
            </el-form-item>
            <el-form-item label="接收部门" prop="backDept">
              <el-select v-model="form.backDept" :class="{'noIcon2':!isEdit}" style="width: 250px;" :disabled="isDetail" clearable placeholder="请选择" size="small">
                <el-option
                  :key="form.deptId"
                  :label="form.deptName"
                  hidden
                  :value="form.deptId"
                />
                <el-input
                  v-model="filterText"
                  style="width: 200px;margin-left: 25px;padding: 10px 0;"
                  placeholder="请输入关键词"
                  prefix-icon="el-icon-search"
                  type="text"
                  class="search"
                  clearable
                  maxlength="15"
                />
                <el-tree
                  ref="treeSelect"
                  class="filter-tree"
                  :data="deptData"
                  :props="defaultProps"
                  node-key="deptId"
                  :expand-on-click-node="false"
                  :check-on-click-node="true"
                  :default-expanded-keys="expandedArr"
                  :filter-node-method="filterNode"
                  @node-click="handleNodeClick"
                />
              </el-select>
            </el-form-item>
          </el-col>
          <el-col :span="6">
            <el-form-item label="创建时间" prop="createTime">
              <el-date-picker v-model="form.createTime" type="date" style="width: 250px;" size="small" :class="{'noIcon2':isDisabled}" disabled value-format="yyyy-MM-dd" class="input-width" />
            </el-form-item>
            <el-form-item ref="warehouseRef" label="存放位置" prop="warehouse">
              <el-select
                v-if="isEdit"
                key="warehouse"
                v-model="form.warehouse"
                style="width: 250px;"
                placeholder="请选择"
                :disabled="!isEdit"
                clearable
                size="small"
                class="input-width"
                @change="getWarehouse"
              >
                <el-option
                  v-for="item in warehouseData"
                  :key="item.id"
                  :label="item.name"
                  :value="item.id"
                />
              </el-select>
              <el-input
                v-else
                v-model.trim="form.warehouse"
                size="small"
                disabled
                class="input-width"
              />
            </el-form-item>
          </el-col>
          <el-col :span="6">
            <el-form-item label="退库说明" prop="note">
              <el-input
                v-model.trim="form.note"
                type="textarea"
                :placeholder="isEdit?'请输入':''"
                :rows="6"
                :maxlength="250"
                :disabled="isDetail"
                class="input-width"
              />
            </el-form-item>
          </el-col>
        </el-row>
      </el-form>
    </collapse>
    <!-- 明细信息 -->
    <collapse title="资产明细">
      <el-table
        ref="detailTable"
        style="width: 100%"
        border
        :data="tableDatas"
        highlight-current-row
      >
        <el-table-column type="index" label="序号" width="50" align="center" />
        <el-table-column prop="internalCode" label="资产编码" width="120" align="center" />
        <el-table-column prop="name" label="资产名称" align="center" />
        <el-table-column prop="brandName" label="品牌" align="center" />
        <el-table-column prop="standard" label="规格型号" align="center" />
        <el-table-column prop="unit" label="计量单位" align="center" />
        <el-table-column prop="deptName" label="使用部门" align="center" />
        <el-table-column prop="warehouseName" label="出库仓库" align="center" />
        <el-table-column v-if="!isEdit" prop="note" label="备注" align="center" />
      </el-table>
    </collapse>
    <!-- 相关附件新增/编辑时显示-->
    <collapse title="相关附件">
      <attachment-upload v-model="form.fileList" :readonly="isDetail" />
    </collapse>
    <!-- 操作日志 新增/编辑时不显示-->
    <collapse v-if="!isEdit" title="操作日志">
      <div>
        <el-table
          ref="logList"
          style="width: 100%"
          border
          :data="logList"
          highlight-current-row
          max-height="200"
        >
          <el-table-column type="index" fixed label="序号" width="50" />
          <el-table-column
            prop="operator"
            label="操作人"
            width="120"
            align="center"
          />
          <el-table-column prop="operation" label="操作描述" align="center" />
          <el-table-column
            prop="operationTime"
            label="操作时间"
            width="160"
            align="center"
          />
        </el-table>
      </div>
    </collapse>
    <!-- <el-dialog title="预览" :visible.sync="fileDig" append-to-body width="1200">
      <filePreview :src="url" />
    </el-dialog> -->
  </div>
</template>
<script>
import collapse from '@/views/basicdata/material/component/collapse'
import { ensureWareBack } from '@/api/warehouse/index.js'
// import { getRealPath } from '@/api/system/unit'
// import filePreview from '@/components/FilePreview/index'
import AttachmentUpload from '@/components/AttachmentUpload'
export default {
  name: 'EnsureDialog',
  components: { collapse, AttachmentUpload },
  props: {
    data: {
      type: Object,
      default: () => {
        return {}
      }
    },
    warehouseData: {
      type: Array,
      default: () => []
    },
    deptData: {
      type: Array,
      default: () => []
    },
    isEdit: {
      type: Boolean,
      default: false
    },
    isDisabled: {
      type: Boolean,
      default: false
    },
    isDetail: {
      type: Boolean,
      default: false
    },
    logList: {
      type: Array,
      default: () => []
    }
  },
  data() {
    return {
      filterText: '',
      // 左侧树形组件默认参数
      defaultProps: {
        children: 'children',
        label: 'deptName'
      },
      expandedArr: [],
      rules: {
        backDept: [
          { required: true, message: '请选择接收部门', trigger: ['change'] }
        ],
        warehouse: [
          { required: true, message: '请选择存放位置', trigger: ['blur'] }
        ]
      },
      selectLeaderDig: false,
      tempPeopleObj: {
        realName: '',
        userId: '',
        deptId: '',
        deptName: ''
      },
      // 选择明细弹框显示隐藏
      openDetail: false,
      selectedData: [],
      materialKeyWords: '',
      loading: false,
      detailData: [],
      fileDig: false,
      url: ''
    }
  },
  computed: {
    form() {
      this.$set(this.data, 'fileList', [])
      return this.data
    },
    tableDatas() {
      return this.data.wareBackDetails
    }
  },
  watch: {
    filterText(val) {
      this.$refs.treeSelect.filter(val)
    }
  },
  created() {
    this.deptData.forEach(item => {
      this.expandedArr.push(item.deptId)
    })
  },
  methods: {
    /** 过滤 */
    filterNode(value, data) {
      if (!value) return true
      return data.deptName.indexOf(value) !== -1
    },
    /** 上级选择 */
    handleNodeClick(data) {
      this.form.backDeptId = data.deptId
      this.form.backDept = data.deptName
    },
    /** 最终提交表单 */
    submit(index) {
      this.$refs['form'].validate(valid => {
        if (valid) {
          const params = {
            id: this.form.id,
            warehouseId: this.form.warehouseId,
            backDeptId: this.form.backDeptId
          }
          const data = this.form.fileList.map(item => {
            return {
              ...item
            }
          })
          // console.log(data)
          ensureWareBack(params, JSON.stringify(data)).then(
            res => {
              if (res.code === 200) {
                this.$emit('successwork', res)
              } else {
                this.$emit('failwork', res)
              }
            }
          )
        }
      })
    },
    getWarehouse(data) {
      this.$refs.form.validateField('warehouse')
      this.warehouseData.forEach(item => {
        if (data === item.id) {
          this.form.warehouseId = item.id
          this.form.warehouse = item.name
        }
      })
    },
    /** 重置下拉框验证 */
    reset() {
      this.$refs.warehouseRef.clearValidate()
    }
  }
}
</script>

<style lang="scss" scoped>
// 基础信息
.form-row {
  padding: 0 20px;
}
.file-list {
  display: flex;
  justify-content: space-between;
  margin-top: 10px;
}

.noIcon2 {
  ::v-deep .el-input__icon.el-icon-date {
    display: none;
  }
  ::v-deep .el-input__inner {
    padding-left: 16px;
  }
  ::v-deep .el-input__suffix {
    display: none;
  }
}
</style>
