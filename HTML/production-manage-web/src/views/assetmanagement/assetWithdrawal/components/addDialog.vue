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
                :disabled="isDisabled"
                class="input-width"
              />
            </el-form-item>
            <el-form-item label="退库人" prop="userName">
              <el-input
                v-model="form.userName"
                disabled
                style="width: 250px;"
              />
            </el-form-item>
          </el-col>
          <el-col :span="6">
            <el-form-item label="退库标题" prop="title">
              <el-input
                v-model="form.title"
                :disabled="isDisabled"
                size="small"
                clearable
                maxlength="20"
                placeholder="请输入"
                class="input-width"
              />
            </el-form-item>
            <!-- <el-form-item label="接收部门" prop="backDeptId">
              <el-select
                v-model="form.backDeptId"
                style="width: 250px;"
                :placeholder="isEdit?'默认账号所属单位':''"
                clearable
                :disabled="isDisabled"
                size="small"
                class="input-width"
              >
                <el-option
                  v-for="dict in deptData"
                  :key="dict.deptId"
                  :label="dict.deptName"
                  :value="dict.deptId"
                />
              </el-select>
            </el-form-item> -->
            <el-form-item label="接收部门" prop="backDept">
              <el-select v-if="isEdit" v-model="form.backDept" style="width: 250px;" :disabled="isDisabled" placeholder="请选择" size="small">
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
              <el-input
                v-else
                v-model.trim="form.backDept"
                size="small"
                disabled
                class="input-width"
              />
            </el-form-item>
          </el-col>
          <el-col :span="6">
            <el-form-item label="创建时间" prop="createTime">
              <el-date-picker v-model="form.createTime" type="date" style="width: 250px;" size="small" disabled value-format="yyyy-MM-dd" :placeholder="addTimeNow" class="input-width noIcon2" />
            </el-form-item>
            <el-form-item label="存放位置" prop="warehouse">
              <el-select
                v-if="isEdit"
                v-model="form.warehouseId"
                style="width: 250px;"
                placeholder="请选择"
                clearable
                size="small"
                class="input-width"
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
                :disabled="isDisabled"
                class="input-width"
              />
            </el-form-item>
          </el-col>
        </el-row>
      </el-form>
    </collapse>
    <!-- 明细信息 -->
    <collapse title="资产明细">
      <div>
        <div class="addOrRemove">
          <el-button v-if="isEdit" size="small" @click="addDetail">
            <i class="iconfont icon-zengjia" style="cursor: pointer" />
            添加明细
          </el-button>
        </div>
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
          <el-table-column prop="dept" label="使用部门" align="center" />
          <el-table-column prop="warehouseName" label="出库仓库" align="center" />
          <el-table-column
            v-if="isEdit"
            label="操作"
            width="100"
          >
            <template slot-scope="scope">
              <el-button
                type="text"
                size="small"
                @click.native.prevent="deleteRow(scope.$index, tableDatas)"
              >删除</el-button>
            </template>
          </el-table-column>
        </el-table>
      </div>
    </collapse>
    <!-- 操作日志 新增/编辑时不显示-->
    <collapse v-if="!isEdit" title="审批流程">
      <ApprovalProcess :id="form.id" ref="approvalProcess" type="type_ware_back" />
    </collapse>
    <!-- 添加明细 -->
    <el-dialog
      destroy-on-close
      title="添加明细"
      :visible.sync="openDetail"
      width="1200px"
      append-to-body
      :close-on-click-modal="false"
      @close="closeDetailDig"
    >
      <div class="selectLeader">
        <div class="search">
          <el-input
            v-model.trim="materialKeyWords"
            style="width: 40%"
            maxlength="15"
            placeholder="请输入资产名称或编码"
            clearable
            @keyup.enter.native="handleDetailSearch"
          />
          <el-button size="small" style="margin-left: 16px" @click="resetDetailQuery">重置</el-button>
          <el-button
            size="small"
            style="margin-left: 16px"
            class="search-btn"
            @click="handleDetailSearch"
          >搜索</el-button>
        </div>
        <el-table
          ref="table"
          v-loading="loading"
          highlight-current-row
          :data="detailData"
          style="margin-top: 10px"
          max-height="500"
          :row-key="getRowKeys"
          @selection-change="handleSelectionDetail"
        >
          <el-table-column
            type="selection"
            width="50"
            :reserve-selection="true"
            align="center"
          />
          <el-table-column label="序号" type="index" align="center" />
          <el-table-column key="internalCode" label="资产编码" align="center" prop="internalCode" :show-overflow-tooltip="true" />
          <el-table-column label="资产类别" align="center" prop="category" :show-overflow-tooltip="true" />
          <el-table-column
            key="name"
            label="资产名称"
            align="center"
            prop="name"
            :show-overflow-tooltip="true"
          />
          <el-table-column
            key="brandName"
            label="品牌"
            align="center"
            prop="brandName"
            :show-overflow-tooltip="true"
          />
          <el-table-column
            key="standard"
            label="规格型号"
            align="center"
            prop="standard"
            :show-overflow-tooltip="true"
          />
          <el-table-column
            key="unit"
            label="计量单位"
            align="center"
            prop="unit"
            :show-overflow-tooltip="true"
          />
          <el-table-column
            key="status"
            label="资产状态"
            align="center"
            prop="status"
            :show-overflow-tooltip="true"
          >
            <template slot-scope="scope">
              <span>{{ getAssetStatus(scope.row) }}</span>
            </template>
          </el-table-column>
          <el-table-column
            key="warehouseName"
            label="出库仓库"
            align="center"
            prop="warehouseName"
            :show-overflow-tooltip="true"
          />
        </el-table>
      </div>
      <div slot="footer" class="dialog-footer">
        <el-button size="small" @click="closeAddDig">取消</el-button>
        <el-button
          size="small"
          type="primary"
          @click="submitAdd"
        >保存</el-button>
      </div>
    </el-dialog>
  </div>
