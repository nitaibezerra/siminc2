<?php

if(isset($_POST["theme_simec"])){
	$theme = $_POST["theme_simec"];
	setcookie("theme_simec", $_POST["theme_simec"] , time()+60*60*24*30, "/");
} else {
	if(isset($_COOKIE["theme_simec"])){
		$theme = $_COOKIE["theme_simec"];
	}
}

	/** 
	 * Sistema Integrado de Monitoramento do Ministério da Educação
	 * Setor responsvel: SPO/MEC
	 * Desenvolvedor: Desenvolvedores Simec
	 * Analistas: Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>, Alexandre Soares Diniz
	 * Programadores: Renê de Lima Barbosa <renedelima@gmail.com>, Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>
	 * Módulo: Segurança
	 * Finalidade: Solicitação de cadastro de contas de usuário.
	 * Data de criação:
	 * Última modificação: 30/08/2006
	 */
	
	define("SIS_PDEESCOLA", 34);
	define("SIS_PSEESCOLA", 65);

	// carrega as bibliotecas internas do sistema
	include "config.inc";
	require APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";

	// abre conexão com o servidor de banco de dados
	$db = new cls_banco();
	
	if(!$theme) {
		$theme = $_SESSION['theme_temp'];
	}
	
	// Particularidade feita para o PDE Escola
	$selecionar_modulo_habilitado = 'S';
	if($_REQUEST['banner_pdeescola']=='acessodireto') {
		$selecionar_modulo_habilitado = 'N';
		$_REQUEST['sisid'] = SIS_PDEESCOLA;
	}
	if($_REQUEST['banner_pseescola']=='acessodireto') {
		$selecionar_modulo_habilitado = 'N';
		$_REQUEST['sisid'] = SIS_PSEESCOLA;
	}

	$sisid  		= $_REQUEST['sisid'];
	$modid  		= $_REQUEST['modid'];
	$usucpf 		= $_REQUEST['usucpf'];

	// leva o usuário para o passo seguinte do cadastro
	if ($_REQUEST['usucpf'] && $_REQUEST['modulo'] && $_REQUEST['varaux'] == '1') {
		$_SESSION = array();
		if($theme) $_SESSION['theme_temp'] = $theme;
		header("Location: cadastrar_usuario_2.php?sisid=$sisid&modid=$modid&usucpf=$usucpf");
		exit();
	}

?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>

<meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1">

<title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>
<?php if(is_file( "includes/layout/".$theme."/include_login.inc" )) include "includes/layout/".$theme."/include_login.inc"; ?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script> 
function ImprimeStatus(texto){ 
    document.formul.numCaracteres.value = texto
} 
</script> 
</head>

<body>
	<div id="tutorial_theme" style="display:none"><span style="color:red;font-weight:bold;">Novidade!</span><br>Agora você pode escolher o VISUAL do seu SIMEC, clique no ícone ao lado e experimente!</div>
	<? include "barragoverno.php"; ?>

<?php
	$mensagens = implode( '<br/>', (array) $_SESSION['MSG_AVISO'] );
	$_SESSION['MSG_AVISO'] = null;
	$titulo_modulo = 'Solicitação de Cadastro de Usuários';
	$subtitulo_modulo = 'Preencha os Dados Abaixo e clique no botão: "Continuar".<br/>'. obrigatorio() .' Indica Campo Obrigatório.'. $mensagens;
//	monta_titulo( $titulo_modulo, $subtitulo_modulo );
?>
<table width="100%" cellpadding="0" cellspacing="0" id="main">
<tr>
	<td width="80%" ><img src="/includes/layout/<? echo $theme ?>/img/logo.png" border="0" /></td>
	<td align="right" style="padding-right: 30px;padding-left:20px;" >
		<img src="/includes/layout/<? echo $theme ?>/img/bt_temas.png" style="cursor:pointer" id="img_change_theme" alt="Alterar Tema" title="Alterar Tema" border="0" />
		<div style="display:none" id="menu_theme">
		<script>
			
			$(document).ready(function() {
			        $().click(function () {
			        	$('#menu_theme').hide();
			        });
			        $("#img_change_theme").click(function () {
			        	$('#menu_theme').show();
			        	return false;
			        });
			        $("#menu_theme").click(function () {
			        	$('#menu_theme').show();
			        	return false;
			        });
			});
			
			function alteraTema(){
				document.getElementById('formTheme').submit();
			}
		</script>
		
		<form id="formTheme" action="" method="post" >
		Tema: 
			<select class="select_ylw" name="theme_simec" title="Tema do SIMEC" onchange="alteraTema(this.value)" >
		            <?php include(APPRAIZ."www/listaTemas.php") ?>
	        </select>
		     <?
				if($_POST) {
					foreach($_POST as $key => $var) {
						if($key != 'theme_simec') echo "<input type=hidden name='".$key."' value='".$var."'>";
					}
				}
		     ?>
		</form>
		</div>
		
	</td>
