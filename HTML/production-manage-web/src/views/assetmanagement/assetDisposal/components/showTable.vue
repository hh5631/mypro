<template>
  <div class="table-content">
    <el-table
      ref="assetTable"
      v-loading="loading"
      class="asset-disposal-table"
      highlight-current-row
      :data="assetList"
      height="90%"
      @selection-change="handleSelectionChange"
    >
      <el-table-column type="selection" width="45" align="left" />
      <el-table-column label="序号" type="index" width="50px" />
      <el-table-column label="处置单号" align="center" prop="orderNo" />
      <el-table-column label="申请人" align="center" prop="applicantName" />
      <el-table-column label="处置类型" align="center" prop="type">
        <template slot-scope="scope">
          <span>{{ getDisposalType(scope.row.type) }}</span>
        </template>
      </el-table-column>
      <el-table-column
        label="处置日期"
        align="center"
        prop="handleDate"
        :show-overflow-tooltip="true"
      >
        <template slot-scope="scope">
          <span>{{ formatDate(scope.row.handleDate) }}</span>
        </template>
      </el-table-column>
      <el-table-column label="审批状态" align="center" prop="status">
        <template slot-scope="scope">
          <el-tag :type="getStatueType(scope.row.status)" style="width: 92px;">{{
            getStatusName(scope.row.status)
          }}</el-tag>
        </template>
      </el-table-column>
      <el-table-column label="处置说明" align="center" prop="note" :show-overflow-tooltip="true" />
      <el-table-column
        label="操作"
        width="200"
        align="left"
        class-name="small-padding"
      >
        <template slot-scope="scope">
          <el-button
            size="small"
            type="text"
            style="color: #4686ef"
            @click="handleDetail(scope.row)"
          >详情</el-button>
          <el-button
            v-if="scope.row.status === '0'"
            size="small"
            type="text"
            style="color: #4686ef"
            @click="handleUpdate(scope.row)"
          >编辑</el-button>
          <el-button
            v-if="scope.row.status === '0'"
            size="small"
            type="text"
            style="color: #ef5826"
            @click="handleDelete(scope.row)"
          >删除</el-button>
        </template>
      </el-table-column>
    </el-table>
  </div>
</template>
<script>
import eventBus from '@/utils/bus.js'
import { getAssetHandleById } from '@/api/assetmanagement/assetDisposal.js'
import { dayjsFormat } from '@/utils/index.js'
export default {
  name: 'ShowTable',
  dicts: ['asset_disposal_status'],
  props: {
    loading: {
      type: Boolean,
      default: false
    },
    assetList: {
      type: Array,
      default: () => { return [] }
    }
  },
  data() {
    return {
    }
  },

  mounted() {

  },

  methods: {
    /** 多选框选中数据 */
    handleSelectionChange (selection) {
      this.$emit('selectChange', selection)
    },
    /** 根据状态显示文本值 */
    getDisposalType(value) {
      const map = new Map([
        ['0', '变卖'],
        ['1', '报废'],
        ['2', '捐赠'],
        ['4', '盘亏'],
        ['100', '其它']
      ])
      return map.get(value)
    },
    /** 根据状态取定type */
    getStatueType (value) {
      if (value === '0') {
        return 'info'
      } else if (value === '1') {
        return 'primary'
      } else if (value === '2') {
        return 'primary'
      } else if (value === '3') {
        return 'success'
      } else if (value === '4') {
        return 'danger'
      }
    },
    /** 根据状态显示文本 */
    getStatusName (value) {
      switch (value) {
        case '0':
          return '草稿'
        case '1':
          return '待审批'
        case '2':
          return '审批中'
        case '3':
          return '通过'
        case '4':
          return '不通过'
        default:
          return ''
      }
    },
    /** 详情 */
    handleDetail(row) {
      getAssetHandleById(row.id).then(res => {
        if (res.code === 200) {
          eventBus.$emit('getAssetHandleDetail', res)
        }
      })
      this.$emit('detail', row)
    },
    /** 编辑 */
    handleUpdate(row) {
      getAssetHandleById(row.id).then(res => {
        if (res.code === 200) {
          eventBus.$emit('getAssetHandleDetail', res)
        }
      })
      this.$emit('edit', row)
    },
    /** 删除 */
    handleDelete(row) {
      this.$emit('deleted', row)
    },
    /** 处置日期显示 */
    formatDate(value) {
      return dayjsFormat(value, 'YYYY-MM-DD')
    }
  }
}
</script>

<style lang="scss" scoped>
  .table-content {
    height: 100%;
  }
  .asset-disposal-table {
    margin-top: 16px;
  }
</style>
