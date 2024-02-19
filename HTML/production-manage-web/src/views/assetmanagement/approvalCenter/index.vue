<template>
  <div class="post-box">
    <!-- header -->
    <tags-view>
      <el-row :gutter="10" class="mb8">
        <!-- 批量提交 -->
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['system:post:add']"
            type="primary"
            size="small"
            @click="handleBatchSubmission"
          >
            <span class="btn-text">批量提交</span>
          </el-button>
        </el-col>
        <!-- 刷新 -->
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['system:post:add']"
            size="small"
            @click="refresh"
          >
            <el-tooltip class="item" effect="dark" content="刷新" placement="top-start">
              <i class="iconfont icon-shuaxin no-margin" />
            </el-tooltip>
          </el-button>
        </el-col>
      </el-row>
    </tags-view>
    <!-- content -->
    <el-card class="main-card-left">
      <el-row :gutter="20">
        <!--左侧tree数据-->
        <el-col
          :span="4"
          :xs="24"
          style="border-right: 1px solid #eaedf4;padding-right: 0px;"
        >
          <!-- 搜索框 -->
          <div>
            <el-input
              v-model.trim="keyword"
              placeholder="请输入关键字搜索"
              clearable
              size="small"
              suffix-icon="el-icon-search"
              style="margin-bottom: 20px;width: calc(100% - 10px);"
              maxlength="15"
            />
          </div>
          <!-- 树形组件 -->
          <div class="left-tree">
            <el-tree
              ref="tree"
              :data="approvalOptions"
              :props="defaultProps"
              :expand-on-click-node="false"
              :filter-node-method="filterNode"
              highlight-current
              node-key="id"
              :default-expanded-keys="expandedArr"
              @node-click="handleNodeClick"
            />
          </div>
        </el-col>
        <!--右侧list数据-->
        <el-col :span="20" :xs="24" class="right-card">
          <!-- 搜索表单 -->
          <el-form ref="queryForm" :model="queryParams" :inline="true">
            <el-form-item label="流水号" prop="num">
              <el-input
                v-model.trim="queryParams.num"
                placeholder="请输入"
                clearable
                maxlength="15"
                size="small"
                style="width: 180px"
                @keyup.enter.native="handleQuery"
              />
            </el-form-item>
            <el-form-item label="选择日期">
              <div class="date_box">
                <el-date-picker
                  v-model="getTime"
                  size="small"
                  style="width: 200px"
                  value-format="yyyy-MM-dd"
                  type="daterange"
                  range-separator="至"
                  start-placeholder="开始日期"
                  end-placeholder="结束日期"
                  :clearable="false"
                /><i class="el-icon-date data_icon" />
              </div>
            </el-form-item>
            <el-form-item label="审批状态" prop="status">
              <el-select v-model="queryParams.status" style="width: 150px" placeholder="请选择" clearable size="small">
                <el-option
                  v-for="dict in dict.type.asset_approval_status"
                  :key="dict.value"
                  :label="dict.label"
                  :value="dict.value"
                />
              </el-select>
            </el-form-item>
            <el-form-item>
              <el-button size="small" @click="resetQuery">重置</el-button>
              <el-button type="primary" plain size="small" class="search-btn" @click="handleQuery">搜索</el-button>
            </el-form-item>
          </el-form>
          <!-- 状态切换按钮 -->
          <el-row class="change-type" type="flex" :gutter="24">
            <el-col :span="16">
              <el-radio-group v-model="queryParams.btnType" size="small" @change="chooseBtn">
                <el-radio-button
                  v-for="(item,index) in radioList"
                  :key="index"
                  :label="item.label"
                >{{ item.name }}</el-radio-button>
              </el-radio-group>
            </el-col>
          </el-row>
          <!-- 展示表格 -->
          <el-table ref="approvalTable" highlight-current-row :data="approvalList" height="100%" @selection-change="handleSelectionChange">
            <el-table-column type="selection" width="50" align="left" />
            <el-table-column label="序号" type="index" width="50" align="left"><template #default="scope">
              <span>{{ (queryParams.pageNum - 1) * queryParams.pageSize + scope.$index + 1 }}</span>
            </template></el-table-column>
            <el-table-column label="标题" align="left" prop="title" :show-overflow-tooltip="true" />
            <el-table-column label="流水号" width="130" align="left" prop="num" />
            <el-table-column label="当前步骤" align="left" prop="code" :show-overflow-tooltip="true" />
            <el-table-column label="创建人" align="left" prop="people" :show-overflow-tooltip="true" />
            <el-table-column label="创建时间" align="left" prop="getTime" :show-overflow-tooltip="true" />
            <el-table-column label="上节点提交人" align="left" prop="getTime" :show-overflow-tooltip="true" />
            <el-table-column label="上节点提交时间" width="120" align="left" prop="getTime" :show-overflow-tooltip="true" />
            <el-table-column label="审批状态" align="left" prop="status">
              <template slot-scope="scope">
                <el-tag :type="getStatueType(scope.row.status)">{{ getStatusName(scope.row.status) }}</el-tag>
              </template>
            </el-table-column>
            <el-table-column label="操作" width="180" align="left" class-name="small-padding">
              <template slot-scope="scope">
                <el-button
                  v-hasPermi="['system:post:edit']"
                  size="small"
                  type="text"
                  style="color:#4686EF"
                  @click="handleDetail(scope.row)"
                >查看</el-button>
                <el-button
                  v-if="scope.row.status === '99'"
                  v-hasPermi="['system:post:edit']"
                  size="small"
                  type="text"
                  style="color:#4686EF"
                  @click="handleSubmit(scope.row)"
                >提交</el-button>
                <el-button
                  v-if="scope.row.status === '99'"
                  v-hasPermi="['system:post:remove']"
                  size="small"
                  type="text"
                  style="color:#4686EF"
                  @click="handle(scope.row)"
                >办理</el-button>
              </template>
            </el-table-column>
          </el-table>
          <!-- 分页 -->
          <div class="common-pagination">
            <!-- <div class="chooseCount">共 {{ total }} 条</div> -->
            <pagination
              v-show="total > 0"
              :total="total"
              :page.sync="queryParams.pageNum"
              :limit.sync="queryParams.pageSize"
              @pagination="getList"
            />
          </div>
        </el-col>
      </el-row>
    </el-card>
  </div>
