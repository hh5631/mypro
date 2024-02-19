<template>
  <div>
    <!-- 搜索form -->
    <el-form
      ref="queryForm"
      :model="queryParams"
      :inline="true"
      label-width="68px"
      class="searchForm"
    >
      <el-form-item label="中文名称" prop="chineseName">
        <el-input
          v-model.trim="queryParams.chineseName"
          placeholder="请输入"
          clearable
          size="small"
          style="width: 200px"
          maxlength="15"
          @keyup.enter.native="handleQuery"
        />
      </el-form-item>
      <el-form-item label="英文名称" prop="englishName">
        <el-input
          v-model.trim="queryParams.englishName"
          placeholder="请输入"
          clearable
          size="small"
          style="width: 200px"
          maxlength="15"
          @keyup.enter.native="handleQuery"
        />
      </el-form-item>
      <!-- <el-form-item label="状态" prop="enable">
        <el-select v-model="queryParams.enable" style="width: 200px" placeholder="请选择" clearable size="small">
          <el-option
            v-for="dict in dict.type.enable_status"
            :key="dict.value"
            :label="dict.label"
            :value="dict.value"
          />
        </el-select>
      </el-form-item> -->
      <el-form-item>
        <el-button size="small" @click="resetQuery">重置</el-button>
        <el-button size="small" @click="handleQuery">搜索</el-button>
      </el-form-item>
    </el-form>
    <!-- 展示table -->
    <el-table ref="multiplePlaceTable" highlight-current-row height="500" :data="placeTable" @current-change="handleSelectionChange">
      <el-table-column label="单选" align="center" width="60">
        <template slot-scope="scope">
          <el-radio v-model="radio" :label="scope.row.id" />
        </template>
      </el-table-column>
      <el-table-column label="序号" sortable align="left" prop="index" type="index" />
      <el-table-column label="中文名称" align="left" prop="chineseName" :show-overflow-tooltip="true" />
      <el-table-column label="英文名称" align="left" prop="englishName" :show-overflow-tooltip="true" />
      <el-table-column label="品牌代码" align="left" prop="code" :show-overflow-tooltip="true" />
      <el-table-column label="常见类目" align="left" prop="type" :show-overflow-tooltip="true">
        <template slot-scope="scope">
          <div>
            {{ typeObj[scope.row.type] || "" }}
          </div>
        </template>
      </el-table-column>
      <el-table-column label="品牌LOGO" align="left" prop="icon" :show-overflow-tooltip="true">
        <template slot-scope="scope">
          <div>
            <img v-if="scope.row.icon" :src="picUrl+scope.row.icon" alt="" width="50" height="50">
          </div>
        </template>
      </el-table-column>
    </el-table>
    <!-- 分页 -->
    <div>
      <pagination
        v-show="total > 0"
        :total="total"
        :page.sync="queryParams.pageNum"
        :limit.sync="queryParams.pageSize"
        @pagination="getList"
      />
    </div>
    <div slot="footer" class="dialog-footer">
      <el-button size="small" @click="cancelBrand">取 消</el-button>
      <el-button size="small" type="primary" @click="submitBrand">保 存</el-button>
    </div>
  </div>
</template>
<script>
import { listBrand } from '@/api/basicdata/brand'
export default {
  name: 'SelectBrand',
  dicts: ['enable_status'],
  data() {
    return {
      total: 0,
      // 右侧头部表单查询参数
      queryParams: {
        pageNum: 1,
        pageSize: 10,
        chineseName: undefined,
        englishName: undefined
        // enable: '0'
      },
      picUrl: process.env.VUE_APP_PIC_URL,
      placeTable: [],
      radio: '',
      selectObj: {},
      // 品牌列举枚举对象
      typeObj: {
        0: '办公家电',
        1: '信息设备',
        2: '办公家具',
        3: '专用设备'
      }
    }
  },
  created() {
    this.getList()
  },
  methods: {
    /** 右侧查询list结构 */
    getList() {
      listBrand(this.queryParams).then(
        (response) => {
          if (response.code === 200) {
            this.placeTable = response.data.records
            this.total = response.data.total
          } else {
            this.$message.error(response.msg)
          }
        }
      )
    },
    /** 选择物料时发射事件传递选择的值 */
    handleSelectionChange(value) {
      this.radio = value.id
      this.selectObj = value
    },
    /** 搜索 */
    handleQuery() {
      this.queryParams.pageNum = 1
      this.getList()
    },
    /** 重置 */
    resetQuery() {
      this.resetForm('queryForm')
      this.getList()
      this.handleQuery()
    },
    cancelBrand() {
      this.$emit('cancelBrand', false)
    },
    /** 设置供应商id*/
    submitBrand() {
      if (!this.selectObj.id) {
        this.$message.error('请先选择品牌')
      }
      this.$emit('selectBrand', this.selectObj)
    }
  }
}
</script>

<style lang="scss" scoped>

::v-deep .el-radio__label{
  display: none;
}
.dialog-footer{
  display: flex;
  justify-content: flex-end;
}
</style>
