<?

function deletarUsuario($dados) {
	global $db;
	$sql = "UPDATE seguranca.envioerrosusuarios SET eeustatus='I' WHERE eeuid='".$dados['eeuid']."'";
	$db->executar($sql);
	$db->commit();
	
	echo "<script>alert('Deletado com sucesso');window.location='seguranca.php?modulo=principal/envioerrosusuarios&acao=A';</script>";
}

function associarSistemaUsuario($dados) {
	global $db;
	
	if($dados['sisid'] && $dados['eeuid']) {
		$db->executar("DELETE FROM envioerrosususistema WHERE eeuid='".$dados['eeuid']."'");
		
		foreach($dados['sisid'] as $sisid) {
			$sql = "INSERT INTO seguranca.envioerrosususistema(eeuid, sisid) VALUES ('".$dados['eeuid']."', '".$sisid."');";
			$db->executar($sql);
		}
		
		$db->commit();
		
		echo "<script>alert('Sistemas vinculados com sucesso');window.opener.location.href=window.opener.location.href;window.close();</script>";
		
	} else {
		echo "<script>alert('Não foram marcados sistemas');window.location='seguranca.php?modulo=principal/envioerrosusuarios&acao=A&requisicao=gerenciarUsuario&usucpf=".$dados['usucpf']."';</script>";
	}
	
}

function inserirUsuarioEnvio($dados) {
	global $db;
	
	$dados['usucpf'] = str_replace(array(".","-"),array("",""),$dados['usucpf']);
	
	$sql = "SELECT usucpf FROM seguranca.usuario WHERE usucpf='".$dados['usucpf']."'";
	$verifica_existe = $db->pegaUm($sql);
	
	if($verifica_existe) {
	
		$sql = "SELECT eeuid FROM seguranca.envioerrosusuarios WHERE usucpf='".$dados['usucpf']."'";
		$eeuid = $db->pegaUm($sql);
		
		if($eeuid) {
			$sql = "UPDATE seguranca.envioerrosusuarios SET eeustatus='A' WHERE usucpf='".$dados['usucpf']."'";
			$db->executar($sql);
			
		} else {
			$sql = "INSERT INTO seguranca.envioerrosusuarios(usucpf, eeustatus)
	    			VALUES ('".$dados['usucpf']."', 'A')";
			$db->executar($sql);
		}
		
		$db->commit();
		echo "<script>alert('Usuário ativado com sucesso');window.location='seguranca.php?modulo=principal/envioerrosusuarios&acao=A&requisicao=gerenciarUsuario&usucpf=".$dados['usucpf']."';</script>"; 
		
	} else {
		echo "<script>alert('Usuário não esta cadastrado no SIMEC. Favor solicitar Cadastro');window.location='seguranca.php?modulo=principal/envioerrosusuarios&acao=A&requisicao=gerenciarUsuario';</script>";
	}
	
}


