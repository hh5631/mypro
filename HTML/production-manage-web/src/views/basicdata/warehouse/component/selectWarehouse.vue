<!--
 * @Descripttion:
 * @version:
 * @Author: lirioing
 * @Date: 2022-07-18 15:20:53
 * @LastEditors: liquan 1106970756@qq.com
 * @LastEditTime: 2022-08-18 16:09:05
-->
<template>
  <div class="selectLeader">
    <el-input v-model.trim="queryParams.name" style="width:40%" maxlength="15" placeholder="请输入关键词搜索" size="mini" />
    <el-button size="small" @click="resetQuery">重置</el-button>
    <el-button size="small" style="margin-left:16px" class="search-btn" @click="handleQuery">搜索</el-button>
    <el-table ref="warehouseTable" v-loading="loading" highlight-current-row :data="listData" style="margin-top: 10px;" @current-change="handleCurrentChange">
      <!-- <el-table-column type="selection" width="55" align="center" /> -->
      <el-table-column type="index" width="100" label="序号" />
      <el-table-column label="仓库名称" align="center" prop="name" :show-overflow-tooltip="true" />
      <el-table-column label="仓库代码" align="center" prop="code" :show-overflow-tooltip="true" />
      <el-table-column label="使用状态" align="center" prop="status">
        <template slot-scope="scope">
          <div class="statePoint">
            <span v-if="scope.row.status==='0'" style="color: #3DB954;"><img class="point" src="@/assets/images/green@2x.png">在用</span>
            <span v-else style="color:#EF5826"><img class="point" src="@/assets/images/red@2x.png">闲置</span>
          </div>
        </template>
      </el-table-column>
      <el-table-column label="仓库用途" align="center" prop="purpose">
        <template slot-scope="scope">
          <div>
            <span v-if="scope.row.purpose ==='1'">资产仓</span>
            <span v-if="scope.row.purpose === '2'">物资仓</span>
            <span v-if="scope.row.purpose === '3'">成品仓</span>
            <span v-if="scope.row.purpose === '4'">原料仓</span>
            <span v-if="scope.row.purpose === '5'">辅料仓</span>
          </div>
        </template>
      </el-table-column>
      <el-table-column label="仓库类型" align="center" prop="status">
        <template slot-scope="scope">
          <div>
            <span v-if="scope.row.type == 1">普通</span>
            <span v-if="scope.row.type == 2">冷库</span>
            <span v-if="scope.row.type == 3">特种</span>
          </div>
        </template>
      </el-table-column>
    </el-table>
    <div>
      <pagination
        v-show="total > 0"
        :total="total"
        :page.sync="queryParams.pageNum"
        :limit.sync="queryParams.pageSize"
        @pagination="getList"
      />
    </div>
    <div class="footer">
      <div>
        <el-button size="small" style="float:left" @click="add"><i class="iconfont icon-zengjia" />新增仓库</el-button>
      </div>

      <div class="footer-right">
        <el-button size="small" @click="cancel">取消</el-button>
        <el-button size="small" type="primary" @click="submitData">保存</el-button>
      </div>
    </div>
    <el-dialog :close-on-click-modal="false" title="新增仓库" :visible.sync="showWarehouseAdd" width="1100px" append-to-body class="post-dialog">
      <addWarehouse ref="addWarehouse" @addSuccess="addSuccess" @cancelAdd="cancelAdd" /></el-dialog>
  </div>
</template>

<script>
import { listWarehouse } from '@/api/basicdata/warehouse'
import addWarehouse from './addWarehouse.vue'

export default {
  components: { addWarehouse },
  data() {
    return {
      title: '',
      showWarehouseSelect: false,
      showWarehouseAdd: false,
      // 遮罩层
      loading: true,
      // 品牌表格数据
      listData: [],
      // 选中的数量
      // selectedLength: 0,
      // 选中的行信息
      currentRow: {},
      // 总条数
      total: 0,
      // 查询参数
      queryParams: {
        pageNum: 1,
        pageSize: 10,
        name: ''
      }
    }
  },
  created() {
    this.getList()
  },
  methods: {
    /** 查询列表 */
    getList() {
      this.loading = true
      listWarehouse(this.queryParams).then(response => {
        if (response.code === 200) {
          this.listData = response.data.records.filter((e) => {
            return e.enable === '0'
          })
          this.listLength = response.data.total + 1
          this.total = response.data.total
          this.loading = false
        } else {
          this.loading = false
          this.$message.error(response.msg)
        }
      })
    },
    /** 搜索按钮操作 */
    handleQuery() {
      this.queryParams.pageNum = 1
      this.getList()
    },
    /** 重置按钮操作 */
    resetQuery() {
      this.queryParams.name = ''
      this.handleQuery()
    },
    handleCurrentChange(val) {
      this.currentRow = val
    },
    // 提交选中的数据
    submitData() {
      this.$emit('handleData', this.currentRow)
    },
    // 取消
    cancel() {
      this.$emit('handleCancel')
      this.currentRow = {}
    },
    add() {
      this.showWarehouseAdd = true
      this.$nextTick(() => {
        this.$refs['addWarehouse'].reset()
      })
    },
    addSuccess() {
      this.showWarehouseAdd = false
      this.getList()
    },
    cancelAdd() {
      this.showWarehouseAdd = false
    }
  }
}
</script>

<style lang="scss" scoped>
 .statePoint .point{
    width:8px;
    height:8px;
    margin:0 8px 2px 0;
  }
.chooseCount {
    color: #7d8592;
    margin-top: 20px;
  }
.selectLeader{
  width: 100%;
  height: 100%;
  padding: 16px;
  overflow-y: scroll;
}
.footer{
  padding: 20px 0 0;
  display: flex;
  justify-content: space-between;
  &-right{
    width: 122px;
  }
}
</style>
