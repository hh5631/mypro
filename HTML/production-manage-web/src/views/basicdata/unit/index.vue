<template>
  <div>
    <tags-view>
      <el-row :gutter="10" class="mb8">
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['system:user:add']"
            type="primary"
            size="small"
            @click="handleAdd"
          ><i class="iconfont icon-zengjia" style="color: #ffffff" />新增</el-button>
        </el-col>
      </el-row>
    </tags-view>
    <el-card class="main-card">
      <el-form ref="queryForm" :model="queryParams" :inline="true">
        <el-form-item label="基准单位" prop="base">
          <el-input
            v-model.trim="queryParams.base"
            placeholder="请输入"
            maxlength="15"
            clearable
            size="small"
            style="width: 200px"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item label="辅助单位" prop="assist">
          <el-input
            v-model.trim="queryParams.assist"
            placeholder="请输入"
            maxlength="15"
            clearable
            size="small"
            style="width: 200px"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <!-- <el-form-item label="状态" prop="enable">
          <el-select v-model="queryParams.enable" style="width: 200px" placeholder="请选择" clearable size="small">
            <el-option
              v-for="dict in dict.type.sys_normal_disable"
              :key="dict.value"
              :label="dict.label"
              :value="dict.value"
            />
          </el-select>
        </el-form-item> -->
        <el-form-item>
          <el-button v-debounce size="small" @click="resetQuery">重置</el-button>
          <el-button v-debounce size="small" class="search-btn" type="primary" plain @click="handleQuery">搜索</el-button>
        </el-form-item>
      </el-form>

      <el-table ref="supplierTable" v-loading="loading" highlight-current-row height="550" :data="supplierTable" @selection-change="handleSelectionChange">
        <el-table-column type="selection" width="55" align="left" />
        <el-table-column label="序号" type="index" width="70" align="left" />
        <el-table-column label="基准单位" width="390" align="left" prop="base" :show-overflow-tooltip="true" />
        <el-table-column label="辅助单位" width="240" align="left" prop="assist" :show-overflow-tooltip="true" />
        <el-table-column label="比例" width="240" align="left" prop="ratio" :show-overflow-tooltip="true" />
        <el-table-column label="单位类别" align="left" prop="unitType" :show-overflow-tooltip="true" />
        <!-- <el-table-column label="状态" align="left" prop="enable">
          <template slot-scope="scope">
            <div>
              <el-switch
                v-model="scope.row.enable"
                active-value="0"
                inactive-value="1"
                @change="changeState($event,scope.row)"
              />
            </div>
          </template>
        </el-table-column> -->
        <!-- <el-table-column label="创建时间" align="center" prop="createTime" width="180">
          <template slot-scope="scope">
            <span>{{ parseTime(scope.row.createTime) }}</span>
          </template>
        </el-table-column> -->
        <el-table-column label="操作" width="200" align="left" class-name="small-padding">
          <template slot-scope="scope">
            <el-button
              size="small"
              type="text"
              @click="handleUpdate(scope.row,0)"
            >详情</el-button>
            <el-button
              size="small"
              type="text"
              style="color:#4686EF"
              @click="handleUpdate(scope.row,1)"
            >编辑</el-button>
            <el-button
              size="small"
              type="text"
              style="color:#EF5826"
              @click="handleDelete(scope.row)"
            >删除</el-button>
          </template>
        </el-table-column>
      </el-table>
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
    <el-dialog
      :title="title"
      :visible.sync="addOrEditDig"
      :width="isEdit?'800px':'600px'"
      class="dialogContent"
      append-to-body
      :close-on-click-modal="false"
      destroy-on-close
      @close="closeDig"
    ><el-form ref="form" :model="form" :rules="rules" label-width="193px" style="margin-right: 20px;">
      <el-form-item label="基准单位" :prop="isEdit?'base':''">
        <el-input v-if="isEdit" v-model="form.base" maxlength="15" placeholder="请输入基准单位(小单位)" @change="trimFun('base',$event)" />
        <span v-else>{{ form.base }}</span>
      </el-form-item>
      <el-row v-if="isEdit">
        <el-col :span="14">
          <el-form-item label="辅助单位" :prop="isEdit?'assist':''" class="assist">
            <el-input v-model="form.assist" maxlength="15" placeholder="请输入辅助单位(大单位)" @change="trimFun('assist',$event)" />
          </el-form-item>
        </el-col>
        <el-col :span="2">
          <span>=</span>
        </el-col>
        <el-col :span="8">
          <el-form-item :prop="isEdit?'ratio':''" class="ratio">
            <el-input v-model="form.ratio" v-trim.exceptTwoNumber="form.ratio" maxlength="15" placeholder="请输入比例(数字)" />
          </el-form-item>
        </el-col>
      </el-row>
      <el-form-item v-else label="辅助单位">
        <span>{{ form.assist }} = {{ form.ratio }}</span>
      </el-form-item>
      <el-form-item label="单位类别" :prop="isEdit?'unitType':''" class="unitType">
        <el-select v-if="isEdit" v-model="form.unitType" placeholder="请选择计量单位类别">
          <el-option
            v-for="dict in dict.type.basicdata_unit_type"
            :key="dict.value"
            :label="dict.label"
            :value="dict.label"
          />
        </el-select>
        <span v-else>{{ form.unitType }}</span>
      </el-form-item>
      <!-- <el-form-item label="状态" :prop="isEdit?'enable':''">
        <el-radio-group v-if="isEdit" v-model="form.enable">
          <el-radio
            v-for="dict in dict.type.sys_normal_disable"
            :key="dict.value"
            :label="dict.value"
          >{{ dict.label }}</el-radio>
        </el-radio-group>
        <span v-else>{{ form.enable==='0'?'正常':'停用' }}</span>
      </el-form-item> -->
      <el-form-item label="备注" prop="remark">
        <el-input v-if="isEdit" v-model="form.remark" maxlength="500" show-word-limit type="textarea" placeholder="请输入" />
        <div v-else class="item-note">{{ form.remark }}</div>
      </el-form-item>
    </el-form><div v-if="isEdit" slot="footer" class="dialog-footer">
      <el-button size="small" @click="cancel">取消</el-button>
      <el-button size="small" type="primary" @click="submitForm">保存</el-button>
    </div></el-dialog>
  </div>
