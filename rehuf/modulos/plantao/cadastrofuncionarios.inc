<?php

/* Segurança : Validando os acessos as funções autorizadas */
switch($_REQUEST['acao']) {
	case 'A';
		$_FUNCOESAUTORIZADAS = array('inserirFuncionariosPlantao'=>true,
									 'atualizarFuncionariosPlantao'=>true,
									 'excluirFuncionariosPlantao'=>true);
	break;
}
/* Fim Segurança : Validando os acessos as funções autorizadas */


if($_REQUEST['requisicao']) {
	if($_FUNCOESAUTORIZADAS[$_REQUEST['requisicao']]) {
		$_REQUEST['requisicao']($_REQUEST);
		exit;
	}
}

if(validaVariaveisSistema()) {
	echo "<script>
			alert('Problemas nas variáveis do sistema.');
			window.location='?modulo=inicio&acao=A';
		  </script>";
	exit;
}


switch($_REQUEST['vis']) {
	
	case 'edicao':
		echo "<script language=\"JavaScript\" src=\"../includes/funcoes.js\"></script>";
		echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>";
		echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
		echo "<script language=\"JavaScript\" src=\"../includes/prototype.js\"></script>";
		echo "<script language=\"JavaScript\" src=\"./js/rehuf.js\"></script>";
		
		if($_REQUEST['fcoid']) {
			$sql = "SELECT * FROM rehuf.funcionarioplantao WHERE fcoid='".$_REQUEST['fcoid']."'";
			$funcionario = $db->pegaLinha($sql);
			$fcoid  = $funcionario['fcoid'];
			$fcocodigosiape  = $funcionario['fcocodigosiape'];
			$fconome = $funcionario['fconome'];
			$carid = $funcionario['carid'];
			$entid = $funcionario['entid'];
			$requisicao = "atualizarFuncionariosPlantao"; 
		} else {
			$requisicao = "inserirFuncionariosPlantao";
		}
?>
		<form method='post' name='formulario' onsubmit='return validarFuncionarioPlantao();'>
		<input type='hidden' name='fcoid' value='<? echo $fcoid; ?>'>
		<input type='hidden' name='requisicao' value='<? echo $requisicao; ?>'>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td colspan="2" class="SubTituloCentro">Cadastro de Funcionários</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Código do SIAPE:</td>
			<td><? echo campo_texto("fcocodigosiape", "N", "S", "Código do SIAPE", 13, 10, "", "", '', '', 0, "id='fcocodigosiape'"); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Nome:</td>
			<td><? echo campo_texto("fconome", "N", "S", "Nome do funcionário", 50, 200, "", "", '', '', 0, "id='fconome'"); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Cargo:</td>
			<td>
			<? 
			$sql = "SELECT c.carid as codigo, c.carnome as descricao FROM rehuf.cargoplantao c 
					LEFT JOIN rehuf.cargohospitalplantao chp ON chp.carid=c.carid 
					WHERE entid='".$_SESSION['rehuf_var']['entid']."' AND c.carstatus='A'";
			$db->monta_combo('carid', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'carid');
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" colspan="2"><input type="submit" value="Gravar"> <input type="button" value="Voltar" onclick="window.close();"></td>
		</tr>
		</table>
		</form>
<?		
		break;

	default:
		include APPRAIZ ."includes/cabecalho.inc";
		echo '<br>';
		
		echo "<script language=\"JavaScript\" src=\"../includes/prototype.js\"></script>";
		echo "<script language=\"JavaScript\" src=\"./js/rehuf.js\"></script>";
		
		/* montando o menu */
		echo montarAbasArray(carregardadosmenuplantao(), $_SERVER['REQUEST_URI']);
		/* fim montando o menu */
		/* montando cabeçalho */
		monta_titulo( "REHUF", "Programa Nacional de Re-estruturação dos Hospitais Universitários Federais");
		monta_cabecalho_rehuf($_SESSION['rehuf_var']['entid']);
		/* fim montando cabeçalho */
?>
		<form method='post' action='?modulo=plantao/cadastrofuncionarios&acao=A'>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td colspan="2" class="SubTituloCentro">Pesquisar</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">SIAPE:</td>
			<td><? echo campo_texto('fcocodigosiape', "S", "S", "SIAPE", 13, 10, "", "", '', '', 0, '' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Nome:</td>
			<td><? echo campo_texto('fconome', "S", "S", "Nome do funcionário", 67, 150, "", "", '', '', 0, '' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2"><input type="submit" value="Pesquisar"> <input type="button" value="Todos" onclick="window.location='?modulo=plantao/cadastrofuncionarios&acao=A';"> <input type='button' onclick="inserirFuncionarios('');" value='Incluir'></td>
		</tr>
		</table>
		</form>
<?
		if($_POST['fcocodigosiape']) {
			$filtro[] = "TRIM(fcocodigosiape) ilike '%".$_POST['fcocodigosiape']."%'";
		}
		if($_POST['fconome']) {
			$filtro[] = "fconome ilike '%".$_POST['fconome']."%'";
		}
		
		$sql = "SELECT '<center><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"inserirFuncionarios(\''||fup.fcoid||'\');\"> <img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"Excluir(\'?modulo=plantao/cadastrofuncionarios&acao=A&requisicao=excluirFuncionariosPlantao&fcoid='||fup.fcoid||'\', \'Deseja realmente excluir este funcionário?\');\"></center>', fcocodigosiape, fconome, carnome 
				FROM rehuf.funcionarioplantao fup 
				LEFT JOIN rehuf.cargoplantao car ON car.carid=fup.carid 
				LEFT JOIN rehuf.funcionarioplantaohospital fun ON fun.fcoid=fup.fcoid 
				WHERE fun.entid='".$_SESSION['rehuf_var']['entid']."' AND fcostatus='A' ".(($filtro)?" AND ".implode("AND ", $filtro):"")." 
				ORDER BY fup.fconome";
		
		$cabecalho = array("", "Código do SIAPE", "Nome", "Cargo");
		$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2);
}
?>
