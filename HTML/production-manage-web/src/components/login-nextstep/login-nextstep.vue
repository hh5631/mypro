<template>
  <div>
    <el-form
      ref="loginForm"
      :model="loginForm"
      :rules="loginRules"
      class="login-form"
    >
      <div class="inputFirst">
        <el-form-item prop="password">
          <span>新密码</span>
          <el-input
            v-model.trim="loginForm.password"
            type="password"
            auto-complete="off"
            maxlength="15"
            :class="isSame ? '' : 'different'"
            placeholder="请输入新密码"
            :onkeyup="
              (loginForm.password = loginForm.password.replace(/\s+/g, ''))
            "
            @focus="isShowClear = true"
            @paste.native.capture.prevent
          >
            <!-- <svg-icon
              slot="prefix"
              icon-class="password"
              class="el-input__icon input-icon"
            /> -->
            <i
              v-if="isShowClear"
              slot="suffix"
              style="font-size:20px;line-height: 40px;"
              class="iconfont icon-guanbi"
              @click="clearPsd"
            />
          </el-input>
        </el-form-item>
      </div>
      <div class="inputSecond">
        <el-form-item prop="passwordConfig">
          <span>确认密码</span>
          <el-input
            v-model.trim="loginForm.passwordConfig"
            type="password"
            auto-complete="off"
            :class="isSame ? '' : 'different'"
            maxlength="15"
            placeholder="请再次输入新密码"
            :onkeyup="
              (loginForm.passwordConfig = loginForm.passwordConfig.replace(
                /\s+/g,
                ''
              ))
            "
            @focus="isShowClear2 = true"
            @paste.native.capture.prevent
          >
            <!-- <svg-icon
              slot="prefix"
              icon-class="password"
              class="el-input__icon input-icon"
            /> -->
            <i
              v-if="isShowClear2"
              slot="suffix"
              class="iconfont icon-guanbi"
              style="font-size:20px;line-height: 40px;"
              @click="clearConfirmPsd"
            />
          </el-input>
        </el-form-item>
      </div>
      <el-form-item>
        <!-- <span
          v-if="
            loginForm.passwordConfig !== loginForm.password &&
              loginForm.passwordConfig
          "
          class="tips"
        ><i class="iconfont icon-jinggao" />两次密码输入不一致</span> -->
        <!-- <el-button
          v-if="!loginForm.passwordConfig || !loginForm.password"
          size="medium"
          type="info"
          class="complete"
        >
          <span>完 成</span>
        </el-button> -->
        <!-- v-if="loginForm.passwordConfig && loginForm.password" -->
        <el-button
          :disabled="!loginForm.passwordConfig || !loginForm.password"
          size="medium"
          type="primary"
          class="complete"
          @click="completeModify"
        >完 成</el-button>
        <el-button v-if="$route.query.needSetPassword" class="goBack" @click="gotoback">返回</el-button></el-form-item>
    </el-form>
  </div>
</template>

