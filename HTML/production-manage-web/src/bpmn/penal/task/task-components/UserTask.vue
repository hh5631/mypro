<template>
  <div style="margin-top: 16px">
    <el-form-item label="任务类型">
      <el-select v-model="userTaskForm.taskType" @change="changetaskType">
        <el-option value="0" label="常规" />
        <el-option value="1" label="退回" />
      </el-select>
    </el-form-item>
    <el-form-item label="处理角色" v-show="userTaskForm.taskType == 0">
      <el-select
        v-model="userTaskForm.candidateGroups"
        clearable
        filterable
        remote
        @change="updateElementTask('candidateGroups')"
      >
        <el-option
          v-for="role in roleList"
          :key="role.roleKey"
          :label="role.roleName"
          :value="role.roleKey"
        />
      </el-select>
      <!-- <el-cascader
    v-model="dept"
    :options="roleList"
    :props="{ expandTrigger: 'hover',checkStrictly: true,value:'deptId',label:'deptName' }"
    @change="handleDeptChange"></el-cascader> -->
    </el-form-item>
    <el-form-item label="处理用户">
      <el-select
        clearable
        v-model="userTaskForm.assignee"
        filterable
        remote
        :remote-method="getUserInfo"
        @change="updateElementTask('assignee')"
        :loading="loading"
        @clear="getUserInfo"
      >
        <el-option
          v-for="user in userList"
          :key="user.userName"
          :label="getUserName(user.realName)"
          :value="user.userName"
        />
      </el-select>
    </el-form-item>
    <!-- <el-form-item label="候选用户">
      <el-select
        v-model="userTaskForm.candidateUsers"
        multiple
        remote
        :remote-method="getUserInfo"
        collapse-tags
        @change="updateElementTask('candidateUsers')"
      >
        <el-option
            v-for="user in userList"
            :key="user.id"
            :label="`${user.nickname}-${user.deptName}部门`"
            :value="user.id"
        />
      </el-select>
    </el-form-item>
    <el-form-item label="候选分组">
      <el-select
        v-model="userTaskForm.candidateGroups"
        multiple
        collapse-tags
        @change="updateElementTask('candidateGroups')"
      >
        <el-option v-for="gk in mockData" :key="'ass-' + gk" :label="`分组${gk}`" :value="`group${gk}`" />
      </el-select>
    </el-form-item>
    <el-form-item label="到期时间">
      <el-date-picker
        v-model="userTaskForm.dueDate"
        clearable
        @change="updateElementTask('dueDate')"
        type="datetime"
        placeholder="选择到期时间"
        value-format="yyyy-MM-dd HH:mm:ss"
      >
      </el-date-picker>
    </el-form-item>
    <el-form-item label="跟踪时间">
      <el-date-picker
        v-model="userTaskForm.followUpDate"
        clearable
        @change="updateElementTask('followUpDate')"
        type="datetime"
        placeholder="选择跟踪时间"
        value-format="yyyy-MM-dd HH:mm:ss"
      >
      </el-date-picker>
    </el-form-item>
    <el-form-item label="优先级">
      <el-input v-model="userTaskForm.priority" clearable @change="updateElementTask('priority')" />
    </el-form-item> -->
  </div>
</template>

<script>
import { listUser } from "@/api/system/user";
import { listRole } from "@/api/system/role";
export default {
  name: "UserTask",
  props: {
    id: String,
    type: String,
    multiType: String,
  },
  data () {
    return {
      defaultTaskForm: {
        assignee: "",
        candidateUsers: [],
        candidateGroups: [],
        dueDate: "",
        followUpDate: "",
        priority: "",
        taskType: "0", //任务类型 0-常规,1-退回
      },
      userTaskForm: {
        assignee: "",
      },
      mockData: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10],
      userList: [],
      roleList: [],
      userSearchInfo: {
        page: 1,
        limit: 999,
      },
      deptSearchInfo: {
        page: 1,
        limit: 99,
      },
      loading: false,
    };
  },
  watch: {
    id: {
      immediate: true,
      handler () {
        this.bpmnElement = window.bpmnInstances.bpmnElement;
        this.$nextTick(() => this.resetTaskForm());
      },
    },
    multiType: {
      immediate: true,
      handler (val) {
        if (!val) {
          return;
        }
        if (val == "ParallelMultiInstance") {
          this.userTaskForm.assignee = "${assignee}";
          this.userTaskForm.candidateGroups = "";
          this.userTaskForm.taskType = "0";
          this.updateElementTask("candidateGroups");
          this.updateElementTask("assignee");
        } else {
          this.userTaskForm.assignee = "";
          this.updateElementTask("assignee");
        }
      },
    },
  },
  mounted () {
    this.getRoleInfo();
    this.getUserInfo();
  },
  methods: {
    //获取用户名
    getUserName (val) {
      switch (val) {
        case "${initiatorBy}":
          return "发起人";
        case "${deptLeader}":
          return "部门领导";
        default:
          return val;
      }
    },
    // 修改任务类型
    changetaskType () {
      this.$set(this.userTaskForm, "assignee", "${initiatorBy}");
      this.$nextTick(() => {
        this.updateElementTask("assignee");
      });
      this.updateElementTask("taskType");
      this.$store.commit("flow/SET_FORM_TYPE", this.userTaskForm.taskType);
    },
    //获取角色信息
    getRoleInfo () {
      listRole({ pageSize: 999 }).then((da) => {
        this.roleList = da.rows;
      });
    },
    //获取用户信息
    getUserInfo (userName) {
      // 
      this.loading = true;
      listUser({ ...this.userSearchInfo, userName })
        .then((da) => {
          // console.log(da)
          this.userList = [
            { realName: "${initiatorBy}", userName: "${initiatorBy}"},
            { realName: "${deptLeader}",userName: "${deptLeader}" },
          ].concat(da.rows);
        })
        .finally(() => {
          this.loading = false;
        });
    },
    resetTaskForm () {
      for (let key in this.defaultTaskForm) {
        let value;
        // if (key === "candidateUsers" || key === "candidateGroups") {
        //   value = this.bpmnElement?.businessObject[key]
        //     ? this.bpmnElement.businessObject[key].split(",")
        //     : [];
        // } else {
        value =
          this.bpmnElement?.businessObject[key] || this.defaultTaskForm[key];
        // }
        this.$set(this.userTaskForm, key, value);
      }
    },
    updateElementTask (key) {
      const taskAttr = Object.create(null);
      // if (key === "candidateUsers" || key === "candidateGroups") {
      //   taskAttr[key] =
      //     this.userTaskForm[key] && this.userTaskForm[key].length
      //       ? this.userTaskForm[key].join()
      //       : null;
      // } else {
      taskAttr[key] = this.userTaskForm[key] || null;
      // }
      window.bpmnInstances.modeling.updateProperties(
        this.bpmnElement,
        taskAttr
      );
    },
  },
  beforeDestroy () {
    this.bpmnElement = null;
  },
};
</script>