</template>
<script>
import TagsView from '@/layout/components/TagsView/index.vue'
export default {
  name: 'ApprovalCenter',
  components: { TagsView },
  dicts: ['asset_approval_status'],
  data() {
    return {
      // 左侧搜索框关键字
      keyword: '',
      approvalOptions: [
        { label: '采购申请 (2/2)' },
        { label: '领用申请 (4/9)' },
        { label: '借用申请 (6/6)' },
        { label: '调拨申请 (3/9)' },
        { label: '其它申请 (5/10)' }
      ],
      // 左侧树状默认展开的节点
      expandedArr: [],
      defaultProps: {
        children: 'children',
        label: 'label'
      },
      // 查询表单对象
      queryParams: {
        btnType: 11,
        pageNum: 1,
        pageSize: 10,
        num: undefined,
        getTime: undefined,
        status: undefined
      },
      // 时间段
      getTime: [],
      // 按钮类型和值
      radioList: [
        { label: '11', name: '全部' },
        { label: '0', name: '待处理' },
        { label: '1', name: '已处理' },
        { label: '2', name: '我提交的' },
        { label: '3', name: '抄送给我的' },
        { label: '4', name: '委托待办的' }
      ],
      // 展示表格数据
      approvalList: [
        { title: '笔记本电脑申请单', num: 'SQ2022080401', people: '张三', status: '0' },
        { title: '笔记本电脑申请单', num: 'SQ2022080402', people: '李四', status: '1' },
        { title: '笔记本电脑申请单', num: 'SQ2022080403', people: '王五', status: '99' }
      ],
      // 分页总条数
      total: 10,
      // 列表选中数据
      ids: [],
      // 加载效果
      loading: true
    }
  },
  watch: {
    // 左侧根据名称筛选树
    keyword(val) {
      this.$refs.tree.filter(val)
    }
  },
  created() {
    this.getList()
  },
  methods: {
    /** 查询列表 */
    getList() {
    },
    /** 批量提交 */
    handleBatchSubmission() {
    },
    /** 刷新 */
    refresh() {
      this.getList()
    },
    /** 左侧树状节点单击事件 */
    handleNodeClick(data) {
    },
    /** 左侧关键字过滤树节点 */
    filterNode(value, data) {
      if (!value) return true
      return data.label.indexOf(value) !== -1
    },
    /** 搜索按钮操作 */
    handleQuery() {
      this.queryParams.pageNum = 1
      this.getList()
    },
    /** 重置按钮操作 */
    resetQuery() {
      this.resetForm('queryForm')
      this.handleQuery()
    },
    /** 按钮状态切换 */
    chooseBtn(label) {
    },
    /** 表格数据多选 */
    handleSelectionChange(selection) {
      this.ids = selection
    },
    /** 根据状态取定type */
    getStatueType(value) {
      if (value === '0') {
        return 'primary'
      } else if (value === '1') {
        return 'success'
      } else {
        return 'info'
      }
    },
    /** 根据状态显示文本 */
    getStatusName(value) {
      switch (value) {
        case '0':
          return '审批中'
        case '1':
          return '已通过'
        case '99':
          return '待审批'
        default:
          return ''
      }
    },
    /** 查看按钮操作 */
    handleDetail(row) {
    },
    /** 提交按钮操作 */
    handleSubmit(row) {
    },
    /** 办理按钮操作 */
    handle(row) {
    }
  }
}
</script>
<style lang="scss" scoped>
// 主体样式
.post-box {
  .btn-text{
    vertical-align: middle;
  }
  // 分页
  // .common-pagination {
  //   display: flex;
  //   justify-content: flex-end;
  //   align-items: center;
  //   border-top: 1px solid #eaedf4;
  //   margin-top: 6px;
  //   .chooseCount {
  //     color: #7d8592;
  //     margin-top: 30px;
  //     margin-right: 360px;
  //   }
  // }
  // 选择日期组件
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
  // 展示表格
  .contentTable{
    padding-top:16px;
  }
}

</style>