<script>
import bus from '@/utils/bus'
import { resetPassword } from '@/api/login.js'
export default {
  name: 'LoginNextstep',

  data() {
    // var validatePass = (rule, value, callback) => {
    //   if (value === '') {
    //     callback(new Error('请输入密码'))
    //   } else {
    //     if (this.loginForm.password !== '') {
    //       this.$refs.loginForm.validateField('passwordConfig')
    //     }
    //     callback()
    //   }
    // }
    var validatePass2 = (rule, value, callback) => {
      // const regex = new RegExp('^(?=.*\\d)|(?=.*[a-zA-Z]).{6,16}$')
      if (value.length < 6) {
        callback(new Error('请输入的密码长度最少为6位'))
      } else if (value.length > 16) {
        callback(new Error('请输入的密码长度最多为16位'))
      } else {
        callback()
      }
    }
    var validatePass3 = (rule, value, callback) => {
      // const regex = new RegExp('^(?=.*\\d)(?=.*[a-zA-Z]).{6,15}$')
      if (this.loginForm.password !== value) {
        callback(new Error('两次密码输入不一致'))
      } else {
        callback()
      }
    }
    /*     var validatePass2 = (rule, value, callback) => {
      if (value === '') {
        callback(new Error('请再次输入密码'))
      } else if (value !== this.loginForm.password) {
        callback(new Error())
      } else {
        callback()
      }
    } */
    return {
      codeUrl: '',
      loginForm: {
        password: '',
        passwordConfig: ''
      },
      loginRules: {
        password: [
          { required: true, trigger: 'blur', message: '请输入新密码' },
          // {
          //   min: 6,
          //   max: 16,
          //   message: '',
          //   trigger: 'blur'
          // }
          // { validator: validatePass },
          { validator: validatePass2, trigger: 'blur' }
        ],
        passwordConfig: [
          { required: true, message: '请再次输入新密码', trigger: 'blur' },
          { validator: validatePass3, trigger: 'blur' }
          // { min: 6, max: 16, message: '请输入的密码长度最少为6位', trigger: 'blur' },
          // { validator: validatePass2, trigger: 'blur', required: true }
        ]
      },
      loading: false,
      // 验证码开关
      captchaOnOff: true,
      // 注册开关
      register: false,
      redirect: undefined,
      isShowClear: false,
      isShowClear2: false,
      userName: undefined
    }
  },
  computed: {
    isSame() {
      if (
        this.loginForm.passwordConfig !== this.loginForm.password &&
        this.loginForm.passwordConfig
      ) {
        return false
      } else {
        return true
      }
    }
  },
  mounted() {
    bus.$on('userName', (msg) => {
      this.userName = msg
    })
  },
  methods: {
    clearPsd() {
      this.loginForm.password = ''
      this.$nextTick(() => {
        this.isShowClear = false
      })
    },
    clearConfirmPsd() {
      this.loginForm.passwordConfig = ''
      this.$nextTick(() => {
        this.isShowClear2 = false
      })
    },
    completeModify() {
      this.$refs.loginForm.validate((valid) => {
        if (
          valid &&
          this.loginForm.password === this.loginForm.passwordConfig
        ) {
          const data = {
            password: this.loginForm.password,
            userName: this.$route.query.username || this.userName
          }
          resetPassword(data).then((res) => {
            if (res.code === 200) {
              this.$message({
                message: '密码修改成功',
                type: 'success'
              })
              this.loginForm = {
                password: '',
                passwordConfig: ''
              }
              this.$emit('goback', 1)
            }
          })
        }
      })
    },
    gotoback() {
      this.loginForm.password = ''
      this.loginForm.passwordConfig = ''
      this.$emit('goback', 1)
    }
  }
}
</script>

<style lang="scss" scoped>
.title {
  margin: 20px auto 70px auto;
  text-align: center;
  width: 128px;
  height: 0px;
  font-size: 32px;
  overflow-wrap: break-word;
  color: rgba(51, 51, 51, 1);
  //display: block;
}

.login-form {
  // position: relative;
  // left: 100px;
  // top: -21px;
  box-shadow: 0px 2px 8px 0px rgba(15, 72, 157, 0.5);
  border-radius: 0px 8px 8px 0px;
  background: #ffffff;
  width: 436px;
  height: 500px;
  padding: 25px 25px 5px 25px;
  .el-input {
    height: 38px;
    input {
      height: 38px;
    }
  }
  .input-icon {
    height: 39px;
    width: 14px;
    margin-left: 2px;
  }
}
.login-tip {
  font-size: 13px;
  text-align: center;
  color: #bfbfbf;
}
.login-code {
  width: 33%;
  height: 38px;
  float: right;
  img {
    cursor: pointer;
    vertical-align: middle;
  }
}
.complete {
  // position: absolute;
  // top: 350px;
  width: 386px;
  margin-top: 130px;
  background: #4686EF;
}
.inputFirst {
  // position: absolute;
  // top: 50px;
  // width: 85%;
}
.inputSecond {
  // position: absolute;
  // top: 150px;
  // width: 85%;
}
.tips {
  color: #ef5826;
  font-size: 12px;
  // position: absolute;
  // top: 196px;
}
.different ::v-deep .el-input__inner {
  border-radius: 4px;
  border: 1px solid #ff0000;
}
.goBack {
  // position: absolute;
  // top: 395px;
  margin-top: 16px;
  margin-left: 0;
  width: 386px;
}
::v-deep .el-button--primary.is-disabled{
  background-color:#C2C4D1;
  border-color: #C2C4D1;
}
::v-deep .el-input__inner{
  height: 40px;
  line-height: 40px;
}
::v-deep .el-form-item__error{
  color: #EF5826;
  font-size: 14px;
}
::v-deep .el-input__inner:focus{
  border-color: #4686EF;
}
</style>