</template>
<script>
import collapse from '@/views/basicdata/material/component/collapse'
import ApprovalProcess from '@/views/assetmanagement/approvalProcess/approvalProcess.vue'
import { getAssetByCurrentUser, addOrUpdateAssetBack } from '@/api/assetmanagement/assetWithdrawal.js'
export default {
  name: 'AddDialog',
  components: { collapse, ApprovalProcess },
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
    addTimeNow: {
      type: String,
      default: ''
    }
  },
  data() {
    return {
      rules: {
        title: [
          { required: true, message: '请输入标题', trigger: ['blur', 'change'] }
        ],
        backDeptId: [
          { required: true, message: '请选择接收部门', trigger: ['select'] }
        ]
      },
      // selectLeaderDig: false,
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
      filterText: '',
      // 左侧树形组件默认参数
      defaultProps: {
        children: 'children',
        label: 'deptName'
      },
      expandedArr: []
    }
  },
  computed: {
    form() {
      return this.data
    },
    tableDatas() {
      return this.data.wareBackDetails
    }
  },
  watch: {
    filterText(val) {
      this.$refs.treeSelect.filter(val)
    },
    'form.userName': {
      // 组件属性变动后，立即保存表单JSON！！
      deep: true,
      handler(newVal, oldVal) {
        this.form.userName = newVal || this.$store.state.user.realName
        this.form.userId = this.$store.state.user.userId
      }
    }

  },
  created() {
    this.deptData.forEach(item => {
      this.expandedArr.push(item.deptId)
    })
    // console.log(this.expandedArr)
  },
  methods: {
    /** 添加明细 */
    addDetail() {
      this.selectedData = []
      this.openDetail = true
      this.loading = true
      getAssetByCurrentUser('').then(res => {
        if (res.code === 200) {
          this.detailData = res.data || []
          this.loading = false
        } else {
          this.$message.error(res.msg)
        }
      })
    },
    /** 删除明细 */
    deleteRow (index, rows) {
      rows.splice(index, 1)
    },
    /** 添加明细 */
    closeDetailDig () {
      this.openDetail = false
    },
    closeAddDig () {
      this.openDetail = false
    },
    /** 选中物品 */
    handleSelectionDetail (val) {
      this.selectedData = val
    },
    /** 确认选择物品 */
    submitAdd () {
      this.openDetail = false
      this.selectedData.forEach(item => {
        this.tableDatas.push(item)
      })
      // console.log(this.tableDatas)
    },
    getRowKeys (row) {
      return row.id + Math.random()
    },
    // 根据资产名称或编码模糊搜索
    handleDetailSearch() {
      getAssetByCurrentUser(this.materialKeyWords).then(res => {
        if (res.code === 200) {
          this.detailData = res.data || []
          this.loading = false
        } else {
          this.$message.error(res.msg)
        }
      })
    },
    resetDetailQuery() {
      this.materialKeyWords = ''
      this.addDetail()
    },
    /** 最终提交表单 */
    submit(index) {
      this.$refs['form'].validate(valid => {
        let isSubmit = true
        if (valid) {
          if (index === 0) {
            // 存草稿
            isSubmit = false
          }
          this.form.wareBackDetails = this.tableDatas.map(item => {
            return {
              assetId: item.assetId || item.id
            }
          })
          const data = {
            orderNo: this.form.orderNo,
            title: this.form.title,
            status: this.form.status,
            userId: this.form.userId,
            userName: this.form.userName,
            note: this.form.note,
            warehouse: this.form.warehouse,
            warehouseId: this.form.warehouseId,
            backDeptId: this.form.backDeptId,
            backDept: this.form.backDept,
            wareBackDetails: this.form.wareBackDetails
          }
          // 编辑
          if (this.form.id) {
            data.id = this.form.id
          }
          addOrUpdateAssetBack({ isSubmit }, JSON.stringify(data)).then(
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
    /** 重置表单 */
    reset() {
      this.$refs['form'].resetFields()
    },
    /** 获取资产状态 */
    getAssetStatus(row) {
      if (row.status === '0') {
        return '闲置'
      }
      if (row.status === '3') {
        return '在用'
      }
    },
    /** 过滤 */
    filterNode(value, data) {
      if (!value) return true
      return data.deptName.indexOf(value) !== -1
    },
    /** 上级选择 */
    handleNodeClick(data) {
      this.form.backDeptId = data.deptId
      this.form.backDept = data.deptName
    }
  }
}
</script>

<style lang="scss" scoped>
// 基础信息
.form-row {
  padding: 0 20px;
}
// 添加明细
.addOrRemove {
  display: flex;
  justify-content: flex-end;
  margin-bottom: 15px;
  margin-right: 10px;
}
.noIcon2 {
  ::v-deep .el-input__icon.el-icon-date {
    display: none;
  }
  ::v-deep .el-input__inner {
    padding-left: 16px;
  }
}

</style>
