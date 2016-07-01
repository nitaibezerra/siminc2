<?
  /*
   Sistema Simec
   Setor responsvel: MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Hugo Morais
   Programador: Alexandre Dourado
   Módulo:inicio.inc
   Finalidade: Lista os campus cadastrados.
    */

if($_REQUEST['exec_function']) {
	$_REQUEST['exec_function']($_REQUEST);
}
/*
 * Variável : acao ($_REQUEST)
 * C => Lista campus
 * E => Excluir campus
 */
switch($_REQUEST['acao']) {
	case 'E':
		//dump($_REQUEST['cmpid']).die;
		$sql = "DELETE FROM academico.campusitem WHERE cmpid = '". $_REQUEST['cmpid'] ."'";
		$db->executar($sql);
//		$sql = "DELETE FROM sig.campussituacao WHERE cmpid = '". $_REQUEST['cmpid'] ."'";
//		$db->executar($sql);
		$sql = "DELETE FROM academico.campuscurso WHERE cmpid = '". $_REQUEST['cmpid'] ."'";
		$db->executar($sql);
		$sql = "DELETE FROM academico.processoseletivo WHERE cmpid = '". $_REQUEST['cmpid'] ."'";
		$db->executar($sql);
		$sql = "DELETE FROM academico.campus WHERE cmpid = '". $_REQUEST['cmpid'] ."'";
		$db->executar($sql);
		$db->commit();
		echo "<script>
				alert('O campus foi excluído com sucesso.');
				window.location = '?modulo=inicio&acao=C';
			  </script>";
		exit;
		break;
}
//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
include APPRAIZ . 'includes/Agrupador.php';
echo "<br/>";

// Verificando segurança
$permissoes = verificaPerfilSig();

if(!$_REQUEST['orgid'] && count($permissoes['vertipoensino']) > 0) {
	$_REQUEST['orgid'] = current($permissoes['vertipoensino']);

}
if($permissoes['vertipoensino'][0]) {
	validaAcessoTipoEnsino($permissoes['vertipoensino'],$_REQUEST['orgid']);
}
// Fim segurança

