<template>
  <el-form
    ref="loginForm"
    :model="loginForm"
    :rules="loginRules"
    class="login-form"
  >
    <h3 class="title">用户登录</h3>
    <el-form-item prop="username">
      <span>用户名</span>
      <el-input
        v-model.trim="loginForm.username"
        type="text"
        auto-complete="off"
        placeholder="请输入用户名"
        maxlength="15"
        autofocus
        @focus="usrFocus"
        @input="usrInput"
      >
        <i v-if="isShowClearUsr" slot="suffix" style="line-height: 40px;" :class="clearIcon" @click="clearUsr" />
        <!-- <svg-icon
          slot="prefix"
          icon-class="user"
          class="el-input__icon input-icon"
        /> -->
      </el-input>
    </el-form-item>
    <el-form-item prop="password">
      <span>密码</span>
      <el-input
        v-model.trim="loginForm.password"
        :type="passw"
        :class="psdError?'passw':''"
        auto-complete="off"
        placeholder="请输入密码"
        maxlength="16"
        oninput="value=value.replace(/[\u4E00-\u9FA5]/ig, '')"
        @blur="stringChange"
        @focus="psdFocus"
        @input="psdInput"
        @keyup.enter.native="handleLogin"
        @paste.native.capture.prevent="handlePaste"
      >
        <!-- <svg-icon
          slot="prefix"
          icon-class="password"
          class="el-input__icon input-icon"
        /> -->
        <i v-if="isShowClearPsd" slot="suffix" :class="clearIcon" style="padding-right:8px;line-height: 40px;" @click="clearPsd" />
        <i slot="suffix" :class="icon" style="font-size:20px;line-height: 40px;" @click="showPass" />
      </el-input>
      <span v-if="psdError" class="tips"><i class="iconfont icon-jinggao" />用户名或密码错误</span>
    </el-form-item>

    <el-checkbox v-model="loginForm.rememberMe" class="rememberMe">
      记住密码</el-checkbox>
    <el-link class="forget" @click="forgetPassword">忘记密码</el-link>

    <el-form-item>
      <!-- <el-button>默认按钮</el-button>
      <el-button size="medium">中等按钮</el-button>
      <el-button size="small">小型按钮</el-button> -->
      <el-button
        :loading="loading"
        type="primary"
        class="tologin"
        @click.native.prevent="handleLogin"
      >
        <span v-if="!loading">登 录</span>
        <span v-else>登 录 中...</span>
      </el-button>
      <div v-if="register" style="float: right">
        <router-link class="link-type" :to="'/register'">立即注册</router-link>
      </div>
    </el-form-item>
  </el-form>
</template>