</tr>
<form method="post" name="formulario" id="formulario" onsubmit="return false;">
<input type=hidden name="modulo" value="./inclusao_usuario.php"/>
<input type=hidden name="varaux" value="">
<tr>
  <td colspan="2" width="100%" valign="top">
  
  <!-- Lista de Módulos-->
  <table width="98%" border="0" cellpadding="0" cellspacing="0" class="tabela_modulos">
  <tr>
  	<td class="td_bg">&nbsp;Solicitação de Cadastro de Usuários - <small>Preencha os Dados Abaixo e clique no botão: "Continuar"</small></td>
  </tr>
  <tr>
  	<td align="center">
  	<? if( strlen($mensagens) > 5 ){?>
	<div class="error_msg"><? echo (($mensagens)?$mensagens:""); ?></div>
	<? } ?>  	
	</td>
  </tr>
  <tr>
	<td valign="middle" class="td_table_inicio">
	<table width="95%">
	<tr>
		<td style="font-weight: bold;" align='right'>Módulos:</td>
		<td>
		<?php
		/*** Recupera todos os sistemas cadastrados ***/
		$sql = "SELECT
					s.sisid AS codigo,
					s.sisabrev AS descricao
				FROM
					seguranca.sistema s
				WHERE
					s.sisstatus = 'A'
					AND sismostra = 't'
				ORDER BY
					descricao";
		$sistemas = $db->carregar($sql);
		/*** Inicializa a variável para montagem do combo ***/
		$select = '';
		/*** Se existem sistemas cadastrados ***/
		if( $sistemas )
		{
			/*** Se o combo deve vir habilitado ***/
			if( $selecionar_modulo_habilitado == 'S' )
			{
				$disabled = '';
			}
			/*** Se o combo deve vir desabilitado ***/
			else
			{
				$disabled = 'disabled="disabled"';
			}
			
			/*** Inicia a montagem do combo ***/
			$select .= '<select class="CampoEstilo" style="width:auto;" name="sisid_modid" '.$disabled.' onchange="sel_modulo(this);">';
			$select .= '<option value="">Selecione...</option>';
			/*** Percorre o array com os sistemas ***/
			foreach($sistemas as $sistema)
			{
				/*** Recupera os módulos do sistema ***/
				$sql = "SELECT
							m.modid AS codigo,
							m.modtitulo as descricao
						FROM
							seguranca.modulo m
						WHERE
							m.sisid = {$sistema['codigo']}
							AND m.modstatus = 'A'";
				$modulos = $db->carregar($sql);
				/*** Se existirem módulos para o sistema ***/
				if( $modulos )
				{
					/*** Inclue o sistema como um grupo de opções na combo ***/
					$select .= '<optgroup id="'.$sistema['codigo'].'" label="'.$sistema['descricao'].'">';
					/*** Percorre o array com os módulos ***/
					foreach($modulos as $modulo)
					{
						$selected = '';
						/*** Se existe a variável de requisição do 'modid' ***/
						if( $modid )
						{
							if( $modid == $modulo['codigo'] )
							{
								$selected = 'selected="selected"';
							}
						}
						/*** Inclue o módulo como uma opção na combo, relacionado ao grupo de opções ***/
						$select .= '<option value="'.$modulo['codigo'].'" '.$selected.'>'.$modulo['descricao'].'</option>';
					}
					/*** Finaliza o grupo de opções da combo ***/
					$select .= '</optgroup>';
				}
				/*** Se não existirem módulos para o sistema ***/
				else
				{
					$selected = '';
					/*** Se existe a variável de requisição do 'modid' ***/
					if( !$modid && $sisid )
					{
						if( $sisid == $sistema['codigo'] )
						{
							$selected = 'selected="selected"';
						}
					}
						
					/*** Inclue o sistema como um grupo de opções na combo ***/
					$select .= '<optgroup id="" label="'.$sistema['descricao'].'">';
					/*** Inclue o sistema como uma opção na combo ***/
					$select .= '<option value="'.$sistema['codigo'].'" '.$selected.'>'.$sistema['descricao'].'</option>';
					/*** Finaliza o grupo de opções da combo ***/
					$select .= '</optgroup>';
				}
			}
			/*** Finaliza o combo ***/
			$select .= '</select>';
		}
		
		/*
		//Código usado anteriormente...
		$sql = "SELECT
					s.sisid AS codigo,
					s.sisabrev AS descricao
				FROM
					seguranca.sistema s
				WHERE
					s.sisstatus = 'A'
					AND sismostra = 't'
				ORDER BY
					descricao";
		$db->monta_combo( "sisid", $sql, $selecionar_modulo_habilitado, "&nbsp;", 'selecionar_modulo', '');
		*/
		
		/*** Inclue o objeto select já montado ***/
		echo $select;
		?>
		<?= obrigatorio(); ?>
		<input type="hidden" name="sisid" id="sisid" value="<?=$sisid?>" />
		<input type="hidden" name="modid" id="modid" value="<?=$modid?>" />
		</td>
	</tr>
	<?php if( $sisid ): ?>
		<tr>
			<td align='right' class="subtitulodireita">&nbsp;</td>
			<td>
				<?php
					$sql = sprintf( "select sisid, sisdsc, sisfinalidade, sispublico, sisrelacionado from sistema where sisid = %d", $sisid );
					$sistema = (object) $db->pegaLinha( $sql );
					if ( $sistema->sisid ) :
				?>
					<font color="#555555" face="Verdana">
						<b><?= $sistema->sisdsc ?></b><br/>
						<p><?= $sistema->sisfinalidade ?></p>
						<ul>
							<li><span style="color: #000000">Público-Alvo:</span> <?= $sistema->sispublico ?><br></li>
							<li><span style="color: #000000">Sistemas Relacionados:</span> <?= $sistema->sisrelacionado ?></li>
						</ul>
					</font>
				<?php endif; ?>
			</td>
		</tr>
	<?php endif; ?>
	<input type="hidden" name="sisfinalidade_selc" value="<?=$sisfinalidade_selc?>"/>
	
	<tr>
		<td style="font-weight: bold;" align='right'>CPF:</td>
		<td>
			<input id="usucpf" type="text" name="usucpf" value=<? print '"'.$usucpf.'"'; ?> class="login_input" onkeyup="this.value=mascaraglobal('###.###.###-##',this.value);" />
			<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.'>
		</td>
	</tr>
	<tr>
		<td>&nbsp;</td>
		<td>
			<a class="botao2" href="javascript:validar_formulario()" >Continuar</a>
			<a class="botao1" href="./login.php" >Voltar</a>
		</td>
	</tr>
	</table>  
    </td>
    
  </tr>
  </table>
  </td>
  </tr>
	<tr>
	  <td colspan="2" class="rodape"> Data do Sistema: <? echo date("d/m/Y - H:i:s") ?></td>
  </tr>
