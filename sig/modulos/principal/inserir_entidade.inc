<?php

require_once APPRAIZ . "includes/classes/entidades.class.inc";

if($_REQUEST['exec_function']) {
	header('Content-Type: text/html; charset=ISO-8859-1');
	$_REQUEST['exec_function']($_REQUEST);
}elseif($_REQUEST['opt']=="buscarCnpj") {
	
	buscarCnpj($_REQUEST);
}
//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
include APPRAIZ . 'includes/Agrupador.php';
echo "<br/>";
?>
<script language="JavaScript" src="../includes/calendario.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="../includes/entidades.js"></script>
<script language="JavaScript" src="./js/sig.js"></script>
<?
if($_REQUEST['entid'] && $_REQUEST['orgid'] && $_REQUEST['iscampus']) {
	$_SESSION['sig_var']['entid'] = $_REQUEST['entid'];
	$_SESSION['sig_var']['iscampus'] = $_REQUEST['iscampus'];
}

if(!$_REQUEST['page']) {
	$_REQUEST['page'] = 'ent';	
}

// Verificando a segurança
$permissoes = verificaPerfilSig();
validaAcessoTipoEnsino($permissoes['vertipoensino'],$_SESSION['sig_var']['orgid']);
// Fim segurança

$permissoes['gravar'] = false;

/*
 * Criando regra especial, quando for a opção "inserir campus/uned", apresentar a aba do pai,
 * ou seja, a aba referente a lista de Uned/Campus
 */
switch($_REQUEST['page']) {
	case 'novoent':
		$pageparam = 'cam';
		break;
	default:
		$pageparam = $_REQUEST['page'];
}

echo montarAbasArray(carregardadosmenusig(), "/sig/sig.php?modulo=principal/inserir_entidade&acao=A&page=".$pageparam);

//montando cabeçalho
monta_cabecalho_sig($_SESSION['sig_var']['entid']);

