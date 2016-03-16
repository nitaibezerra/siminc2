<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */

class LoginModel extends Model{

    public function __construct($dao) {
    	parent::__construct($dao);
    }

	public function verificaUsuario($user) {
		$sql = "SELECT * FROM  tb_reuni_usuario WHERE ds_login='$user'";
		$this->dao->fetch($sql);
		$usuario = $this->dao->getRow();
		if (!$usuario) {
			return false;
		}
		return true;
	}

	public function verificaSenha($user,$password) {
		$password = bin2hex(md5(sha1($password)));
		$sql = "SELECT * FROM  tb_reuni_usuario WHERE ds_login='$user' AND ds_senha='$password'";
		$this->dao->fetch($sql);
		$usuario = $this->dao->getRow();
		if (!$usuario) {
			return false;
		}
		return $usuario;
	}

}
?>
