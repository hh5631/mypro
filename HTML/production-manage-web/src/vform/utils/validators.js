import { isEmptyStr, isNull } from "./util";

export const getRegExp = function (validatorName) {
  const commonRegExp = {
    number: '/^\\d+(\\.\\d+)?$/',
    int: '/^[0-9]+$/',
    letter: '/^[A-Za-z]$/',
    letterAndNumber: '/^[A-Za-z0-9]+$/',
    // 大写字母和数字
    upperLetterAndNumber: '/^[A-Z0-9]+$/',
    mobilePhone: /^(13[0-9]|14[01456879]|15[0-35-9]|16[2567]|17[0-8]|18[0-9]|19[0-35-9])\d{8}$/,
    allPhone: '/^((\\d{3,4}-\\d{7,8})|(\\d{7,8})|(1[3584]\\d{9}))$/',
    telPhone: '/^([0-9]{3,4}-)?[0-9]{7,8}$/',
    letterStartNumberIncluded: '/^[A-Za-z]+[A-Za-z\\d]*$/',
    noChinese: '/^[^\u4e00-\u9fa5]+$/',
    noNumber:/\D/,
    chinese: '/^[\u4e00-\u9fa5]+$/',
    chineseAndLetter: '/^[\u4e00-\u9fa5a-zA-Z]+$/',
    chineseAndLetterAndNumber: '/^[\u4e00-\u9fa5a-zA-Z0-9]+$/',
    email: '/^([-_A-Za-z0-9.]+)@([_A-Za-z0-9]+\\.)+[A-Za-z0-9]{2,3}$/',
    // 邮编
    zipCode: '/^\\d{6}$/',
    url: '/^([hH][tT]{2}[pP]:\\/\\/|[hH][tT]{2}[pP][sS]:\\/\\/)(([A-Za-z0-9-~]+)\\.)+([A-Za-z0-9-~\\/])+$/',
  }

  return commonRegExp[validatorName]
}

const validateFn = function (validatorName, rule, value, callback, defaultErrorMsg) {
  //空值不校验
  if (isNull(value) || (value.length <= 0)) {
    callback()
    return
  }
  const reg = eval(getRegExp(validatorName))
  if (!reg.test(value)) {
    let errTxt = rule.errorMsg || defaultErrorMsg
    callback(new Error(errTxt))
  } else {
    callback()
  }
}

const FormValidators = {

  /* 数字 */
  number(rule, value, callback) {
    validateFn('number', rule, value, callback, '[' + rule.label + ']包含非数字字符')
  },
  int(rule, value, callback) {
    validateFn('int', rule, value, callback, '[' + rule.label + ']仅支持数字')
  },
  // 更宽泛的手机,座机号码
  allPhone(rule, value, callback) {
    if(!value){
      callback()
      return
    }
    console.log('value',value)
    if (value.charAt(0) === '0') {
      validateFn('telPhone', rule, value, callback, '[' + rule.label + ']请输入正确的手机号或者电话号码')
    } else {
      validateFn('mobilePhone', rule, value, callback, '[' + rule.label + ']请输入正确的手机号或者电话号码')
    }
    // validateFn('mobilePhone', rule, value, callback, '[' + rule.label + ']请输入正确的手机号或者电话号码')
    // validateFn('telPhone', rule, value, callback, '[' + rule.label + ']请输入正确的手机号或者电话号码')
  },

  /* 字母 */
  letter(rule, value, callback) {
    validateFn('letter', rule, value, callback, '[' + rule.label + ']包含非字母字符')
  },
  upperLetterAndNumber(rule, value, callback) {
    validateFn('upperLetterAndNumber', rule, value, callback, '[' + rule.label + ']只支持大写字母和数字')
  },

  /* 字母和数字 */
  letterAndNumber(rule, value, callback) {
    validateFn('letterAndNumber', rule, value, callback, '[' + rule.label + ']只能输入字母或数字')
  },
  /* 字母和汉字 */
  chineseAndLetter(rule, value, callback) {
    validateFn('chineseAndLetter', rule, value, callback, '[' + rule.label + ']只能输入字母或汉字')
  },
  /* 字母和汉字和数字 */
  chineseAndLetterAndNumber(rule, value, callback) {
    validateFn('chineseAndLetterAndNumber', rule, value, callback, '[' + rule.label + ']只能输入字母、汉字或者数字')
  },

  /* 手机号码 */
  mobilePhone(rule, value, callback) {
    // validateFn('mobilePhone', rule, value, callback, '[' + rule.label + ']手机号码格式有误')
    validateFn('mobilePhone', rule, value, callback, '手机号码格式有误')

  },
  /* 邮编 */
  zipCode(rule, value, callback) {
    validateFn('zipCode', rule, value, callback, '[' + rule.label + ']必须是数字且长度为6')
  },

  /* 禁止空白字符开头 */
  noBlankStart(rule, value, callback) {
    //暂未实现
  },

  /* 禁止空白字符结尾 */
  noBlankEnd(rule, value, callback) {
    //暂未实现
  },

  /* 字母开头，仅可包含数字 */
  letterStartNumberIncluded(rule, value, callback) {
    validateFn('letterStartNumberIncluded', rule, value, callback, '[' + rule.label + ']必须以字母开头，可包含数字')
  },

  /* 禁止中文输入 */
  noChinese(rule, value, callback) {
    validateFn('noChinese', rule, value, callback, '[' + rule.label + ']不可输入中文字符')
  },

  /* 必须中文输入 */
  chinese(rule, value, callback) {
    validateFn('chinese', rule, value, callback, '[' + rule.label + ']只能输入中文字符')
  },
  /* 禁止输入数字 */
  noNumber(rule, value, callback) {
    validateFn('noNumber', rule, value, callback, '[' + rule.label + ']不可输入数字')
  },
  /* 电子邮箱 */
  email(rule, value, callback) {
    validateFn('email', rule, value, callback, '[' + rule.label + ']邮箱格式有误')
  },

  /* URL网址 */
  url(rule, value, callback) {
    validateFn('url', rule, value, callback, '[' + rule.label + ']URL格式有误')
  },

  /*测试
  test(rule, value, callback, errorMsg) {
    //空值不校验
    if (isNull(value) || (value.length <= 0)) {
      callback()
      return
    }

    if (value < 100) {
      callback(new Error('[' + rule.label + ']不能小于100'))
    } else {
      callback()
    }
  },
  */

  regExp(rule, value, callback) {
    //空值不校验
    if (isNull(value) || (value.length <= 0)) {
      callback()
      return
    }

    const pattern = eval(rule.regExp)
    if (!pattern.test(value)) {
      let errTxt = rule.errorMsg || '[' + rule.label + ']invalid value'
      callback(new Error(errTxt))
    } else {
      callback()
    }
  },

}

export default FormValidators
