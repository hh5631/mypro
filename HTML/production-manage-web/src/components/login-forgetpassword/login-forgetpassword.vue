<template>
  <el-form
    ref="loginForm"
    :model="loginForm"
    :rules="loginRules"
    class="login-form"
  >
    <div class="username">
      <el-form-item prop="username">
        <span>用户名</span>
        <el-input
          v-model.trim="loginForm.username"
          type="text"
          auto-complete="off"
          placeholder="请输入用户名"
          maxlength="15"
        >
          <!-- <svg-icon
            slot="prefix"
            icon-class="user"
            class="el-input__icon input-icon"
          /> -->
        </el-input>
      </el-form-item>
    </div>
    <div class="phone">
      <el-form-item prop="phone">
        <span>手机号</span>
        <el-input
          v-model="loginForm.phone"
          type="number"
          auto-complete="off"
          placeholder="请输入手机号"
          show-phone
          oninput="if(value.length>11)value=value.slice(0,11)"
        >
          <!-- <svg-icon
            slot="prefix"
            icon-class="phone"
            class="el-input__icon input-icon"
          /> -->
        </el-input>
      </el-form-item>
    </div>

    <div class="code">
      <div>
        <el-form-item prop="code">
          <span>验证码</span>
          <el-input
            v-model="loginForm.code"
            auto-complete="off"
            placeholder="请输入短信验证码"
            size="medium"
            maxlength="20"
            :class="codeError||codeBlank?'codeBlank':''"
            @input="codeInput"
            @blur="codeBlur"
            @focus="codeError=false,codeBlank=false"
          >
          <!-- <svg-icon
            slot="prefix"
            icon-class="validCode"
            class="el-input__icon input-icon"
          /> -->
          </el-input>
        <!-- <span style="color:#EF5826;font-size:14px;"><i class="iconfont icon-jinggao" />用户名或密码错误</span> -->
        </el-form-item>
      </div>
      <div>
        <el-button :disabled="btnDisable" type="primary" plain size="small" class="btnCode" @click="btnClick">{{ btnText }}</el-button>
      </div>
    </div>
    <div v-if="codeBlank" class="tips">请输入验证码</div>
    <div v-if="codeError" class="tips">验证码错误，请重新输入</div>

    <el-form-item class="next">
      <!--  未输入手机号验证码时的灰色“下一步”按钮 -->
      <!-- <el-button
        v-if="!loginForm.phone||!loginForm.code||!loginForm.username"
        size="small"
        type="info"
        class="btnNext"
      >
        <span>下一步</span>
      </el-button> -->
      <!--  输入手机号验证码时的蓝色“下一步”按钮 -->
      <!-- v-if="loginForm.phone&&loginForm.code&&loginForm.username" -->
      <el-button
        :disabled="!loginForm.phone||!loginForm.code||!loginForm.username"
        size="small"
        type="primary"
        class="btnNext"
        @click="nextStep"
      >
        <span>下一步</span>
      </el-button>
      <!-- v-if="!this.$route.query.needSetPassword"   -->
      <el-button
        class="goBack"
        size="small"
        @click="goback"
      >
        <span>返回</span>
      </el-button>
    </el-form-item>
  </el-form>
</template>