// verificando em qual página o programa deve ir
switch($_REQUEST['page']) {
	/*
	 * Tela das obras cadastradas (selecionar quais vão ser inauguradas)
	 */
	case 'obr':
		if($_SESSION['sig_var']['iscampus'] == 'sim') {
			$filtro = "e.entid='".$_SESSION['sig_var']['entid']."'"; 
		} elseif($_SESSION['sig_var']['iscampus'] == 'nao') {
			$filtro = "au.entid='".$_SESSION['sig_var']['entid']."'";
		} else {
			echo "<script>alert('Problemas com as variaveis de sessão.');window.location='?modulo=inicio&acao=C';<script>";
			exit;
		}
		
		$_SESSION['downloadfiles']['pasta'] = array("origem" => "obras","destino" => "sig");
		
		?>
		<form action="sig.php?modulo=principal/inserir_entidade&acao=A&exec_function=salvarObrasInaugurada" method="post" name="formulario" id="formulario">
		<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr><td class="SubTituloEsquerda">Obra(s) a ser(em) inaugurada(s)</td></tr>
		<tr><td id="td_serinaugurada"></td></tr>
		<script>
		ajaxatualizar('exec_function=carregarlistaobras&buscar=serinaugurada','td_serinaugurada');
		</script>
		<tr><td class="SubTituloDireita"><input type="button" value="Inserir/Remover obra a ser inaugurada" disabled="disabled"></td></tr>
		<tr><td class="SubTituloEsquerda">Outra(s) obra(s) do campus/uned</td></tr>
		<tr><td id="td_outras"></td></tr>
		<script>
		ajaxatualizar('exec_function=carregarlistaobras&buscar=outras','td_outras');
		</script>
		</table>
		</form>
		<?
		break;
	/*
	 * Tela da lista de campus (somente visualizada para unidade orçamentária)
	 */
	case 'cam':
 		$sql = "SELECT DISTINCT
				'<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=principal/editarcampus&acao=A&orgid=".$_SESSION['sig_var']['orgid']."&cmpid=' || cam.cmpid || '\'\");\">' as acao,
    			'<a style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=principal/editarcampus&acao=A&orgid=".$_SESSION['sig_var']['orgid']."&cmpid=' || cam.cmpid || '\';\">'|| e.entnome||'</a>' as campus , 
		    	 en.estuf AS uf,
				 m.mundsc AS municipio 
				FROM entidade.entidade e 
				INNER JOIN entidade.funcaoentidade fen ON fen.entid = e.entid 
				INNER JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
				INNER JOIN entidade.entidade au ON fea.entid = au.entid 
				INNER JOIN entidade.funcaoentidade fen2 ON fen2.entid = au.entid
				INNER JOIN academico.orgaouo teu ON teu.funid = fen2.funid
				INNER JOIN academico.campus cam on e.entid = cam.entid					
				LEFT JOIN entidade.endereco en ON( e.entid = en.entid AND en.tpeid = 1)
				LEFT JOIN public.municipio m on m.muncod = en.muncod   
				WHERE fea.entid = '". $_SESSION['sig_var']['entid'] ."'";
    			
		$cabecalho = array( "Ação", "Nome do campus/unidade", "UF", "Município");
		$db->monta_lista($sql,$cabecalho,50,5,'N','center',$par2);
		?>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr bgcolor="#C0C0C0">
			<td colspan="2"><? if($permissoes['inserircampusuned']) { ?><input type='button' onclick="window.location='?modulo=principal/inserir_entidade&acao=A&page=novoent';this.disabled=true" value='Inserir campus/uned'><? } ?></td>
		</tr>
		</table>
		<?
		break;
		
	case 'dir':
		$entidade = new Entidades();
		$entidade->carregarPorFuncaoEntAssociado(($_SESSION['sig_var']['iscampus']=='sim')?$_funcoes[$_SESSION['sig_var']['orgid']]['campus']:$_funcoes[$_SESSION['sig_var']['orgid']]['unidade'],$_SESSION['sig_var']['entid']);
		echo $entidade->formEntidade("sig.php?modulo=principal/inserir_entidade&acao=A&exec_function=salvarRegistroDirigente",
									 array("funid" => ($_SESSION['sig_var']['iscampus']=='sim')?$_funcoes[$_SESSION['sig_var']['orgid']]['campus']:$_funcoes[$_SESSION['sig_var']['orgid']]['unidade'], "entidassociado" => $_SESSION['sig_var']['entid']),
									 array("enderecos"=>array(1))
									 );
		?>
		<script type="text/javascript">
		<? if(!$permissoes['gravar']) { ?>
		$('btngravar').disabled=true;
		<? } ?>
		$('frmEntidade').onsubmit  = function(e) {
			if (trim($F('entnumcpfcnpj')) == '') {
				alert('CPF é obrigatório.');
		    	return false;
			}
			if (trim($F('entnome')) == '') {
				alert('O nome da entidade é obrigatório.');
				return false;
			}
			if (trim($F('entdatanasc')) != '') {
				if(!validaData(document.getElementById('entdatanasc'))) {
					alert("Data de nascimento é inválida.");return false;
				}
			}
			return true;
		}
		</script>
		<?
		break;
	case 'int':
		$entidade = new Entidades();
		$entidade->carregarPorFuncaoEntAssociado(INTERLOCUTORINS,$_SESSION['sig_var']['entid']);
		echo $entidade->formEntidade("sig.php?modulo=principal/inserir_entidade&acao=A&exec_function=salvarRegistroInterlocutor",
									 array("funid" => INTERLOCUTORINS, "entidassociado" => $_SESSION['sig_var']['entid']),
									 array("enderecos"=>array(1))
									 );
	
		?>
		<script type="text/javascript">
		<? if(!$permissoes['gravar']) { ?>
		$('btngravar').disabled=true;
		<? } ?>
		$('frmEntidade').onsubmit  = function(e) {
			if (trim($F('entnumcpfcnpj')) == '') {
				alert('CPF é obrigatório.');
		    	return false;
			}
			if (trim($F('entnome')) == '') {
				alert('O nome da entidade é obrigatório.');
				return false;
			}
			if (trim($F('entdatanasc')) != '') {
				if(!validaData(document.getElementById('entdatanasc'))) {
					alert("Data de nascimento é inválida.");return false;
				}
			}
			return true;
		}
		</script>
		<?
		break;
	case 'ent':
		$entidassociado = $db->pegaUm("SELECT 
											fea.entid 
									   FROM 
									   		entidade.entidade ent 
									   LEFT JOIN 
									   		entidade.funcaoentidade fen ON fen.entid = ent.entid 
									   LEFT JOIN 
									   		entidade.funentassoc fea ON fea.fueid = fen.fueid 
									   WHERE 
									   		fen.funid='".(($_SESSION['sig_var']['iscampus']=='sim')?$_funcoesentidade[$_SESSION['sig_var']['orgid']]['campus']:$_funcoesentidade[$_SESSION['sig_var']['orgid']]['unidade'])."' AND fen.entid='".$_SESSION['sig_var']['entid']."'");
	
		$entidade = new Entidades();
		$entidade->carregarPorEntid($_SESSION['sig_var']['entid']);
		echo $entidade->formEntidade("sig.php?modulo=principal/inserir_entidade&acao=A&exec_function=salvarRegistroEntidade",
									 array("funid" => ($_SESSION['sig_var']['iscampus']=='sim')?$_funcoesentidade[$_SESSION['sig_var']['orgid']]['campus']:$_funcoesentidade[$_SESSION['sig_var']['orgid']]['unidade'], "entidassociado" => $entidassociado),
									 array("enderecos"=>array(1))
									 );
	
		?>
		<script type="text/javascript">
		document.getElementById('tr_entcodent').style.display = 'none';
		document.getElementById('tr_entnuninsest').style.display = 'none';
		document.getElementById('tr_entungcod').style.display = 'none';
		document.getElementById('tr_tpctgid').style.display = 'none';
		
		/*
		 * DESABILITANDO O CAMPO DE CNPJ
		 */
		document.getElementById('entnumcpfcnpj').readOnly = true;
		document.getElementById('entnumcpfcnpj').className = 'disabled';
		document.getElementById('entnumcpfcnpj').onfocus = "";
		document.getElementById('entnumcpfcnpj').onmouseout = "";
		document.getElementById('entnumcpfcnpj').onblur = "";
		document.getElementById('entnumcpfcnpj').onkeyup = "";
		
		/*
		 * DESABILITANDO O CÓDIGO DA UNIDADE
		 */
		document.getElementById('entunicod').readOnly = true;
		document.getElementById('entunicod').className = 'disabled';
		document.getElementById('entunicod').onfocus = "";
		document.getElementById('entunicod').onmouseout = "";
		document.getElementById('entunicod').onblur = "";
		document.getElementById('entunicod').onkeyup = "";
		
		/*
		 * DESABILITANDO O NOME DA ENTIDADE
		 */
		document.getElementById('entnome').readOnly = true;
		document.getElementById('entnome').className = 'disabled';
		document.getElementById('entnome').onfocus = "";
		document.getElementById('entnome').onmouseout = "";
		document.getElementById('entnome').onblur = "";
		document.getElementById('entnome').onkeyup = "";
		
		/*
		 * DESABILITANDO A SIGLA DA ENTIDADE
		 */
		document.getElementById('entsig').readOnly = true;
		document.getElementById('entsig').className = 'disabled';
		document.getElementById('entsig').onfocus = "";
		document.getElementById('entsig').onmouseout = "";
		document.getElementById('entsig').onblur = "";
		document.getElementById('entsig').onkeyup = "";

    	</script>
		<?
	break;
	
	case 'novoent':
		$entidade = new Entidades();
		echo $entidade->formEntidade("sig.php?modulo=principal/inserir_entidade&acao=A&exec_function=salvarNovoCampusUned",
									 array("funid" => $_funcoesentidade[$_SESSION['sig_var']['orgid']]['campus'], "entidassociado" => $_SESSION['sig_var']['entid']),
									 array("enderecos"=>array(1))
									 );
		?>
		<script type="text/javascript">
		document.getElementById('tr_entcodent').style.display = 'none';
		document.getElementById('tr_entnuninsest').style.display = 'none';
		document.getElementById('tr_entungcod').style.display = 'none';
		document.getElementById('tr_tpctgid').style.display = 'none';

		$('frmEntidade').onsubmit  = function(e) {
			if (trim($F('entnome')) == '') {
				alert('O nome da entidade é obrigatório.');
				return false;
			}
			return true;
		}

    	</script>
		<?
	break;
	
	case 'esp':
		/* 
		 * TELA de 'Dados específicos' referente  aos campus 
		 */
		$tipolocalidade = definirtipolocalidade($_SESSION['sig_var']['orgid']);
		
		$permCampo = ($permissoes['gravar'] ? 'S' : 'N');
		$permDisab = ($permissoes['gravar'] ? '' : 'disabled="disabled"');
		
		if($_SESSION['sig_var']['iscampus'] == 'sim') {
			echo "<form id='frmEntidadeDetalhes' method='post' action='sig.php?modulo=principal/inserir_entidade&acao=A&exec_function=salvarRegistroDetalhes'>";
			echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
			
			$dadoscampus = $db->carregar("SELECT cam.cmpid, cam.cmpobs, entd.edtdsc, cam.cmpdataimplantacao, cam.cmpdatainauguracao, cam.cmpexistencia, cam.cmpsituacao, cam.cmpsituacao, cam.cmpsituacaoobra, cam.cmpinstalacao FROM academico.campus cam LEFT JOIN academico.entidadedetalhe entd ON entd.entid = cam.entid WHERE cam.entid='".$_SESSION['sig_var']['entid']."'");
			if($dadoscampus)$dadoscampus=current($dadoscampus);
			echo "<input type='hidden' name='cmpid' value='".$dadoscampus['cmpid']."'>";
			
			echo "<tr><td colspan='2'><strong>Dados específicos d".$tipolocalidade['artigo+nome']."</strong></td></tr>";
			if(trim($dadoscampus['cmpdataimplantacao']))
				$cmpdataimplantacao=substr($dadoscampus['cmpdataimplantacao'],4,2)."/".substr($dadoscampus['cmpdataimplantacao'],0,4);
			echo "<tr><td class=\"SubTituloDireita\">Data de implantação :</td><td>".campo_texto('cmpdataimplantacao', "N", $permCampo, "Data de Implantação", 8, 7, "##/####", "", '', '', 0, 'id="cmpdataimplantacao"')." (mm/yyyy)</td></tr>";
			$cmpdatainauguracao=formata_data($dadoscampus['cmpdatainauguracao']);
			echo "<tr><td class=\"SubTituloDireita\">Data de inauguração :</td><td><input $permDisab type=\"radio\" name=\"inaugura\" value=\"sim\" onclick=\"if(this.checked){document.getElementById('cmpdatainauguracao').value='';document.getElementById('cmpdatainauguracao').disabled=true;}else{document.getElementById('cmpdatainauguracao').disabled=false;}\" ".((trim($cmpdatainauguracao))?"":"checked")."> Não inaugurado <input $permDisab type=\"radio\" name=\"inaugura\" value=\"sim\" onclick=\"if(this.checked){document.getElementById('cmpdatainauguracao').disabled=false;}else{document.getElementById('cmpdatainauguracao').value='';document.getElementById('cmpdatainauguracao').disabled=true;}\" ".((trim($cmpdatainauguracao))?"checked":"")."> Inaugurado ".campo_texto('cmpdatainauguracao', "N", $permCampo, "Data de inauguração", 12, 10, "##/##/####", "", '', '', 0, 'id="cmpdatainauguracao"' )." (dd/mm/yyyy)</td></tr>";
			echo "<tr><td class=\"SubTituloDireita\">Existência do campus/uned :</td><td><input $permDisab type='radio' name='cmpexistencia' value='N' ".(($dadoscampus['cmpexistencia']=='N')?'checked':'')."> Novo <input $permDisab type='radio' name='cmpexistencia' value='P' ".(($dadoscampus['cmpexistencia']=='P')?'checked':'')."> Pré-Existente</td></tr>";
			echo "<tr><td class=\"SubTituloDireita\">Situação do campus/uned :</td><td><input $permDisab type='radio' name='cmpsituacao' value='F' ".(($dadoscampus['cmpsituacao']=='F')?'checked':'')." onclick='verificasituacaocampus(this);'> Funcionando <input $permDisab type='radio' name='cmpsituacao' value='N' ".(($dadoscampus['cmpsituacao']=='N')?'checked':'')." onclick='verificasituacaocampus(this);'> Não Funcionando</td></tr>";
			echo "<tr id='trcmpinstalacao' ".(($dadoscampus['cmpsituacao']=='F')?'':'style="display:none"')."><td class=\"SubTituloDireita\">Instalações :</td><td><input $permDisab type='radio' id='cmpinstalacaoP' name='cmpinstalacao' value='P' ".((!$dadoscampus['cmpinstalacao'] || $dadoscampus['cmpinstalacao']=='P')?'checked':'')."> Instalações Provisórias <input $permDisab type='radio' id='cmpinstalacaoD' name='cmpinstalacao' value='D' ".(($dadoscampus['cmpinstalacao']=='D')?'checked':'')."> Instalações Definitivas</td></tr>";
			
			echo "<tr><td class=\"SubTituloDireita\">Situação da obra no campus/uned :</td><td>";
			$cmpsituacaoobra=$dadoscampus['cmpsituacaoobra'];
			$db->monta_combo('cmpsituacaoobra', array(0 =>array("codigo" => "L", "descricao" => "Licitação de Obras"),1 =>array("codigo" => "A", "descricao" => "Obras em Andamento"),2 =>array("codigo" => "C", "descricao" => "Obras Concluídas")), $permCampo, 'Selecione', '', '', '', '200', 'N', 'cmpsituacaoobra');
			echo "</td></tr>";
			$edtdsc = $dadoscampus['edtdsc'];
			echo "<tr><td class='SubTituloDireita' width='340'>Caracterização da Unidade (<font size=1>texto sucinto destacando informações importantes sobre a unidade. Incluir informações sobre sua data de criação e sua importância para a região, dentre outras</font>) :</td><td>".campo_textarea( 'edtdsc', 'N', $permCampo, '', '70', '4', '500')."</td></tr>";
			$cmpobs = $dadoscampus['cmpobs'];
			echo "<tr><td class='SubTituloDireita'>Informações adicionais :</td><td>". campo_textarea( 'cmpobs', 'N', $permCampo, '', '70', '4', '1000')."</td></tr>";
			echo "<tr><td colspan='2' bgcolor=\"#C0C0C0\" align='center'><input type='button' $permDisab onclick=\"if(validardadosespecificos()){document.getElementById('frmEntidadeDetalhes').submit();}\" value='Salvar dados específicos'></td></tr>";
			echo "</table>";
			echo "</form>";
			
			echo "<form name='formulario' id='formulario' method='post' action='sig.php?modulo=principal/inserir_entidade&acao=A&exec_function=salvarProcessoSeletivo'>";
			if($_REQUEST['prsid']) {
				$processoseletivo = $db->carregar("SELECT * FROM academico.processoseletivo WHERE prsid='".$_REQUEST['prsid']."'");
				if($processoseletivo[0]) {
					$processoseletivo=current($processoseletivo);
					$prsinscricaoini = $processoseletivo['prsinscricaoini'];
					$prsinscricaofim = $processoseletivo['prsinscricaofim'];
					$prsprovaini = $processoseletivo['prsprovaini'];
					$prsprovafim = $processoseletivo['prsprovafim'];
					$prsinicioaula = $processoseletivo['prsinicioaula'];
					$prsnrvagas = $processoseletivo['prsnrvagas'];
					echo "<input type='hidden' name='prsid' value='".$processoseletivo['prsid']."'>";
				}
			}
			echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
			echo "<tr><td colspan='2'><strong>Cadastrar processo seletivo de estudantes</strong></td></tr>";
			echo "<tr><td class=\"SubTituloDireita\">Inscrições :</td><td>de ".campo_data( 'prsinscricaoini', 'N', $permCampo, '', 'S' )." até ".campo_data( 'prsinscricaofim', 'N', $permCampo, '', 'S' )."</td>";
			echo "<tr><td class=\"SubTituloDireita\">Provas :</td><td>de ".campo_data( 'prsprovaini', 'N', $permCampo, '', 'S' )." até ".campo_data( 'prsprovafim', 'N', $permCampo, '', 'S' )."</td>";
			echo "<tr><td class=\"SubTituloDireita\">Data de início das aulas :</td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;".campo_data( 'prsinicioaula', 'N', $permCampo, '', 'S' )."</td>";
			echo "<tr><td class=\"SubTituloDireita\">Número de vagas ofertadas :</td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;". campo_texto('prsnrvagas', "N", $permCampo, "Número de vagas ofertadas", 12, 10, "##########", "", '', '', 0, '' )."</td>";
			echo "<tr><td bgcolor=\"#C0C0C0\" align='center' colspan='2'><input $permDisab type='button' onclick=\"return validaprocessoseletivo();this.disabled=true;\" value='Salvar processo seletivo' ><input $permDisab type='button' value='Novo' onclick=\"window.location='?modulo=principal/inserir_entidade&acao=A&page=esp';this.disabled=true;\"></td>";
			echo "</table>";
			echo "</form>";

			if ($permCampo == 'S'){
				$img = "<img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"Excluir(\'?modulo=principal/inserir_entidade&acao=A&exec_function=salvarProcessoSeletivo&rmprsid='|| prsid ||'\',\'Deseja realmente excluir este processo seletivo?\');\">";	
			}else{
				$img = "<img src=\"/imagens/excluir_01.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"alert(\'Permissão negada!\');\">";					
			}	
			$sql = "SELECT '<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=principal/inserir_entidade&acao=A&page=esp&prsid='|| prsid ||'\'\"> {$img}</center>', 'de '||(to_char(prsinscricaoini,'DD/MM/YYYY') ||' até '|| to_char(prsinscricaofim,'DD/MM/YYYY')) as perinsc, 'de '||(to_char(prsprovaini,'DD/MM/YYYY') ||' até '|| to_char(prsprovafim,'DD/MM/YYYY')) as perprova, to_char(prsinicioaula,'DD/MM/YYYY'), prsnrvagas FROM academico.campus cam
				    INNER JOIN academico.processoseletivo prs ON prs.cmpid = cam.cmpid  
					WHERE cam.entid='".$_SESSION['sig_var']['entid']."'";
		
			$cabecalho = array("","Inscrição","Provas","Início das aulas","Número de vagas");
			echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
			echo "<tr><td class=\"SubTituloCentro\" colspan='4'><strong>Lista de processos seletivos</strong></td></tr>";
			echo "<tr><td>";
			$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '100%','N');
			echo "</td></tr>";
			echo "</table>";
		/* 
		 * FIM - TELA de 'Dados específicos' referente  aos campus 
		 */
		} elseif($_SESSION['sig_var']['iscampus'] == 'nao') {
			/* 
			 * TELA de 'Dados específicos' referente  as unidades orçamentárias 
			 */
			echo "<form id='frmEntidadeDetalhes' method='post' action='sig.php?modulo=principal/inserir_entidade&acao=A&exec_function=salvarRegistroDetalhes'>";
			echo "<table  class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
			
			$dadoscampus = $db->carregar("SELECT entd.edtdsc FROM academico.entidadedetalhe entd WHERE entd.entid='".$_SESSION['sig_var']['entid']."'");
			if($dadoscampus)$dadoscampus=current($dadoscampus);
			echo "<tr><td colspan='2'><strong>Dados específicos da Unidade</strong></td></tr>";
			$edtdsc = $dadoscampus['edtdsc'];
			echo "<tr><td class='SubTituloDireita'>Caracterização da Unidade (texto sucinto destacando informações importantes sobre a unidade. Incluir informações sobre sua data de criação e sua importância para a região, dentre outras) :</td><td>".campo_textarea( 'edtdsc', 'N', 'N', '', '70', '4', '200')."</td></tr>";
			echo "<tr><td colspan='2' bgcolor=\"#C0C0C0\" align='center'>";
			if($permissoes['gravar']) {
				echo "<input type='button' disabled=\"disabled\"  onclick=\"document.getElementById('frmEntidadeDetalhes').submit();this.disabled=true;\" value='Salvar'>";
			} else {
				echo "&nbsp;";
			}
			echo "</td></tr>";
			echo "</table>";
			echo "</form>";
			/* 
			 * FIM - TELA de 'Dados específicos' referente  as unidades orçamentárias 
			 */
			
		}

	break;
}

?>
</div>

  </body>
</html>