</table>


</form>

</body>
</html>	

<script language="javascript">

	function sel_modulo(obj)
	{
		if( obj.value == "" )
		{
			document.getElementById('sisid').value = "";
			document.getElementById('modid').value = "";
		}
		else
		{
			var option = obj.options[obj.selectedIndex];
			var sisid = option.parentNode.id;
	
			if( sisid == "" )
			{
				document.getElementById('sisid').value = obj.value;
				document.getElementById('modid').value = "";
			}
			else
			{
				document.getElementById('sisid').value = sisid;
				document.getElementById('modid').value = obj.value;
			}
			
			document.getElementById('formulario').submit();
		}
	}

	function selecionar_modulo()
    {
		document.formulario.submit();
	}

	function validar_formulario() 
    {
        var validacao = true;
        var mensagem  = '';

        if (document.formulario.sisid.value == "" ) {
            mensagem += '\nSelecione o módulo no qual você pretende ter acesso.';
            validacao = false;
        }
        
        if (document.formulario.usucpf.value == '' || !validar_cpf(document.formulario.usucpf.value)) {
            mensagem += '\nO cpf informado não é válido.';
            validacao = false;
        }

        document.formulario.varaux.value = '1'; 

        if ( !validacao ) {
            alert( mensagem );
        }else{
        	document.formulario.submit();
        }
	}
</script>