<script>
import { checkUser, sendSmsCode, checkSmsCode } from '@/api/login.js'
import bus from '@/utils/bus'
export default {
  name: 'Loginforgetpassword',

  data() {
    const checkPhone = (rule, value, callback) => {
      const reg = /^1[345789]\d{9}$/
      if (!reg.test(value)) {
        callback(new Error('请输入11位手机号'))
      } else {
        callback()
      }
    }
    return {
      codeUrl: '',
      loginForm: {
        username: this.$route.query.username,
        code: '',
        phone: ''
      },
      loginRules: {
        username: [
          { required: true, trigger: 'blur', message: '请输入您的用户名' }
        ],
        password: [
          { required: true, trigger: 'blur', message: '请输入您的密码' }
        ],
        phone: [
          { required: true, trigger: 'blur', message: '请输入您的手机号' },
          { type: 'number', validator: checkPhone, message: '请输入正确的手机号', trigger: 'blur' }
        ]
        // code: [{ required: true, trigger: 'blur', message: '请输入验证码' }]
      },
      loading: false,
      // 验证码开关
      captchaOnOff: true,
      // 注册开关
      register: false,
      redirect: undefined,
      btnText: '发送验证码',
      btnDisable: false,
      codeError: false,
      codeBlank: false
    }
  },
  watch: {
    $route: {
      handler: function(route) {
        this.redirect = route.query && route.query.redirect
      },
      immediate: true
    }
  },
  mounted() {
    bus.$on('userName', (msg) => {
      this.loginForm.username = msg
    })
  },
  methods: {
    btnClick() {
      this.$refs.loginForm.validate((valid) => {
        if (valid) {
          // let obj = {
          //   phoneNumber:this.loginForm.phoneNumber
          // }
          checkUser({ phoneNumber: this.loginForm.phone, userName: this.loginForm.username }).then(res => {
            if (res.code === 200) {
              this.btnDisable = true
              this.$message({
                message: '发送成功',
                type: 'success'
              })
              sendSmsCode({ phone: this.loginForm.phone }).then(response => {
                if (response.code === 200) {
                  let sec = 60
                  sec = sec - 1
                  this.btnText = '重新发送' + sec
                  const timer = setInterval(() => {
                    sec = sec - 1
                    this.btnText = '重新发送' + sec + 's'
                    if (sec <= 0) {
                      this.btnDisable = false
                      this.btnText = '发送验证码'
                      clearInterval(timer)
                    }
                  }, 1000)
                }
              })
            }
          })
        }
      })
      // const reg = /^1[1356789]\d{9}$/
      // if (!reg.test(this.loginForm.phone)) {
      //   this.$message({
      //     message: '请输入正确的手机号',
      //     type: 'warning'
      //   })
      //   return
      // }
    },
    nextStep() {
      this.$refs.loginForm.validate((valid) => {
        if (valid) {
          const data = {
            phone: this.loginForm.phone,
            code: this.loginForm.code
          }
          checkSmsCode(data).then(res => {
            if (res.code !== 200) {
              this.codeError = true
              return
            }
            bus.$emit('userName', this.loginForm.username)
            // this.loginForm = {
            //   code: '',
            //   phone: ''
            // }
            this.$emit('nextStep', 3)
          })
        }
      })
    },
    goback() {
      this.$refs.loginForm.clearValidate()
      this.codeBlank = false
      this.loginForm.phone = ''
      this.loginForm.code = ''
      this.$emit('nextStep', 1)
    },
    codeInput(e) {
      this.codeBlank = false
      this.codeError = false
      e = this.loginForm.code
    },
    codeBlur() {
      if (!this.loginForm.code) {
        this.codeBlank = true
      }
    }
  }
}
</script>

<style lang="scss" scoped>
.title {
  margin: 20px auto 80px auto;
  text-align: center;
  width: 128px;
  height: 15px;
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
}
.login-tip {
  font-size: 13px;
  text-align: center;
  color: #bfbfbf;
}
.login-code {
  width: 100%;
  height: 38px;
  float: right;
  img {
    cursor: pointer;
    vertical-align: middle;
  }
}

.username {
  // position: absolute;
  // top: 50px;
  // right: 20px;
  // width: 90%;
}
.phone {
  // position: absolute;
  // top: 150px;
  // right: 20px;
  // width: 90%;
}
.code {
  display: flex;
  align-items: center;
    justify-content: space-between;
  // width: 53%;
  // position: absolute;
  // top: 240px;
}
.btnCode {
  // position: absolute;
  height: 40px;
  width: 120px;
  margin-top: 16px;
  // margin-left: 8%;
  // margin-top: -2px;
}
.btnNext {
  // position: absolute;
  // top: -30px;
  margin-top: 40px;
  width: 386px;
  background: #4686EF;
}
.tips{
  color:#EF5826;
  font-size:14px;
  margin-top: -20px;
  // position: absolute;
  // left: 25px;
  // top: 320px;
}
.next {
  // width: 356px;
  // position: absolute;
  // top: 390px;
  // left: 24px;
}
.codeBlank ::v-deep .el-input__inner{
  border-radius: 4px;
  border: 1px solid #EF5826;
}
.codeinput{
  border-radius: 4px;

}
::v-deep .btnCode.el-button--primary{
  background: #fff;
   color: #7D8592;
   border-color: #DDDDDD;
   //font-size: 18px;
}
::v-deep .btnCode.el-button--primary:hover{
  color: #417CEC;
  border-color: #417CEC;
}
.goBack{
   width: 386px;
   margin-top: 16px;
  // position: absolute;
  // top: 20px;
  margin-left: 0;
  // width: 112%;
}
::v-deep .btnNext.el-button--primary.is-disabled{
  background-color:#C2C4D1;
  border-color: #C2C4D1;
}
::v-deep .btnCode.el-button--primary:hover{

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
