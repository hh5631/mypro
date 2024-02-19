<template>
  <div>
    <tags-view>
      <el-row :gutter="10" class="mb8">
        <el-col :span="1.5">
          <function-btn icon="icon-daochu" name="导出" @click.native="handleExport" />
        </el-col>
        <!-- <el-col :span="1.5">
          <function-btn icon="icon-daoru" name="打印" />
        </el-col> -->
        <el-col :span="1.5">
          <el-button v-hasPermi="['system:post:add']" size="small" @click="refresh">
            <el-tooltip class="item" effect="dark" content="刷新" placement="top-start">
              <i class="iconfont icon-shuaxin no-margin" />
            </el-tooltip>
          </el-button>
        </el-col>
      </el-row>
    </tags-view>
    <el-card class="main-card">
      <el-form ref="queryForm" :model="queryParams" :inline="true" label-width="80">
        <el-form-item label="物料编码" prop="state">
          <el-input
            v-model.trim="queryParams.materialCode"
            placeholder="请输入"
            clearable
            size="small"
            style="width: 200px"
            maxlength="15"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item label="物料名称" prop="orderNo">
          <el-input
            v-model.trim="queryParams.materialName"
            placeholder="请输入"
            clearable
            size="small"
            style="width: 200px"
            maxlength="15"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item label="存放仓库" prop="title">
          <el-select v-model="queryParams.warehouseId" clearable filterable placeholder="请选择">
            <el-option v-for="item in warehouseData" :key="item.id" :label="item.name" :value="item.id" />
          </el-select>
        </el-form-item>
        <el-form-item label="退库时间">
          <div class="date_box">
            <el-date-picker
              v-model="dateRange"
              size="small"
              style="width: 200px"
              value-format="yyyy-MM-dd"
              type="daterange"
              range-separator="至"
              start-placeholder="开始日期"
              end-placeholder="结束日期"
              :clearable="false"
            />
            <i class="el-icon-date data_icon" />
          </div>
        </el-form-item>
        <el-form-item>
          <el-button size="small" @click="resetQuery">重置</el-button>
          <el-button size="small" class="search-btn" type="primary" @click="handleQuery">搜索</el-button>
        </el-form-item>
      </el-form>
      <el-table
        ref="multipleTable"
        v-loading="tableLoading"
        :data="tableData"
        height="100%"
        highlight-current-row
        @selection-change="handleSelectionChange"
        @cell-click="changeSafeNum"
      >
        <el-table-column type="selection" align="left" />
        <el-table-column label="序号" type="index" prop="index" sortable align="left">
          <template #default="scope">
            <span>{{
              (queryParams.pageNum - 1) * queryParams.pageSize +
                scope.$index +
                1 }}
            </span>
          </template>
        </el-table-column>
        <el-table-column
          v-for="(table, index) in colums"
          :key="index"
          :label="table.label"
          :align="table.align"
          :prop="table.prop"
          :show-overflow-tooltip="true"
        >
          <template slot-scope="scope">
            <span v-if="index === 4" :class="scope.row.num < scope.row.safeNum ? 'num-red' : ''">{{ scope.row.num }}
            </span>
            <span v-else> {{ scope.row[table.prop] }}</span>
          </template>
        </el-table-column>
        <el-table-column label="操作" align="left" width="160" class-name="small-padding">
          <template slot-scope="scope">
            <el-button size="small" type="text" @click="handleDetail(scope.row, 0)">详情</el-button>
          </template>
        </el-table-column>
      </el-table>
      <!-- 分页 -->
      <!-- 分页 -->
      <div class="common-pagination">
        <!-- <div class="chooseCount">已选{{ selectedLength }}条</div> -->
        <pagination
          v-show="total > 0"
          :total="total"
          :page.sync="queryParams.pageNum"
          :limit.sync="queryParams.pageSize"
          @pagination="getList"
        />
      </div>
    </el-card>
    <stock-detail ref="detail" />
    <!-- 安全库存弹框 -->
    <!-- <template v-if="safeNumDialog"> -->
    <el-dialog title="安全库存" :visible.sync="safeNumDialog" :close-on-click-modal="false" width="25%">
      <el-form ref="safeNumForm" :rules="safeRules" :model="safeNumForm">
        <el-form-item label="安全库存" prop="safeNum">
          <el-input v-model.number="safeNumForm.safeNum" autocomplete="off" placeholder="请输入" style="width: 200px" />
        </el-form-item>
      </el-form>
      <div slot="footer">
        <el-button @click="safeNumDialog = false">取 消</el-button>
        <el-button type="primary" @click="saveSafeNum('safeNumForm')">保存</el-button>
      </div>
    </el-dialog>
    <!-- </template> -->
  </div>