function gerenciarUsuario($dados) {
	global $db;
	
	echo "<script language='JavaScript' src='../includes/funcoes.js'></script>";
	echo "<link rel='stylesheet' type='text/css' href='../includes/Estilo.css'/>";
	echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
	echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
	
	if($dados['usucpf']) {
		
		$sql = "SELECT u.usucpf, u.usunome, u.usuemail, e.eeuid FROM seguranca.usuario u INNER JOIN seguranca.envioerrosusuarios e ON u.usucpf = e.usucpf WHERE u.usucpf='".$dados['usucpf']."' AND eeustatus='A'";
		$dadosusr = $db->pegaLinha($sql);
		
		if(!$dadosusr) die("<script>alert('CPF inválido');window.close();</script>");
		
		
		echo "<script>
			  function validarAssociaSistema() {
			  document.getElementById('formulariousu').submit();
			  }
			  function marcarTodos(obj) {
			  if(obj.checked) {
			  jQuery(\"[name^='sisid[]']\").attr(\"checked\",\"checked\");
			  } else {
			  jQuery(\"[name^='sisid[]']\").attr(\"checked\",\"\");
			  }
			  }
			  </script>";
		
		echo "<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>
				<tr><td class='SubTituloDireita'>CPF:</td><td>".$dadosusr['usucpf']."</td></tr>
				<tr><td class='SubTituloDireita'>Nome:</td><td>".$dadosusr['usunome']."</td></tr>
				<tr><td class='SubTituloDireita'>Email:</td><td>".$dadosusr['usuemail']."</td></tr>
			  </table>";
		
		echo "<form method=post id=formulariousu action=seguranca.php?modulo=principal/envioerrosusuarios&acao=A&requisicao=associarSistemaUsuario>";
		echo "<input type=hidden name=eeuid value=".$dadosusr['eeuid'].">";
		echo "<input type=hidden name=usucpf value=".$dadosusr['usucpf'].">";
		echo "<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>
			  <tr><td class='SubTituloEsquerda'>Sistemas associados:</td></tr>
			  <tr><td><div style=\"height:300px;overflow:auto;\">";
		
		$sql = "SELECT sisid, sisdsc, (SELECT eesid FROM seguranca.envioerrosususistema e WHERE e.sisid=s.sisid AND e.eeuid='".$dadosusr['eeuid']."') as eesid FROM seguranca.sistema s WHERE sisstatus='A'";
		$sistemas = $db->carregar($sql);
		
		if($sistemas[0]) {
			echo "<table width=100% class=listagem>";
			echo "<tr><td colspan=2><input type=checkbox name=todos value=todos onclick=marcarTodos(this); ></td></tr>";
			foreach($sistemas as $sis) {
				echo "<tr><td><input type=checkbox name=sisid[] value=".$sis['sisid']." ".(($sis['eesid'])?"checked":"")."></td><td>".$sis['sisdsc']."</td></tr>";
			}
			echo "</table>";
		}
		
		echo "</div></td></tr>
			  <tr><td><input type=button name=sis value=Associar onclick=validarAssociaSistema();></td></tr></table>";
		echo "</form>";
		
	
	
	} else {

		echo "<script>
			  function validarGerenciaUsuario() {
			  if(document.getElementById('usucpf').value=='') {alert('Preencha um cpf');return false;}
			  document.getElementById('usucpf').value=mascaraglobal('###.###.###-##',document.getElementById('usucpf').value);
			  if(!validar_cpf(document.getElementById('usucpf').value)) {alert('CPF inválido');return false;}
			  document.getElementById('formulariousu').submit();
			  }
			  </script>";
		echo "<form method=post id=formulariousu action=seguranca.php?modulo=principal/envioerrosusuarios&acao=A&requisicao=inserirUsuarioEnvio>";
		echo "<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center><tr><td class='SubTituloDireita'>CPF:</td><td>";
		echo campo_texto('usucpf', "S", "S", "CPF", 16, 14, "###.###.###-##", "", '', '', 0, 'id="usucpf"' ); 
		echo   "<input type=button value=Inserir onclick=validarGerenciaUsuario();></td></tr></table>";
		echo "</form>";
		
		
	}
	
}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';

/* montando cabeçalho */
monta_titulo( "Envio de erros para usuários", "Gerenciamento do envio dos erros.");


?>
<script>

function abrirGerenciaUsuario(usucpf) {
	window.open('seguranca.php?modulo=principal/envioerrosusuarios&acao=A&requisicao=gerenciarUsuario&usucpf='+usucpf,'Usuario','scrollbars=no,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
}

function deletarUsuario(eeuid) {
	var conf = confirm('Deseja realmente deletar?');
	if(conf) {
		window.location='seguranca.php?modulo=principal/envioerrosusuarios&acao=A&requisicao=deletarUsuario&eeuid='+eeuid;
	}

}


</script>
<?
$sql = "SELECT u.usucpf, u.usunome, u.usuemail, e.eeuid FROM seguranca.envioerrosusuarios e INNER JOIN seguranca.usuario u ON u.usucpf = e.usucpf WHERE eeustatus='A'";
$usus = $db->carregar($sql);
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class='SubTituloCentro'>&nbsp;</td>
	<td class='SubTituloCentro'>Usuários</td>
	<td class='SubTituloCentro'>Email</td>
	<td class='SubTituloCentro'>&nbsp;</td>
</tr>
<? if($usus[0]) : ?>
<? foreach($usus as $usu) : ?>
<tr>
	<td width="5%"><center><img src="../imagens/alterar.gif" style="cursor:pointer;" border="0" onclick="abrirGerenciaUsuario('<?=$usu['usucpf'] ?>')"> <img src="../imagens/excluir.gif" style="cursor:pointer;" border="0" onclick="deletarUsuario('<?=$usu['eeuid'] ?>')"></center></td>
	<td><?=$usu['usunome'] ?></td>
	<td><?=$usu['usuemail'] ?></td>
	<td width="30%">
	<div style="height:100px;overflow:auto;">
	<?
	$sql = "SELECT s.sisdsc FROM seguranca.envioerrosususistema e INNER JOIN seguranca.sistema s ON s.sisid = e.sisid WHERE eeuid='".$usu['eeuid']."'";
	$db->monta_lista_simples($sql,array("Sistemas associados"),50,5,'N','100%',$par2);
	?>
	</div>
	</td>
</tr>
<? endforeach; ?>
<? else : ?>
<tr>
	<td colspan="4">Não exitem usuários cadastrados</td>
</tr>
<? endif; ?>
<tr>
	<td class='SubTituloCentro' colspan="4"><input type="button" name="botao" value="Inserir Novo" onclick="abrirGerenciaUsuario('');"></td>
</tr>
</table>