<script>
import router from '@/router'
import { getCodeImg } from '@/api/login'
import Cookies from 'js-cookie'
import { encrypt, decrypt } from '@/utils/jsencrypt'
import { getNumByReadType } from '@/api/assetChange/index'
import eventBus from '@/utils/bus.js'
export default {
  name: 'Login',
  data() {
    return {
      codeUrl: '',
      loginForm: {
        username: '',
        password: '',
        rememberMe: false,
        code: '',
        uuid: ''
      },
      loginRules: {
        username: [
          { required: true, trigger: 'blur', message: '请输入您的账号' }
        ],
        password: [
          { required: true, trigger: 'blur', message: '请输入您的密码' }
        ],
        code: [{ required: true, trigger: 'change', message: '请输入验证码' }]
      },
      loading: false,
      // 验证码开关
      captchaOnOff: true,
      // 注册开关
      register: false,
      redirect: undefined,
      passw: 'password',
      clearIcon: 'iconfont icon-guanbi',
      icon: 'iconfont icon-yincang',
      psdError: false,
      isShowClearPsd: false,
      isShowClearUsr: false
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
  created() {
    this.getCode()
    this.getCookie()
  },
  methods: {
    showPass() {
      //  点击图标是密码隐藏或显示
      if (this.passw === 'text') {
        this.passw = 'password'
        //  更换图标
        this.icon = 'iconfont icon-yincang'
      } else {
        this.passw = 'text'
        this.icon = 'iconfont icon-xianshi'
      }
    },
    clearPsd() {
      this.loginForm.password = ''
      this.$nextTick(() => {
        this.isShowClearPsd = false
      })
    },
    clearUsr() {
      this.loginForm.username = ''
      this.$nextTick(() => {
        this.isShowClearUsr = false
      })
    },
    psdFocus() {
      this.psdError = false
      this.isShowClearPsd = true
    },
    usrFocus() {
      this.isShowClearUsr = true
    },
    psdInput(e) {
      this.isShowClearPsd = true
      if (e === '') {
        this.isShowClearPsd = false
      }
    },
    usrInput(e) {
      this.isShowClearUsr = true
      if (e === '') {
        this.isShowClearUsr = false
      }
    },
    getCode() {
      getCodeImg().then((res) => {
        this.captchaOnOff =
          res.captchaOnOff === undefined ? true : res.captchaOnOff
        if (this.captchaOnOff) {
          this.codeUrl = 'data:image/gif;base64,' + res.img
          this.loginForm.uuid = res.uuid
        }
      })
    },
    getCookie() {
      const username = Cookies.get('username')
      const password = Cookies.get('password')
      const rememberMe = Cookies.get('rememberMe')
      this.loginForm = {
        username: username === undefined ? this.loginForm.username : username,
        password:
          password === undefined ? this.loginForm.password : decrypt(password),
        rememberMe: rememberMe === undefined ? false : Boolean(rememberMe)
      }
    },
    handleLogin() {
      this.$refs.loginForm.validate((valid) => {
        if (valid) {
          this.$store.dispatch('Login', this.loginForm).then(async(e) => {
            this.loading = true
            const res = await this.$store.dispatch('GetInfo')
            // 首次登录修改密码
            if (res.user.isLogined === '0') {
              this.$alert('首次登录，请修改密码', '提示', {
                confirmButtonText: '确定',
                callback: (action) => {
                  this.$store.dispatch('LogOut').then(() => {
                    this.$router.push({
                      path: '/login',
                      query: {
                        needSetPassword: true,
                        username: this.loginForm.username
                      }
                    })
                  })
                }
              })
            }
            // 派发路由
            this.$store.dispatch('GenerateRoutes').then(accessRoutes => {
              // 根据roles权限生成可访问的路由表
              router.addRoutes(accessRoutes) // 动态添加可访问路由表
            })
            // 跳转到首页
            this.$router.push({ path: this.redirect || '/' }).catch(() => {})
            // 勾选了记住密码
            if (this.loginForm.rememberMe) {
              Cookies.set('username', this.loginForm.username, { expires: 30 })
              Cookies.set('password', encrypt(this.loginForm.password), {
                expires: 30
              })
              Cookies.set('rememberMe', this.loginForm.rememberMe, {
                expires: 30
              })
            } else {
              Cookies.remove('username')
              Cookies.remove('password')
              Cookies.remove('rememberMe')
            }
            // 测试登录成功，路由派发完成，vuex信息设置完成，发起websocket通信
            const url = `ws://172.16.8.123:38080/message/websocket/${this.$store.state.user.name}`
            this.initWebSocket(url)
          }).catch((e) => {
            this.loading = false
            if (this.captchaOnOff) {
              this.getCode()
            }
            if ('msg' in e) {
              this.psdError = true
            }
          })
        }
      })
    },
    forgetPassword() {
      // bus.$emit('userName', this.loginForm.username)
      // this.$emit('forgetPassword', 2)
      this.$message.error('请联系管理员,帮助重新设置密码')
    },
    stringChange(e) {
      this.loginForm.password = e.target.value
    },
    /** 建立websocket通信 */
    // 初始化weosocket
    initWebSocket(url) {
      // const wsuri = 'ws://10.100.2.151:38080/message/websocket/admin'
      this.websock = new WebSocket(url)
      this.$store.commit('SET_WSINSTANCE', this.websock)
      this.websock.onmessage = this.websocketonmessage
      this.websock.onopen = this.websocketonopen
      this.websock.onerror = this.websocketonerror
      this.websock.onclose = this.websocketclose
    },
    // 连接建立之后执行send方法发送数据
    websocketonopen() {
      const actions = { 'test': '12345' }
      this.websocketsend(JSON.stringify(actions))
    },
    // 连接建立失败重连
    websocketonerror() {
      this.initWebSocket()
    },
    // 数据接收
    websocketonmessage(e) {
      const redata = JSON.parse(e.data)
      console.log(' this.websock', redata)
      if (redata) {
        // 获得未读数据
        this.getMsgNum()
        // 消息
        this.$notify.info({
          title: '消息提醒',
          offset: 100,
          dangerouslyUseHTMLString: true,
          message: `<div style="margin-bottom:8px;">${redata.content}</div>
      <div style="text-align: right;">
      ${redata.receiveTime}
      </div>`,
          position: 'bottom-right',
          duration: 8000
        })
      }
    },
    websocketsend(Data) { // 数据发送
      this.websock.send(Data)
      //
    },
    websocketclose(e) { // 关闭
      // console.log('关闭', e)
    },
    getMsgNum() {
      // 已读未读数据查询
      getNumByReadType({ readStatus: 0 }).then(res => {
        if (res.code === 200) {
          eventBus.$emit('getMsgCount', res.data)
        }
      })
    }

  }
}
</script>

<style lang="scss" scoped>
.title {
  margin: 20px auto 70px auto;
  text-align: center;
  width: 100%;
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
.rememberMe {
  margin: 0 0 25px 0;
}
.forget {
  float: right;
  color: #4686EF;
}
.tips{
  color:#EF5826;
  font-size:14px;
}
.tologin {
  width: 100%;
  height: 40px;
  background: #4686EF;
  font-size: 18px;
  font-family: SourceHanSansCN-Regular, SourceHanSansCN;
  font-weight: 400;
  color: #FFFFFF;
  line-height: 27px;
  text-shadow: 0px 2px 8px rgba(15,72,157,0.5);
}
.passw ::v-deep .el-input__inner{
  border-radius: 4px;
 // border: 1px solid #ff0000;
}
::v-deep .el-input__inner{
  height: 40px;
  line-height: 40px;
}
.icon-guanbi{
  font-size: 20px;
}
::v-deep .el-form-item__error{
  color: #EF5826;
  font-size: 14px;
}
::v-deep .el-input__inner:focus{
  border-color: #4686EF;
}
::v-deep .el-checkbox__input.is-checked + .el-checkbox__label{
  color: #4686EF;
}
::v-deep .el-checkbox__input.is-checked .el-checkbox__inner{
  background-color: #4686EF;
  border-color: #4686EF;
}
</style>
