<template>
  <div class="login">
    <!-- <div class="group1" />
      <div class="lan">
        <lan-selector />
      </div> -->
    <div class="topContent">
      <div class="group2" />
      <div class="lan2">
        <lan-selector />
      </div>
    </div>
    <div class="centerContent">
      <div class="box2">
        <img src="@/assets/login_images/hello.png">
      </div>
      <!-- 用户登录 -->
      <div>
        <login-login v-show="loginState==1&&!needSetPassword" @forgetPassword="updateloginState" />
        <!-- 点忘记密码后的界面 -->
        <login-forgetpassword v-show="loginState==2&&!needSetPassword" @nextStep="updateloginState" />
        <!-- <login-forgetpassword v-show="needSetPassword" @nextStep="updateloginState" /> -->
        <!-- 点下一步后的界面-->
        <login-nextstep v-show="loginState==3&&!needSetPassword" @goback="updateloginState" />
        <login-nextstep v-show="needSetPassword" @goback="updateloginState" />
      </div>
    </div>
    <div class="login-bottom">
      <div class="pic2-div">
        <img
          class="pic1"
          referrerpolicy="no-referrer"
          src="@/assets/login_images/logo.png"
        >
      </div>
      <div class="footer">
        <span class="word7">&nbsp;©&nbsp;Copyright&nbsp;Reserved</span>
        <span class="info6">浙江大学嘉兴研究院版权所有</span>
      </div>
    </div>
    <!-- <div class="box1 flex-col">
      <img src="@/assets/login_images/hello.png" size="50%">
    </div> -->

    <!-- 用户登录 -->
    <!-- <login-login v-show="loginState==1&&!needSetPassword" @forgetPassword="updateloginState" /> -->
    <!-- 点忘记密码后的界面 -->
    <!-- <login-forgetpassword v-show="loginState==2&&!needSetPassword" @nextStep="updateloginState" /> -->
    <!-- <login-forgetpassword v-show="needSetPassword" @nextStep="updateloginState" /> -->
    <!-- 点下一步后的界面-->
    <!-- <login-nextstep v-show="loginState==3&&!needSetPassword" @goback="updateloginState" /> -->

    <!--  底部  -->
    <!-- <div class="pic1-div">
      <img
        class="pic1"
        referrerpolicy="no-referrer"
        src="@/assets/login_images/logo.png"
      >
    </div>
    <div class="el-login-footer">
      <span class="word7">&nbsp;©&nbsp;Copyright&nbsp;Reserved</span>
      <span class="info6">浙江大学嘉兴研究院版权所有</span>
    </div> -->
  </div>
</template>

<script>

import lanSelector from '@/components/lan-selector/lan-selector.vue'
import LoginLogin from '../login-login/login-login.vue'
import LoginForgetpassword from '../login-forgetpassword/login-forgetpassword.vue'
import LoginNextstep from '../login-nextstep/login-nextstep.vue'

export default {
  name: 'Loginlayout',
  components: { lanSelector, LoginLogin, LoginForgetpassword, LoginNextstep },
  data() {
    return {
      loginState: 1,
      needSetPassword: this.$route.query.needSetPassword
    }
  },
  methods: {
    updateloginState(data) {
      this.loginState = data
      this.needSetPassword = false
    }
  }
}
</script>

<style rel="stylesheet/scss" lang="scss" scoped>
.login {
  // display: flex;
  // justify-content: center;
  // align-items: center;
   min-height: 100vh;
   min-width: 1036px;
  background-image: url('../../assets/login_images/background.png');
  background-size: cover;
  // position: relative;
  .lan {
    position: absolute;
    right: 40px;
    top: 35px;
  }
}
.topContent{
  display: flex;
  justify-content: space-between;
  width: 100%;
  padding-top: 40px;
    .group2 {
      width: 105px;
      height: 46px;
      background: url(../../assets/login_images/group_leftTop.png) 100% no-repeat;
      margin-left: 40px;
    }
    .lan2{
      margin-right: 40px;
    }
}
.centerContent{
  display: flex;
  justify-content: center;
  padding-top: 120px;
    .box2{
      width: 600px;
      height: 500px;
      border-radius: 8px 0px 0px 8px;;
      box-shadow: 0px 2px 8px 0px rgba(15, 72, 157, 0.5);
       img{
        //  width: 100%;
        //  height: 100%;
         width: 600px;
      height: 500px;
       }
    }
}

.login-bottom{
  display: flex;
  flex-direction: column;
  align-items: center;
  .pic2-div{
    padding-top: 80px;
  }
  .footer{
    padding-top: 20px;
  }
}
.el-login-footer {
  height: 100px;
  line-height: 100px;
  position: absolute;
  bottom: 0;
  width: 100%;
  text-align: center;
  color: #fff;
  font-family: Arial;
  font-size: 12px;
  letter-spacing: 1px;
}
.login-code-img {
  height: 38px;
}

.box1 {
  position: relative;
  left: 100px;
  top: -21px;
  height:500px;
  box-shadow: 0px 2px 8px 0px rgba(15, 72, 157, 0.5);
  img{
    width: 100%;
    height: 100%;
  }
}
.word7 {
  overflow-wrap: break-word;
  color: #15599F;
  font-size: 15px;
  font-family: ArialMT;
  text-align: left;
  white-space: nowrap;
  line-height: 25px;
}
.info6 {
  overflow-wrap: break-word;
  color: #15599F;
  font-size: 15px;
  text-align: left;
  white-space: nowrap;
  line-height: 25px;
}
.pic1-div {
  position: relative;
  left: -500px;
  top: 360px;
}
.pic1 {
  width: 237px;
  height: 64px;
  // top: 10px;
}
.group1 {
  width: 105px;
  height: 46px;
  position: absolute;
  top: 50px;
  left: 40px;
  background: url(../../assets/login_images/group_leftTop.png)
    100% no-repeat;
}
.lang {
  width: 40px;
  border-block-color: white;
  background-color: gold;
}
.forget {
  float: right;
  color: #417cec;
  position: absolute;
  top: 515px;
  left: 1340px;
}
</style>
