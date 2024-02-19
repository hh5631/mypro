<!--
 * @Descripttion:
 * @version:
 * @Author: lirioing
 * @Date: 2022-07-04 14:54:09
 * @LastEditors: liquan 1106970756@qq.com
 * @LastEditTime: 2022-09-07 13:32:13
-->
<template>
  <div class="selectLeader">
    <div class="detail-title">选择物料</div>
    <div class="search">
      <el-input
        v-model.trim="materialKeyWords"
        style="width: 40%"
        maxlength="15"
        placeholder="请输入物料名称或编码"
      />

      <!-- <div class="label">归属仓库</div> -->
      <!-- <el-select v-model="queryParams.warehouseId" placeholder="全部仓库">
        <el-option
          v-for="dict in warehouseData"
          :key="dict.id"
          :label="dict.name"
          :value="dict.id"
        />
      </el-select> -->
      <el-button size="small" @click="resetQuery">重置</el-button>
      <el-button
        size="small"
        style="margin-left: 16px"
        class="search-btn"
        @click="search"
      >搜索</el-button>
    </div>
    <div>
      <el-table
        ref="table"
        v-loading="loading"
        highlight-current-row
        :data="tableData"
        style="margin-top: 10px"
        max-height="300"
        :row-key="getRowKeys"
        @selection-change="handleSelectionChange"
      >
        <el-table-column
          type="selection"
          width="50"
          :reserve-selection="true"
          align="center"
        />
        <!-- <el-table-column type="index" width="100" label="序号" /> -->
        <el-table-column
          key="materialCode"
          label="物料编号"
          align="center"
          prop="materialCode"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          key="materialName"
          label="物料名称"
          align="center"
          prop="materialName"
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
          key="useNum"
          label="可生成资产卡片数"
          align="center"
          prop="useNum"
          :show-overflow-tooltip="true"
        >
          <template slot-scope="scope">
            <div>
              {{ scope.row.useNum ? scope.row.useNum : 0 }}
            </div>
          </template>
        </el-table-column>
        <el-table-column
          key="unitName"
          label="计量单位"
          align="center"
          prop="unitName"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          key="warehouseName"
          label="存放仓库"
          align="center"
          prop="warehouseName"
          :show-overflow-tooltip="true"
        />
      </el-table>
    </div>

    <div class="asset-detail">
      <div class="detail-title sub-title">物料明细</div>
      <el-table
        ref="selectedData"
        style="width: 100%"
        border
        :data="selectedData"
        highlight-current-row
        max-height="200"
        :header-cell-class-name="cellClass"
      >
        <!-- <el-table-column type="index" width="100" label="序号" /> -->
        <el-table-column
          key="materialCode"
          label="物料编码"
          align="center"
          prop="materialCode"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          key="materialName"
          label="物料名称"
          align="center"
          prop="materialName"
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
          key="useNum"
          label="可生成资产卡片数"
          align="center"
          prop="useNum"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          key="assetCardNum"
          label="生成资产卡片数量"
          align="center"
          prop="assetCardNum"
        >
          <template slot-scope="scope">
            <div>
              <el-input
                v-model.trim="scope.row.assetCardNum"
                v-trim.exceptInt="scope.row.assetCardNum"
                placeholder="请输入"
                class="noborder"
                @input="checkInput(scope.row)"
              />
            </div>
          </template>
        </el-table-column>
        <!-- <el-table-column
          key="canGetNum"
          label="可领数量"
          align="center"
          prop="canGetNum"
        >
          <template slot-scope="scope">
            <div>
              <el-input
                v-model.trim="scope.row.canGetNum"
                placeholder="请输入"
                class="noborder"
                @blur="canGetNumInput(scope.row)"
              />
            </div>
          </template>
        </el-table-column> -->
      </el-table>
    </div>
  </div>
</template>

<script>
import { listWarehouse } from '@/api/basicdata/warehouse'
import {
  getStockList,
  getStockVoById
} from '@/api/assetmanagement/assetsParameter'
export default {
  name: 'AssetsParameterAddPage',
  props: {},
  data () {
    return {
      loading: true,
      title: '',
      addApplyDig: false,
      tableData: [],
      materialKeyWords: '',
      selectedData: [], // 选中的数据
      currentTitleRow: {},
      queryParams: {
        pageNum: 0,
        pageSize: 9999
      },
      warehouseData: []
    }
  },
  created () {
    // this.getList()
  },
  methods: {
    getList () {
      this.loading = true
      getStockList(this.queryParams).then(
        (response) => {
          this.loading = false
          this.tableData = response.data.records
          this.tableData.map((e) => {
            // 可生成资产卡片的数量
            e.useNum = e.totalNum - e.existNum
          })
        }
      )
      /* 获取仓库信息 */
      listWarehouse({ pageSize: 999 }).then((response) => {
        this.warehouseData = response.data.records
      })
    },
    getDetail () {
      getStockVoById(this.queryParams).then((response) => {
        // 11
      })
    },
    search () {
      if (this.materialKeyWords === '') {
        // 如果搜索栏为空就执行getList方法获取最初始的列表
        this.getList()
      } else {
        // 搜索
        this.tableData = this.tableData.filter((item) => {
          return (
            item.materialName.toString().indexOf(this.materialKeyWords) >= 0 ||
            item.materialCode.toString().indexOf(this.materialKeyWords) >= 0
          )
        })
      }
    },
    handleQuery () {
      this.queryParams.pageNum = 1
      this.getList()
    },
    resetQuery () {
      this.materialKeyWords = ''
      this.handleQuery()
    },
    handleSelectionChange (val) {
      this.selectedData = JSON.parse(JSON.stringify(val))
    },
    getRowKeys (row) {
      return row.id + Math.random()
    },
    checkInput (row) {
      if (row.assetCardNum > row.useNum) {
        this.$message.error('生成资产卡片数量已超过现有库存数量')
        row.assetCardNum = ''
      }
    },
    // canGetNumInput (row) {
    //   if (row.canGetNum > row.assetCardNum) {
    //     this.$message.error('可领数量已超过生成资产卡片数量')
    //     row.canGetNum = ''
    //   }
    // },
    cellClass (row) {
      if (row.columnIndex === 5) {
        return 'must'
      }
    }
  }
}
</script>

<style lang="scss" scoped>
.selectLeader {
  width: 100%;
  height: 100%;
  padding: 16px;
  overflow-y: scroll;
}
.search {
  display: flex;
  align-items: center;
  .label {
    margin: 0 8px 0 24px;
  }
}
.detail-title {
  font-size: 16px;
  font-weight: 500;
  color: #6a7697;
  margin-bottom: 15px;
  &.sub-title {
    margin-top: 15px;
  }
}
.noborder {
  ::v-deep .el-input__inner {
    border: none;
  }
}
::v-deep .el-table th.must > .cell:after {
  content: " *";
  color: #ff1818;
}
</style>