// Monta menu contendo os tipos de ensino (DE ACORDO COM O PERFIL)
if($permissoes['vertipoensino'][0]) {
	$sql = "SELECT orgid AS id, orgdesc AS descricao, '/sig/sig.php?modulo=inicio&acao=C&orgid=' || orgid AS link  
			FROM academico.orgao 
			WHERE orgid IN('".implode("','",$permissoes['vertipoensino'])."')";
	$menutipoensino = $db->carregar($sql);
	if ($menutipoensino){
   		echo montarAbasArray($menutipoensino, '/sig/sig.php?modulo=inicio&acao=C&orgid='.$_REQUEST['orgid']);
	}
	/* mostrando apenas as entidades vinculadas ao seu perfil */
	if(count($permissoes['verunidade'][$_REQUEST['orgid']]) > 0) {
		$filtroentidade = "AND entuo.entid IN('".implode("','",$permissoes['verunidade'][$_REQUEST['orgid']])."')";
	}
	
	/* Analisando filtro de pesquisa */
	if($_REQUEST['unidadeorcid']) {
		$filtropesquisa .= " AND entuo.entid='".$_REQUEST['unidadeorcid']."'";
	}
	if($_REQUEST['campusid']) {
		$filtropesquisa .= " AND ent.entid='".$_REQUEST['campusid']."'";
	}
	if($_REQUEST['cmpnome']) {
		$filtropesquisa .= " AND ent.entnome ILIKE '%".$_REQUEST['cmpnome']."%'";
	}
	/* FIM - Analisando filtro de pesquisa */
	
	echo "<script language=\"JavaScript\" src=\"./js/sig.js\"></script>";
	$titulo_modulo = "SIG";
	monta_titulo( $titulo_modulo, 'Sistema de Informações Gerenciais' );
	
?>
<form method="post" id="formpesquisa" name="formpesquisa">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr><td class="SubTituloCentro" colspan="2">Argumentos de Pesquisa</td></tr>
<tr><td class="SubTituloDireita">Agrupamento :</td><td><input type="radio" name="pes_agrupamento" value="uo" <? echo (($_REQUEST['pes_agrupamento']=="uo"||!$_REQUEST['pes_agrupamento'])?"checked":""); ?> > Unidades Orçamentárias <input type="radio" name="pes_agrupamento" value="cu" <? echo (($_REQUEST['pes_agrupamento']=="cu")?"checked":""); ?> > Campus / Uned</td></tr>
<tr><td class="SubTituloDireita">Unidade Orçamentária :</td><td><? $unidadeorcid = $_REQUEST['unidadeorcid']; $db->monta_combo('unidadeorcid', "SELECT entuo.entid as codigo, upper(entuo.entnome) as descricao FROM academico.campus cam 
																																				LEFT JOIN entidade.entidade ent ON ent.entid = cam.entid 
																																				LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid   
																																				LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
																																				LEFT JOIN entidade.entidade entuo ON fea.entid = entuo.entid 
																																				LEFT JOIN entidade.funcaoentidade fen2 ON fen2.entid = entuo.entid 
																																				LEFT JOIN academico.orgaouo teu ON teu.funid = fen2.funid 
																																				WHERE teu.orgid = " . $_REQUEST['orgid'] . " ".$filtroentidade." 
																																				GROUP BY entuo.entnome,entuo.entid ORDER BY entuo.entnome", 'S', "Selecione...", '', '', '', '', 'S', 'unidadeorcid'); ?></td></tr>
<tr><td class="SubTituloDireita">Campus / Uned :</td><td><? $campusid = $_REQUEST['campusid']; $db->monta_combo('campusid', "SELECT ent.entid as codigo, ent.entnome as descricao FROM academico.campus cam 
																															 LEFT JOIN entidade.entidade ent ON ent.entid = cam.entid 
																															 LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid   
																															 LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
																															 LEFT JOIN entidade.entidade entuo ON fea.entid = entuo.entid 
																															 LEFT JOIN entidade.funcaoentidade fen2 ON fen2.entid = entuo.entid 
																															 LEFT JOIN academico.orgaouo teu ON teu.funid = fen2.funid 
																															 WHERE teu.orgid = " . $_REQUEST['orgid'] . " ".$filtroentidade." GROUP BY ent.entnome,ent.entid ORDER BY ent.entnome", 'S', "Selecione...", '', '', '', '', 'S', 'campusid'); ?></td></tr>
<tr><td class="SubTituloDireita">Pesquisa por campus / uned :</td><td><? $cmpnome = $_REQUEST['cmpnome']; echo campo_texto('cmpnome', "N", "S", "Data de inauguração", 70, 100, "", "", '', '', 0, 'id="cmpnome"' ); ?></td></tr>
<tr><td align="center" colspan="2"><input type="button" onclick="this.disabled=true;document.getElementById('formpesquisa').submit();" value="Pesquisar"> <input type="submit" onclick="this.disabled=true;document.getElementById('cmpnome').disabled=true;document.getElementById('unidadeorcid').disabled=true;document.getElementById('campusid').disabled=true;" value="Ver Todos"></td></tr>
</table>
</form>
<?
	if($_REQUEST['pes_agrupamento']=="uo" || !$_REQUEST['pes_agrupamento']) {
		
		$sql = "SELECT
    		    '<img src=\"../imagens/mais.gif\" style=\"padding-right: 5px\" border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\" id=\"img' || entuo.entid || '\" name=\"+\" onclick=\"abreconteudo(\'sig.php?modulo=inicio&acao=C&orgid=".$_REQUEST['orgid']."&exec_function=carregacampus&unidade='|| entuo.entid ||' \',' || entuo.entid || ');\"/>' as img,
		        '<a style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=principal/editarentidade&acao=A&iscampus=nao&orgid=".$_REQUEST['orgid']."&entid='|| entuo.entid ||'\';\">' ||upper(entuo.entnome)|| '</a>',
		        COUNT(distinct cam.cmpid) as total,
		        '<tr><td style=\"padding:0px;margin:0;\"></td><td id=\"td' || entuo.entid || '\" colspan=\"2\" style=\"padding:0px;display:none;border: 5px red\"></td><td style=\"padding:0px;margin:0;\"></td></tr>' as tr
				FROM academico.campus cam
				LEFT JOIN entidade.entidade ent ON ent.entid = cam.entid
				LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid   
				LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
				LEFT JOIN entidade.entidade entuo ON fea.entid = entuo.entid 
				LEFT JOIN entidade.funcaoentidade fen2 ON fen2.entid = entuo.entid
				LEFT JOIN academico.orgaouo teu ON teu.funid = fen2.funid
				WHERE teu.orgid = " . $_REQUEST['orgid'] . " ".$filtroentidade." ".$filtropesquisa."
    			GROUP BY entuo.entnome,entuo.entid 
		    	ORDER BY entuo.entnome";
		
		$cabecalho = array( "", "Unidade", "Qtd. Campus/Unidade", "");
	} else {
		
		if($permissoes['remover']) {
			$excluircampus = "<img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"Excluir(\'?modulo=inicio&acao=E&cmpid=' || cam.cmpid || '\',\'Deseja realmente excluir este campus?\');\">";
		}
		
		$sql = "SELECT
				'<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=principal/editarcampus&acao=A&orgid=".$_REQUEST['orgid']."&cmpid=' || cam.cmpid || '\';\"> ".$excluircampus."' as acao,
		        '<a style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=principal/editarcampus&acao=A&orgid=".$_REQUEST['orgid']."&cmpid=' || cam.cmpid || '\';\">' ||ent.entnome|| '</a>',
		        entuo.entnome,
		        estdescricao,
		        mundescricao
				FROM academico.campus cam
				LEFT JOIN entidade.entidade ent ON ent.entid = cam.entid
				LEFT JOIN entidade.entidadeendereco edo ON edo.entid = ent.entid
				LEFT JOIN entidade.endereco en ON en.endid = edo.endid
				LEFT JOIN territorios.estado est ON est.estuf = en.estuf
				LEFT JOIN territorios.municipio mun ON mun.muncod = en.muncod
				LEFT JOIN entidade.entidade entuo ON ent.entidassociado = entuo.entid
				LEFT JOIN academico.orgaouo teu ON teu.funid = entuo.funid
				WHERE teu.orgid = " . $_REQUEST['orgid'] . " ".$filtroentidade." ".$filtropesquisa."
    			GROUP BY ent.entnome,ent.entid,cam.cmpid,entuo.entnome,estdescricao,mundescricao
		    	ORDER BY ent.entnome";
		$cabecalho = array( "", "Campus/Uned","Unidade Orçamentária","Estado","Município");
	}


	$db->monta_lista( $sql, $cabecalho, 100, 10, 'S', 'center', '' );
	if($permissoes['gravar']) {
	?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr bgcolor="#C0C0C0">
		<td colspan="2">
			<div style="float: left;">
				<input type="button" name="cadastrar_campus" id="cadastrar_campus" value="Cadastrar Campus/Unidade" onclick="this.disabled=true;window.location = '?modulo=principal/cadastrarcampus&acao=A&orgid=<?php echo ($_REQUEST['orgid']?$_REQUEST['orgid']:TIPOENSINO_DEFAULT); ?>';" <?php echo $disabled; ?>/>
			</div>
		</td>
	</tr>
	</table>
	<?
	}
} else {
	?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr bgcolor="red">
		<td colspan="2" align="center">NÃO FOI ATRIBUÍDO NENHUM <strong>TIPO DE ENSINO</strong> AO SEU PERFIL. ENTRE EM CONTATO COM ADMINISTRADOR DO SISTEMA.</td>
	</tr>
	</table>
	<?	
}
?>