</template>
<script>
import TagsView from '../../../layout/components/TagsView/index.vue'
import { getStockInfoList, changeSafeNum } from '@/api/warehouse/index'
import { listWarehouse } from '@/api/basicdata/warehouse'
import FunctionBtn from '@/components/FunctionBtn/index.vue'
import { jriMixin } from '@/mixins/jriMixin.js'
import stockDetail from './component/detail.vue'
// import { download } from '@/utils/request'
export default {
  name: 'InventoryParameterPage',
  components: { TagsView, FunctionBtn, stockDetail },
  dicts: ['procurement_plan_status'],
  mixins: [jriMixin],
  beforeRouteLeave(to, from, next) {
    this.$store.dispatch('settings/changeSetting', {
      key: 'showTags',
      value: true
    })
    next()
  },
  data() {
    var validateSafeNum = (rule, value, callback) => {
      if (!(/(^[0-9]\d*$)/.test(value))) {
        callback(new Error('请输入正整数'))
      } else {
        callback()
      }
    }
    return {
      selectedLength: 0,
      total: 0,
      queryParams: {
        pageNum: 1,
        pageSize: 10,
        materialCode: undefined,
        materialName: undefined,
        warehouseId: undefined
      },
      detailVisible: false,
      colums: [
        { prop: 'materialCode', label: '物料编码', align: 'left' },
        { prop: 'materialName', label: '物料名称', align: 'left' },
        { prop: 'brand', label: '品牌', align: 'left' },
        { prop: 'materialStandard', label: '规格型号', align: 'left' },
        { prop: 'num', label: '库存数量', align: 'left' },
        // { prop: 'num', label: '安全库存', align: 'left' },
        { prop: 'safeNum', label: '安全库存', align: 'left' },
        { prop: 'unit', label: '计量单位', align: 'left' },
        { prop: 'warehouseName', label: '存放仓库', align: 'left' }
      ],
      dateRange: [],
      checkAll: true,
      checkedColum: ['计划编号', '标题', '预期完成时间', '资产名称', '创建人', '创建时间', '状态'],
      tableColum: ['采购单编号', '申请标题', '采购负责人', '申请时间', '供应商'],
      isIndeterminate: false,
      tableData: [],
      warehouseData: [],
      tableLoading: false,
      ids: [],
      safeNumDialog: false,
      safeNumForm: {
        safeNum: null,
        id: null
      },
      safeRules: {
        safeNum: [
          { validator: validateSafeNum, trigger: 'blur' }
        ]
      },
      numClassFlog: false
    }
  },
  watch: {
    tableData: {
      handler() {
        this.$refs.multipleTable.bodyWrapper.scrollTop = 0
      },
      deep: true
    }
  },
  // activated() {
  //   this.getList()
  //   this.$store.dispatch('settings/changeSetting', {
  //     key: 'showTags',
  //     value: false
  //   })
  //   // const ele = document.getElementById('tags-title-name')
  //   // ele.parentNode.replaceChild(this.$refs.headBar.$el, ele)
  // },
  created() {
    this.getWarehouse()
    this.getList()
    this.$store.dispatch('settings/changeSetting', {
      key: 'showTags',
      value: false
    })
  },
  methods: {
    /* 获取仓库信息 */
    getWarehouse() {
      listWarehouse({ pageSize: 999 }).then(response => {
        this.warehouseData = response.data.records
      })
    },
    /** 获取库存台账列表数据 */
    getList() {
      this.tableLoading = true
      getStockInfoList(this.queryParams).then(res => {
        this.tableData = res.data.records
        this.total = res.data.total
        this.tableLoading = false
        // this.tableData.map(item => item.safeNum = 80)
      })
    },
    refresh() {
      this.getList()
    },
    /* 导出 */
    handleExport() {
      this.download(
        '/biz/stockInfo/export/',
        this.ids,
        `库存台账_${this.parseTime(new Date(), '{y}-{m}-{d}')}.xlsx`
      )
    },
    /* 详情 */
    handleDetail(row) {
      this.$refs.detail.open(row)
    },
    handleSelectionChange(val) {
      //
      this.ids = val.map(item => item.id)
      this.selectedLength = val.length
      this.$emit('handleSelectionChange', val)
    },
    // 修改安全库存
    changeSafeNum(row, column, cell, event) {
      if (column.label === '安全库存') {
        this.safeNumDialog = true
        this.safeNumForm.safeNum = row.safeNum
        this.safeNumForm.id = row.id
      }
    },
    async saveSafeNum(formName) {
      this.$refs[formName].validate(async (valid) => {
        if (valid) {
          try {
            await changeSafeNum({
              id: this.safeNumForm.id,
              safeNum: this.safeNumForm.safeNum
            })
            //
            this.getList()
            this.safeNumDialog = false
          } catch (err) {
            return err
          }
        } else {
          return false
        }
      })
    }
    // 库存数量地域安全库存时 样式变化
    // modality(row) {
    //    if (row.row.id == 1) {
    //     return {
    //       color: "red",
    //     };
    //   }
    // }
  }
}
</script>
<style lang="scss" scoped>
.my-table {
  padding: 8px 16px 0 16px;
}

.table-tool {
  display: flex;
  height: 100%;
  justify-content: flex-end;
  margin-right: 11px;

  .items {
    margin: auto 5px;
    width: 32px;
    height: 32px;
    background: #ffffff;
    border-radius: 4px;
    border: 1px solid #dddddd;
    display: flex;
    cursor: pointer;

    img {
      margin: auto;
    }
  }
}

.date_box {
  position: relative;
  width: fit-content;

  .data_icon {
    position: absolute;
    top: 50%;
    right: 17px;
    z-index: 9;
    color: #c0c4cc;
    font-size: 14px;
    transform: translateY(-50%);
  }

  ::v-deep .el-input__icon.el-range__icon.el-icon-date {
    display: none;
  }
}

.my-dropdown {
  margin: auto 0;
}

.my-dropdown-menu {
  padding: 6px 16px;

  ::v-deep .el-checkbox {
    line-height: 36px;
  }
}

.my-checkbox-group {
  display: flex;
  flex-flow: column;
}

.num-red {
  color: red;
}
</style>