</template>

<script>
import { getUnitByCondition, addOrUpdateUnit, delUnit, updateUnitState } from '@/api/basicdata/index'
import TagsView from '@/layout/components/TagsView/index.vue'
import { myTrim } from '@/utils/index'
export default {
  components: { TagsView },
  dicts: ['sys_normal_disable', 'basicdata_unit_type'],
  data() {
    const self = this
    return {
      queryParams: {
        assist: undefined,
        base: undefined,
        enable: undefined,
        pageNum: 1,
        pageSize: 10
      },
      supplierTable: [],
      total: 0,
      loading: false,
      // selectedLength: 0,

      title: '',
      addOrEditDig: false,
      form: {},
      isEdit: false,
      rules: {
        base: [
          { required: true, message: '请填写基准单位', trigger: 'blur' },
          {
            validator: function(rule, value, callback) {
              if (/^[\u4e00-\u9fa5a-zA-Z\s]+$/.test(value) === false) {
                callback(new Error('请输入汉字或英文'))
              } else {
                // 校验通过
                callback()
              }
            },
            trigger: ['blur', 'change']
          }
        ],
        assist: [
          {
            validator: function(rule, value, callback) {
              if (value === '' || value === undefined || value === null) {
                callback()
              } else {
                if (/^[\u4e00-\u9fa5a-zA-Z\s]+$/.test(value) === false) {
                  callback(new Error('请输入汉字或英文'))
                } else {
                  callback()
                }
              }
            },
            trigger: ['blur', 'change']
          }
        ],
        ratio: [
          {
            validator: function(rule, value, callback) {
              if (self.form.assist) {
                if (/^\d+$|^\d*\.\d+$/g.test(value) === false) {
                  callback(new Error('请输入数字'))
                } else if (value <= 0) {
                  callback(new Error('请填写大于0的比例'))
                } else {
                  callback()
                }
              } else {
                callback()
              }
            },
            trigger: ['blur', 'change']
          }
        ],
        unitType: [
          { required: true, message: '请选择单位类别', trigger: ['blur', 'change'] }
        ],
        enable: [
          { required: true, message: '请选择状态', trigger: 'blur' }
        ]
      }
    }
  },
  created() {
    this.getList()
  },
  methods: {
    getList() {
      this.loading = true
      getUnitByCondition(this.queryParams).then(
        (response) => {
          this.loading = false
          this.supplierTable = response.data.records
          this.total = response.data.total
        }
      )
    },
    // 去首位空格，但可输入中间空格
    trimFun(str, val) {
      this.form[str] = myTrim(val)
    },
    handleUpdate(row, index) {
      if (index === 1) {
        this.isEdit = true
        this.title = '编辑计量单位'
        getUnitByCondition({ id: row.id, pageNum: 1, pageSize: 10 }).then((res) => {
          this.form = res.data.records[0]

          // this.form.enable = this.form.enable.toString()
          this.addOrEditDig = true
        })
      } else {
        this.title = '详情'
        this.isEdit = false
        getUnitByCondition({ id: row.id, pageNum: 1, pageSize: 10 }).then((res) => {
          this.form = res.data.records[0]
          // this.form.enable = this.form.enable.toString()
          this.addOrEditDig = true
        })
      }
    },
    handleDelete(row) {
      const id = row.id || this.ids
      const base = row.base
      this.$modal
        .confirm('确认删除单位"' + base + '"', '删除单位')
        .then(function() {
          return delUnit(id)
        })
        .then(() => {
          // 如果删除之后当前数据长度为1则this.queryParams页数少1
          if (this.supplierTable.length === 1 && this.queryParams.pageNum !== 1) {
            this.queryParams.pageNum = this.queryParams.pageNum - 1
          }

          this.getList()
          this.$modal.msgSuccess('删除成功')
        })
        .catch(() => {})
    },
    submitForm() {
      this.$refs['form'].validate(valid => {
        console.log(valid)
        if (valid) {
          if (!this.form.postId) {
            addOrUpdateUnit(this.form).then(() => {
              this.$modal.msgSuccess('保存成功')
              this.addOrEditDig = false
              this.resetForm('queryForm')
              this.getList()
            })
          } else {
            addOrUpdateUnit(this.form).then(() => {
              this.$modal.msgSuccess('修改成功')
              this.addOrEditDig = false
              this.resetForm('queryForm')
              this.getList()
            })
          }
        }
      })
    },
    reset() {
      this.form = {
        assist: undefined,
        base: undefined,
        unitType: undefined,
        enable: '0',
        remark: undefined,
        ratio: undefined
      }
      this.resetForm('form')
    },
    resetForm(refName) {
      if (this.$refs[refName]) {
        this.$refs[refName].resetFields()
      }
    },
    handleQuery() {
      this.queryParams.pageNum = 1
      this.getList()
    },
    resetQuery() {
      this.resetForm('queryForm')
      this.handleQuery()
    },
    // 多选框选中数据
    handleSelectionChange(selection) {
      this.ids = []
      selection.forEach(item => {
        this.ids.push(item.id)
      })
    },
    handleAdd() {
      this.addOrEditDig = true
      this.isEdit = true
      this.reset()
      this.title = '新建计量单位'
    },
    // 取消按钮
    cancel() {
      this.reset()
      this.addOrEditDig = false
    },
    closeDig() {
      this.reset()
      this.addOrEditDig = false
    },
    // switch改变状态
    changeState(el, row) {
      updateUnitState(row.id).then(res => {
        if (res.code === 200) {
          this.$modal.msgSuccess('修改成功')
          this.getList()
        } else {
          this.$message.error(res.msg)
          this.getList()
        }
      })
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
  .common-pagination {
    // display: flex;
    // justify-content: space-between;
    // align-items: center;
    // border-top: 1px solid #eaedf4;
    margin-top: 6px;
//     .chooseCount {
//       color: #7d8592;
//       margin-top: 20px;
// }
  }
  ::v-deep .el-dialog .el-dialog__body{
      max-height: 542px;
      overflow-y: auto;
      padding: 24px 20px 0;
    }
.assist{
    ::v-deep .el-form-item__content{
      width:220px;

    }
    ::v-deep .el-input__inner{
        width:220px;
      }
}
::v-deep .el-col-2 {
	width: 8.33333%;
	height: 36px;
	text-align: center;
	line-height: 36px;
}
.unitType{
  ::v-deep .el-select{
    width: 100%;
  }
}
.dialogContent {

}
.ratio {
  ::v-deep .el-form-item__content {
      margin-left: 26px !important;
      width:220px
    }
}
.item-note{
    width: 370px;
    // height:auto;
    max-height: 250px;
    padding: 5px 10px 5px 0;
    overflow-y: scroll;
  }
</style>

