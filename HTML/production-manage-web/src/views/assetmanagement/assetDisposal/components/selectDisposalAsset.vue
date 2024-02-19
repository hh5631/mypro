<template>
  <div>
    <!-- 选择资产 -->
    <el-dialog
      title="选择资产"
      :visible.sync="openDetail"
      width="1200px"
      append-to-body
      :close-on-click-modal="false"
      :before-close="closeDetailDig"
    >
      <el-form
        ref="queryForm"
        :model="queryParams"
        :inline="true"
        label-width="68px"
        class="searchForm"
      >
        <el-form-item label="资产名称" prop="name">
          <el-input
            v-model.trim="queryParams.name"
            placeholder="请输入"
            clearable
            size="small"
            style="width: 200px"
            maxlength="15"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item label="资产编码" prop="internalCode">
          <el-input
            v-model.trim="queryParams.internalCode"
            placeholder="请输入"
            clearable
            size="small"
            style="width: 200px"
            maxlength="20"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item label="资产状态" prop="status">
          <el-select
            v-model="queryParams.status"
            style="width: 130px"
            placeholder="请选择"
            clearable
            size="small"
          >
            <el-option
              v-for="item in assetsStatus"
              :key="item.value"
              :label="item.label"
              :value="item.value"
            />
          </el-select>
        </el-form-item>
        <el-form-item>
          <el-button size="small" @click="resetQuery">重置</el-button>
          <el-button
            size="small"
            class="search-btn"
            @click="handleQuery"
          >搜索</el-button>
        </el-form-item>
      </el-form>
      <el-table
        ref="table"
        v-loading="detailLoading"
        highlight-current-row
        :data="detailData"
        border
        style="margin-top: 10px"
        max-height="500"
        :row-key="getRowKeys"
        @selection-change="handleSelectionDetail"
      >
        <el-table-column type="selection" align="center" />
        <el-table-column label="序号" type="index" width="50px" />
        <el-table-column
          key="internalCode"
          label="资产编码"
          align="center"
          prop="internalCode"
          width="180"
          :show-overflow-tooltip="true"
        />
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
        <!-- <el-table-column
          key="price"
          label="单价"
          align="center"
          prop="price"
          :show-overflow-tooltip="true"
        /> -->
        <el-table-column
          key="unit"
          label="计量单位"
          align="center"
          prop="unit"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          key="purchaseDate"
          label="购买日期"
          align="center"
          prop="purchaseDate"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          key="status"
          label="状态"
          align="center"
          prop="status"
          :show-overflow-tooltip="true"
        >
          <template slot-scope="scope">
            <span>{{ filterStatus(scope.row.status) }}</span>
          </template>
        </el-table-column>
        <el-table-column
          key="price"
          width="120"
          label="购置单价（元）"
          align="center"
          prop="price"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          key="serviceLife "
          label="使用年限"
          align="center"
          prop="serviceLife"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          key="usedMonth"
          label="已使用月份"
          align="center"
          prop="usedMonth"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          key="netResidualValue"
          label="净值"
          align="center"
          prop="netResidualValue"
          :show-overflow-tooltip="true"
        />
      </el-table>
      <div class="common-pagination">
        <pagination
          :total="total"
          :page.sync="queryParams.pageNum"
          :limit.sync="queryParams.pageSize"
          @pagination="getList"
        />
      </div>
      <div slot="footer" class="dialog-footer">
        <el-button size="small" @click="closeDetailDig">取消</el-button>
        <el-button
          size="small"
          type="primary"
          @click="submitAdd"
        >确认</el-button>
      </div>
    </el-dialog>
  </div>
</template>

<script>
import { getCanHandleAssetCard } from '@/api/assetmanagement/assetsParameter'
export default {
  name: 'SelectDisposalAsset',
  props: {
    openDetail: { type: Boolean, default: false }
  },
  data() {
    return {
      detailLoading: false,
      // 右侧头部表单查询参数
      queryParams: {
        pageNum: 0,
        pageSize: 10,
        status: undefined,
        name: undefined,
        code: undefined,
        queryType: 0
      },
      total: 0,
      detailData: [],
      selectedData: [],
      // 资产状态只保留闲置、在用
      assetsStatus: [
        {
          value: '0',
          label: '闲置'
        }, {
          value: '3',
          label: '在用'
        }]
    }
  },
  created() {
  },
  methods: {
    // 初始化
    init() {
      this.queryParams = {
        pageNum: 0,
        pageSize: 10,
        status: undefined,
        name: undefined,
        internalCode: undefined,
        queryType: 0
      }
      this.getList()
    },
    /** 获取列表数据 */
    // console.log();
    getList() {
      this.detailLoading = true
      getCanHandleAssetCard(this.addDateRange(this.queryParams)).then(
        (res) => {
          this.detailLoading = false
          if (res.code === 200) {
            this.detailData = res.data.records
            this.total = res.data.total
          } else {
            this.$message.error(res.msg)
          }
        }
      )
    },
    /** 弹窗关闭 */
    closeDetailDig() {
      this.$emit('closeDetailDig', false)
    },
    getRowKeys (row) {
      return row.id + Math.random()
    },
    /** 选择资产 */
    handleSelectionDetail(selection) {
      this.selectedData = selection
    },
    /** 确认选择物品 */
    submitAdd () {
      this.closeDetailDig()
      this.$emit('selected', this.selectedData)
    },
    /** 搜索 */
    handleQuery() {
      this.getList()
    },
    /** 重置 */
    resetQuery() {
      this.queryParams = {
        pageNum: 0,
        pageSize: 10,
        status: undefined,
        name: undefined,
        internalCode: undefined,
        queryType: 0
      }
      this.getList()
    },
    /** 转换状态 */
    filterStatus(status) {
      const map = new Map([
        ['0', '闲置'],
        ['3', '在用'],
        ['4', '变卖'],
        ['5', '报废'],
        ['6', '捐赠'],
        ['7', '盘亏'],
        ['100', '其他']
      ])
      return map.get(status)
    }
  }
}
</script>

<style lang="scss" scoped>

</style>
