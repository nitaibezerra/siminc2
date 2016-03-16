<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */

class LoginView extends View {
	private $erro;
	private $erros;
	private $login;

    public function __construct($model,$erro=null,$login=null) {
    	parent::__construct($model);
    	$this->erro = $erro;
    	$this->login = $login;
    }

    public function display() {
    	$this->menu=null;
		$classusuario='normal';
		$classsenha='normal';
		if ($this->erro==="Usuario") {
			$classusuario='erro';
			$this->erros[] = 'Usuário desconhecido.';
		}
		if ($this->erro==="Senha") {
			$classsenha='erro';
			$this->erros[] = 'Senha Incorreta.';
		}
		if ($this->erro==="Banco") {
			$this->erros[] = 'Erro de acesso ao Banco de Dados.<BR>Contacte o Administrador do sistema.';
		}

		$this->output.='
			<div id="login">
			<br/>
			<form action="index.php" method="post">
			<table class="formulario" width="200" align="center">
			<tr>
				<th colspan="2" class="center">Acesso ao Sistema</th>
			</tr>
			<tr>
				<th class="valoresitens" nowrap>Usuário:</th>
				<td><input class="'.$classusuario.'" type="text" name="usuario" value="'.$this->login.'"autocomplete="off" size="30" onfocus="MouseClick(this);" onblur="MouseBlur(this);"></td>
			</tr>
			<tr>
				<th class="valoresitens" nowrap>Senha:</th>
				<td><input class="'.$classsenha.'" type="password" name="senha" autocomplete="off" size="30""></td>
			</tr>
			<tr>
				<th  class="barrabotoes" colspan="2">
					<input type="submit" class="formbutton" name="Entrar" value="Acessar"/>
				</th>
			</tr>
			<tr><th class="esp">&nbsp;</th></tr>
			';
			if ($this->erros) {
				foreach ($this->erros as $er) {
					$this->output.='<tr><td colspan="2" class="erroinfo">'.$er.'</td></tr>';
				};
			}
			$this->output.= '</table></form><br>
			</div>
			';
        	parent::display();
    }

}

?>
